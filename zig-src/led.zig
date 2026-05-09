const af = @import("af.zig");
const gpio = @import("gpio.zig");

pub const Led = struct {
    pin: gpio.PinConfig,

    pub fn on(self: Led) void {
        gpio.clear(self.pin);
    }

    pub fn off(self: Led) void {
        gpio.set(self.pin);
    }

    pub fn toggle(self: Led) void {
        if (gpio.read(self.pin) == .high) {
            self.on();
        } else {
            self.off();
        }
    }
};

pub const led1 = Led{ .pin = .{
    .port = .g,
    .pin = .p9,
    .mode = .output,
    .output = .push_pull,
    .pull = .down,
    .alternate = af.none,
} };

pub const led2 = Led{ .pin = .{
    .port = .f,
    .pin = .p11,
    .mode = .output,
    .output = .push_pull,
    .pull = .down,
    .alternate = af.none,
} };

pub fn init() void {
    gpio.configure(led1.pin);
    gpio.configure(led2.pin);
    led1.off();
    led2.on();
}
