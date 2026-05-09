const gpio = @import("gpio.zig");
const i2c = @import("i2c.zig");
const led = @import("led.zig");
const rcc = @import("rcc.zig");
const thread = @import("thread.zig");
const timer = @import("timer.zig");
comptime {
    _ = @import("startup.zig");
}

fn ledRun0(_: ?*anyopaque) callconv(.c) void {
    while (true) {
        led.led1.toggle();
        thread.delay(1, .s);
    }
}

fn ledRun1(_: ?*anyopaque) callconv(.c) void {
    while (true) {
        led.led2.toggle();
        thread.delay(3, .s);
    }
}

pub export fn main() callconv(.c) void {
    rcc.systemClockInit();
    rcc.init();
    gpio.init();
    timer.init();
    i2c.init();
    led.init();

    thread.create(ledRun0, "task0", 256, null, 1);
    thread.create(ledRun1, "task1", 256, null, 2);
    thread.run();
}
