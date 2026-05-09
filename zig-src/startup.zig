const main = @import("main.zig");
const thread = @import("thread.zig");
const timer = @import("timer.zig");

extern var _sidata: u8;
extern var _sdata: u8;
extern var _edata: u8;
extern var _sbss: u8;
extern var _ebss: u8;

fn Default_Handler() callconv(.c) void {
    while (true) {}
}

export fn Reset_Handler() callconv(.c) void {
    initData();
    zeroBss();
    main.main();
    while (true) {}
}

fn initData() void {
    var src: [*]const u8 = @ptrCast(&_sidata);
    var dst: [*]u8 = @ptrCast(&_sdata);
    const end = @intFromPtr(&_edata);

    while (@intFromPtr(dst) < end) {
        dst[0] = src[0];
        dst += 1;
        src += 1;
    }
}

fn zeroBss() void {
    var dst: [*]u8 = @ptrCast(&_sbss);
    const end = @intFromPtr(&_ebss);

    while (@intFromPtr(dst) < end) {
        dst[0] = 0;
        dst += 1;
    }
}

const stack_top: usize = 0x2000_0000 + (128 * 1024);
const Vector = ?*const anyopaque;

export const vector_table linksection(".isr_vector") = [_]Vector{
    @ptrFromInt(stack_top),
    @ptrCast(&Reset_Handler),
    @ptrCast(&Default_Handler), // NMI
    @ptrCast(&Default_Handler), // HardFault
    @ptrCast(&Default_Handler), // MemManage
    @ptrCast(&Default_Handler), // BusFault
    @ptrCast(&Default_Handler), // UsageFault
    @ptrCast(&Default_Handler),
    @ptrCast(&Default_Handler),
    @ptrCast(&Default_Handler),
    @ptrCast(&Default_Handler),
    @ptrCast(&thread.SVC_Handler),
    @ptrCast(&Default_Handler),
    @ptrCast(&Default_Handler),
    @ptrCast(&thread.PendSV_Handler),
    @ptrCast(&thread.SysTick_Handler),
} ++ [_]Vector{@ptrCast(&Default_Handler)} ** 28 ++ [_]Vector{
    @ptrCast(&timer.TIM2_IRQHandler),
    @ptrCast(&timer.TIM3_IRQHandler),
    @ptrCast(&timer.TIM4_IRQHandler),
} ++ [_]Vector{@ptrCast(&Default_Handler)} ** 19 ++ [_]Vector{
    @ptrCast(&timer.TIM5_IRQHandler),
};
