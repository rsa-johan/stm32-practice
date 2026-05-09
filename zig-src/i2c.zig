const af = @import("af.zig");
const gpio = @import("gpio.zig");

pub const Error = error{
    BusyTimeout,
    TransferTimeout,
    Nack,
    InvalidLength,
};

const Registers = extern struct {
    cr1: u32,
    cr2: u32,
    oar1: u32,
    oar2: u32,
    timingr: u32,
    timeoutr: u32,
    isr: u32,
    icr: u32,
    pecr: u32,
    rxdr: u32,
    txdr: u32,
};

const base_i2c3: usize = 0x4000_5c00;

const cr1_pe: u32 = 1 << 0;
const cr1_rxie: u32 = 1 << 2;
const cr1_tcie: u32 = 1 << 6;
const cr1_anfoff: u32 = 1 << 12;
const cr1_nostretch: u32 = 1 << 17;

const cr2_sadd_mask: u32 = 0x3ff;
const cr2_rd_wrn: u32 = 1 << 10;
const cr2_start: u32 = 1 << 13;
const cr2_nbytes_mask: u32 = 0xff << 16;
const cr2_autoend: u32 = 1 << 25;
const cr2_transfer_mask: u32 = cr2_sadd_mask | cr2_rd_wrn | cr2_nbytes_mask | cr2_autoend;

const isr_txis: u32 = 1 << 1;
const isr_rxne: u32 = 1 << 2;
const isr_nackf: u32 = 1 << 4;
const isr_stopf: u32 = 1 << 5;
const isr_busy: u32 = 1 << 15;

const icr_nackcf: u32 = 1 << 4;
const icr_stopcf: u32 = 1 << 5;

pub const timing_100khz_16mhz: u32 = 0x40b2_85c2;
pub const timeout: u32 = 100_000;
pub const max_transfer: usize = 255;

pub const Mode = enum { controller, target };

pub const Bus = struct {
    registers: *volatile Registers,
    sda: gpio.PinConfig,
    scl: gpio.PinConfig,
    address: u10,
    timing: u32,
    mode: Mode = .controller,
    interrupts: bool = false,

    pub fn setup(self: Bus) void {
        gpio.configure(self.sda);
        gpio.configure(self.scl);

        self.registers.cr1 &= ~cr1_pe;

        var cr1 = self.registers.cr1 & ~(cr1_pe | cr1_anfoff | cr1_nostretch | cr1_rxie | cr1_tcie);
        if (self.mode == .target) cr1 |= cr1_nostretch;
        if (self.interrupts) cr1 |= cr1_rxie | cr1_tcie;

        self.registers.cr1 = cr1;
        self.registers.timingr = self.timing;
        clearFlags(self.registers);
        self.registers.cr1 |= cr1_pe;
    }

    pub fn write(self: Bus, data: []const u8) Error!void {
        if (data.len == 0 or data.len > max_transfer) return Error.InvalidLength;
        try waitClear(self.registers, isr_busy, Error.BusyTimeout);
        clearFlags(self.registers);
        start(self, @intCast(data.len), false);

        for (data) |byte| {
            try waitSet(self.registers, isr_txis);
            self.registers.txdr = byte;
        }

        try waitStop(self.registers);
    }

    pub fn read(self: Bus, data: []u8) Error!void {
        if (data.len == 0 or data.len > max_transfer) return Error.InvalidLength;
        try waitClear(self.registers, isr_busy, Error.BusyTimeout);
        clearFlags(self.registers);
        start(self, @intCast(data.len), true);

        for (data) |*byte| {
            try waitSet(self.registers, isr_rxne);
            byte.* = @truncate(self.registers.rxdr);
        }

        try waitStop(self.registers);
    }
};

pub const audio = Bus{
    .registers = @as(*volatile Registers, @ptrFromInt(base_i2c3)),
    .sda = .{ .port = .g, .pin = .p8, .mode = .alternate, .output = .open_drain, .pull = .none, .alternate = af.i2c3 },
    .scl = .{ .port = .g, .pin = .p7, .mode = .alternate, .output = .open_drain, .pull = .none, .alternate = af.i2c3 },
    .address = 0x34,
    .timing = timing_100khz_16mhz,
};

pub fn init() void {
    audio.setup();
}

fn start(bus: Bus, len: u8, read: bool) void {
    var cr2 = bus.registers.cr2 & ~cr2_transfer_mask;
    cr2 |= @as(u32, bus.address);
    cr2 |= @as(u32, len) << 16;
    cr2 |= cr2_autoend;
    if (read) cr2 |= cr2_rd_wrn;
    bus.registers.cr2 = cr2 | cr2_start;
}

fn clearFlags(regs: *volatile Registers) void {
    regs.icr = icr_nackcf | icr_stopcf;
}

fn waitClear(regs: *volatile Registers, mask: u32, err: Error) Error!void {
    var remaining: u32 = timeout;
    while ((regs.isr & mask) != 0) {
        if (remaining == 0) return err;
        remaining -= 1;
    }
}

fn waitSet(regs: *volatile Registers, mask: u32) Error!void {
    var remaining: u32 = timeout;
    while ((regs.isr & mask) == 0) {
        const status = regs.isr;
        if ((status & isr_nackf) != 0) {
            clearFlags(regs);
            return Error.Nack;
        }
        if ((status & isr_stopf) != 0) {
            clearFlags(regs);
            return Error.TransferTimeout;
        }
        if (remaining == 0) return Error.TransferTimeout;
        remaining -= 1;
    }
}

fn waitStop(regs: *volatile Registers) Error!void {
    try waitSet(regs, isr_stopf);
    clearFlags(regs);
}
