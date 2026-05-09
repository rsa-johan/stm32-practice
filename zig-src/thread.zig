const mmio = @import("mmio.zig");
const timer = @import("timer.zig");

pub const max_tasks = 4;
pub const max_stack_words = 256;

const scb_icsr: usize = 0xe000_ed04;
const scb_shpr3: usize = 0xe000_ed20;
const syst_csr: usize = 0xe000_e010;
const syst_rvr: usize = 0xe000_e014;
const syst_cvr: usize = 0xe000_e018;

const pendsv_set: u32 = 1 << 28;

pub const TaskIndex = u32;
pub const Tick = u32;
pub const Delay = timer.Delay;
pub const Unit = timer.Unit;

const TaskFn = *const fn (?*anyopaque) callconv(.c) void;

const State = enum(u32) {
    unused = 0,
    active = 1,
    suspended = 2,
    completed = 3,
};

const TaskControlBlock = extern struct {
    stack_pointer: usize,
    priority: u32,
    state: State,
    end_tick: Tick,
    name: ?[*:0]const u8,
};

var tick: Tick = 0;
var tasks: [max_tasks]TaskControlBlock = [_]TaskControlBlock{emptyTask()} ** max_tasks;
var stacks: [max_tasks][max_stack_words]usize align(8) = undefined;
var current_index: TaskIndex = 0;
var created_count: u32 = 0;

export var current_task: usize = 0;
export var initial_psp: usize = 0;

fn emptyTask() TaskControlBlock {
    return .{
        .stack_pointer = 0,
        .priority = 0,
        .state = .unused,
        .end_tick = 0,
        .name = null,
    };
}

pub fn create(task_fn: TaskFn, name: ?[*:0]const u8, stack_bytes: u16, parameters: ?*anyopaque, priority: u32) void {
    for (&tasks, 0..) |*task, i| {
        if (task.state != .unused) continue;

        var words = (@as(usize, stack_bytes) + @sizeOf(usize) - 1) / @sizeOf(usize);
        if (words < 64) words = 64;
        if (words > max_stack_words) words = max_stack_words;

        var sp = @intFromPtr(&stacks[i][0]) + (words * @sizeOf(usize));
        sp &= ~@as(usize, 0x7);

        sp = push(sp, 0x0100_0000);
        sp = push(sp, @intFromPtr(task_fn));
        sp = push(sp, @intFromPtr(&endTask));
        sp = push(sp, 0);
        sp = push(sp, 0);
        sp = push(sp, 0);
        sp = push(sp, 0);
        sp = push(sp, @intFromPtr(parameters));

        for (0..8) |_| {
            sp = push(sp, 0);
        }

        task.* = .{
            .stack_pointer = sp,
            .priority = priority,
            .state = .active,
            .end_tick = 0,
            .name = name,
        };
        created_count += 1;
        return;
    }
}

fn push(sp: usize, value: usize) usize {
    const next = sp - @sizeOf(usize);
    @as(*usize, @ptrFromInt(next)).* = value;
    return next;
}

pub export fn scheduleNextTask() callconv(.c) void {
    if (created_count == 0) {
        current_task = 0;
        return;
    }

    while (true) {
        var completed: u32 = 0;
        var offset: u32 = 1;

        while (offset <= created_count) : (offset += 1) {
            const next = (current_index + offset) % created_count;
            const task = &tasks[next];

            if (task.state == .suspended and tick >= task.end_tick) {
                task.state = .active;
            }

            switch (task.state) {
                .active => {
                    current_index = next;
                    current_task = @intFromPtr(task);
                    return;
                },
                .completed => completed += 1,
                else => {},
            }
        }

        if (completed >= created_count) {
            current_task = 0;
            return;
        }

        asm volatile ("wfi");
    }
}

pub export fn endTask() callconv(.c) void {
    if (current_task != 0) {
        @as(*TaskControlBlock, @ptrFromInt(current_task)).state = .completed;
    }

    yield();
    while (true) {}
}

pub fn yield() void {
    mmio.reg(u32, scb_icsr).* = pendsv_set;
    asm volatile ("dsb");
    asm volatile ("isb");
}

pub fn delay(amount: Delay, unit: Unit) void {
    sleepTicks(amount * @intFromEnum(unit));
}

pub fn sleepTicks(delay_ticks: Delay) void {
    if (current_task != 0) {
        const task: *TaskControlBlock = @ptrFromInt(current_task);
        task.state = .suspended;
        if (delay_ticks > 0) {
            task.end_tick = tick + delay_ticks;
        }
    }
    mmio.reg(u32, scb_icsr).* = pendsv_set;
}

pub fn resumeTask(index: TaskIndex) void {
    if (index < max_tasks and tasks[index].state == .suspended) {
        tasks[index].state = .active;
    }
    mmio.reg(u32, scb_icsr).* = pendsv_set;
}

pub fn run() void {
    current_index = 0;
    tick = 0;
    current_task = 0;

    while (current_index < max_tasks and tasks[current_index].state != .active) {
        current_index += 1;
    }

    if (current_index >= max_tasks) return;

    current_task = @intFromPtr(&tasks[current_index]);

    mmio.reg(u32, scb_shpr3).* &= ~@as(u32, (0xff << 16) | (0xff << 24));
    mmio.reg(u32, scb_shpr3).* |= @as(u32, (0xff << 16) | (0xef << 24));

    mmio.reg(u32, syst_rvr).* = 16_000 - 1;
    mmio.reg(u32, syst_cvr).* = 0;
    mmio.reg(u32, syst_csr).* = (1 << 2) | (1 << 1) | (1 << 0);

    initial_psp = @as(*TaskControlBlock, @ptrFromInt(current_task)).stack_pointer;
    asm volatile (
        \\ldr r0, =initial_psp
        \\ldr r0, [r0]
        \\msr psp, r0
    );
    asm volatile ("svc #0");
}

pub export fn SysTick_Handler() callconv(.c) void {
    tick += 1;
    mmio.reg(u32, scb_icsr).* = pendsv_set;
}

pub export fn SVC_Handler() callconv(.naked) void {
    asm volatile (
        \\ldr r1, =current_task
        \\ldr r2, [r1]
        \\ldr r0, [r2]
        \\ldmia r0!, {r4-r11}
        \\msr psp, r0
        \\movs r0, #2
        \\msr control, r0
        \\isb
        \\ldr lr, =0xfffffffd
        \\bx lr
    );
}

pub export fn PendSV_Handler() callconv(.naked) void {
    asm volatile (
        \\mrs r0, psp
        \\stmdb r0!, {r4-r11}
        \\ldr r1, =current_task
        \\ldr r2, [r1]
        \\str r0, [r2]
        \\push {lr}
        \\bl scheduleNextTask
        \\pop {lr}
        \\ldr r1, =current_task
        \\ldr r2, [r1]
        \\ldr r0, [r2]
        \\ldmia r0!, {r4-r11}
        \\msr psp, r0
        \\bx lr
    );
}
