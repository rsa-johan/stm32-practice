pub inline fn reg(comptime T: type, address: usize) *volatile T {
    return @as(*volatile T, @ptrFromInt(address));
}

pub inline fn setBits(comptime T: type, address: usize, mask: T) void {
    const r = reg(T, address);
    r.* |= mask;
}

pub inline fn clearBits(comptime T: type, address: usize, mask: T) void {
    const r = reg(T, address);
    r.* &= ~mask;
}
