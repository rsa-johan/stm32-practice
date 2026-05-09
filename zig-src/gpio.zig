const af = @import("af.zig");

const base: usize = 0x4800_0000;

pub const Port = enum(usize) {
    a = base + 0x0000,
    b = base + 0x0400,
    c = base + 0x0800,
    d = base + 0x0c00,
    e = base + 0x1000,
    f = base + 0x1400,
    g = base + 0x1800,
};

pub const Pin = enum(u5) {
    p0 = 0,
    p1 = 1,
    p2 = 2,
    p3 = 3,
    p4 = 4,
    p5 = 5,
    p6 = 6,
    p7 = 7,
    p8 = 8,
    p9 = 9,
    p10 = 10,
    p11 = 11,
    p12 = 12,
    p13 = 13,
    p14 = 14,
    p15 = 15,
};

pub const Mode = enum(u2) {
    input = 0,
    output = 1,
    alternate = 2,
    analog = 3,
};

pub const Output = enum(u1) {
    push_pull = 0,
    open_drain = 1,
};

pub const Pull = enum(u2) {
    none = 0,
    up = 1,
    down = 2,
};

pub const Level = enum(u1) {
    low = 0,
    high = 1,
};

const Registers = extern struct {
    moder: u32,
    otyper: u32,
    ospeedr: u32,
    pupdr: u32,
    idr: u32,
    odr: u32,
    bsrr: u32,
    lckr: u32,
    afr: [2]u32,
    brr: u32,
};

pub const PinConfig = struct {
    port: Port,
    pin: Pin,
    mode: Mode,
    output: Output = .push_pull,
    pull: Pull = .none,
    alternate: af.Type = af.none,

    inline fn regs(self: PinConfig) *volatile Registers {
        return @as(*volatile Registers, @ptrFromInt(@intFromEnum(self.port)));
    }

    inline fn index(self: PinConfig) u5 {
        return @intFromEnum(self.pin);
    }
};

pub fn init() void {}

pub fn configure(pin: PinConfig) void {
    const regs = pin.regs();
    const idx = pin.index();
    const mode_shift: u5 = idx * 2;
    const mode_mask: u32 = @as(u32, 0b11) << mode_shift;
    const pin_mask: u32 = @as(u32, 1) << idx;

    regs.moder = (regs.moder & ~mode_mask) | (@as(u32, @intFromEnum(pin.mode)) << mode_shift);

    if (pin.output == .open_drain) {
        regs.otyper |= pin_mask;
    } else {
        regs.otyper &= ~pin_mask;
    }

    regs.pupdr = (regs.pupdr & ~mode_mask) | (@as(u32, @intFromEnum(pin.pull)) << mode_shift);

    if (pin.mode == .alternate) {
        const afr_index: usize = @as(usize, idx) >> 3;
        const afr_shift: u5 = (idx & 0x7) * 4;
        regs.afr[afr_index] = (regs.afr[afr_index] & ~(@as(u32, 0xf) << afr_shift)) |
            (@as(u32, pin.alternate) << afr_shift);
    }
}

pub inline fn read(pin: PinConfig) Level {
    const bit = (pin.regs().odr >> pin.index()) & 1;
    return @enumFromInt(bit);
}

pub inline fn set(pin: PinConfig) void {
    pin.regs().bsrr = @as(u32, 1) << pin.index();
}

pub inline fn clear(pin: PinConfig) void {
    pin.regs().bsrr = @as(u32, 1) << (@as(u5, pin.index()) + 16);
}
