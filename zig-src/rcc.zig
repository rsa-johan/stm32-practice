const mmio = @import("mmio.zig");

const base: usize = 0x4002_1000;

const cr = base + 0x00;
const ahb2rstr = base + 0x2c;
const apb1rstr1 = base + 0x38;
const ahb2enr = base + 0x4c;
const apb1enr1 = base + 0x58;

pub const hsi_on: u32 = 1 << 8;
pub const hsi_ready: u32 = 1 << 10;

pub const gpio_f: u32 = 1 << 5;
pub const gpio_g: u32 = 1 << 6;

pub const tim2: u32 = 1 << 0;
pub const tim3: u32 = 1 << 1;
pub const tim4: u32 = 1 << 2;
pub const tim5: u32 = 1 << 3;

pub const i2c3: u32 = 1 << 23;

pub fn systemClockInit() void {
    mmio.setBits(u32, cr, hsi_on);
    while ((mmio.reg(u32, cr).* & hsi_ready) == 0) {}
}

pub fn init() void {
    reset(ahb2rstr, gpio_f | gpio_g);
    reset(apb1rstr1, tim2 | tim3 | tim4 | tim5 | i2c3);

    mmio.setBits(u32, ahb2enr, gpio_f | gpio_g);
    mmio.setBits(u32, apb1enr1, tim2 | tim3 | tim4 | tim5 | i2c3);
}

inline fn reset(comptime address: usize, mask: u32) void {
    mmio.setBits(u32, address, mask);
    mmio.clearBits(u32, address, mask);
}
