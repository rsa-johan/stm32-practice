const mmio = @import("mmio.zig");

const timer_base: usize = 0x4000_0000;
const nvic_iser_base: usize = 0xe000_e100;

const cr_offset: usize = 0x000;
const dier_offset: usize = 0x00c;
const sr_offset: usize = 0x010;
const egr_offset: usize = 0x014;
const cnt_offset: usize = 0x024;
const psc_offset: usize = 0x028;
const arr_offset: usize = 0x02c;

const cr_cen: u32 = 1 << 0;
const dier_uie: u32 = 1 << 0;
const sr_uif: u32 = 1 << 0;
const egr_ug: u32 = 1 << 0;

pub const Delay = u32;
pub const Unit = enum(u32) {
    ms = 1,
    s = 1000,
};

pub const Timer = enum(usize) {
    tim2 = 0x0000,
    tim3 = 0x0400,
    tim4 = 0x0800,
    tim5 = 0x0c00,
};

const timer_clock_khz: u32 = 16_000;

pub fn init() void {
    inline for (.{ Timer.tim2, Timer.tim3, Timer.tim4, Timer.tim5 }) |timer| {
        setPrescaler(timer, timer_clock_khz);
        disableInterrupt(timer);
        updateRegisters(timer);
        reset(timer);
        enableNvic(timer);
    }

    asm volatile ("cpsie i");
}

pub fn startInterrupt(timer: Timer, delay: Delay, unit: Unit) void {
    reset(timer);
    setReload(timer, delay * @intFromEnum(unit));
    enableInterrupt(timer);
    reg(timer, cr_offset).* |= cr_cen;
}

fn irq(timer: Timer) u6 {
    return switch (timer) {
        .tim2 => 28,
        .tim3 => 29,
        .tim4 => 30,
        .tim5 => 50,
    };
}

fn reg(timer: Timer, comptime offset: usize) *volatile u32 {
    return mmio.reg(u32, timer_base + @intFromEnum(timer) + offset);
}

fn setPrescaler(timer: Timer, value: u32) void {
    reg(timer, psc_offset).* = value - 1;
}

fn setReload(timer: Timer, value: u32) void {
    reg(timer, arr_offset).* = value - 1;
}

fn enableInterrupt(timer: Timer) void {
    reg(timer, dier_offset).* |= dier_uie;
}

fn disableInterrupt(timer: Timer) void {
    reg(timer, dier_offset).* &= ~dier_uie;
}

fn clearUpdateFlag(timer: Timer) void {
    if ((reg(timer, sr_offset).* & sr_uif) != 0) {
        reg(timer, sr_offset).* &= ~sr_uif;
    }
}

fn updateRegisters(timer: Timer) void {
    reg(timer, egr_offset).* |= egr_ug;
}

fn reset(timer: Timer) void {
    reg(timer, cnt_offset).* = 0;
    reg(timer, cr_offset).* &= ~cr_cen;
    clearUpdateFlag(timer);
}

fn enableNvic(timer: Timer) void {
    const bit = irq(timer);
    const iser = nvic_iser_base + ((@as(usize, bit) >> 5) << 2);
    mmio.reg(u32, iser).* = @as(u32, 1) << @as(u5, @truncate(bit));
}

pub export fn TIM2_IRQHandler() callconv(.c) void {
    handleTimer(.tim2);
}

pub export fn TIM3_IRQHandler() callconv(.c) void {
    handleTimer(.tim3);
}

pub export fn TIM4_IRQHandler() callconv(.c) void {
    handleTimer(.tim4);
}

pub export fn TIM5_IRQHandler() callconv(.c) void {
    handleTimer(.tim5);
}

fn handleTimer(timer: Timer) void {
    disableInterrupt(timer);
    clearUpdateFlag(timer);
}
