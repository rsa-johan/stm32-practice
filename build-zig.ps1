$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$env:ZIG_GLOBAL_CACHE_DIR = Join-Path $root "build\zig-global-cache"
$env:ZIG_LOCAL_CACHE_DIR = Join-Path $root "build\zig-local-cache"

zig build-exe `
  -target thumb-freestanding-eabihf `
  -mcpu cortex_m4 `
  -O ReleaseSmall `
  -fentry=Reset_Handler `
  --script stm32g474xe_zig.ld `
  -femit-bin=build/uraura-zig `
  zig-src/main.zig
