$ErrorActionPreference = "Stop"

# Run from the script directory so relative paths resolve correctly
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
Push-Location $root

# Ensure build directories exist
New-Item -ItemType Directory -Force -Path (Join-Path $root "build") | Out-Null
New-Item -ItemType Directory -Force -Path (Join-Path $root "build\zig-global-cache") | Out-Null
New-Item -ItemType Directory -Force -Path (Join-Path $root "build\zig-local-cache") | Out-Null

$env:ZIG_GLOBAL_CACHE_DIR = Join-Path $root "build\zig-global-cache"
$env:ZIG_LOCAL_CACHE_DIR = Join-Path $root "build\zig-local-cache"

$emitBin = Join-Path $root "build/uraura-zig"
$scriptFile = Join-Path $root "zig-src/link/stm32g474xe_zig.ld"
$srcFile = Join-Path $root "zig-src/main.zig"

& zig build-exe `
  -target thumb-freestanding-eabihf `
  -mcpu cortex_m4 `
  -O ReleaseSmall `
  -fentry=Reset_Handler `
  --script $scriptFile `
  -femit-bin $emitBin `
  $srcFile

Pop-Location

