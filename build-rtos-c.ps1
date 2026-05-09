$ErrorActionPreference = "Stop"

# $armGcc = Get-Command arm-none-eabi-gcc -ErrorAction SilentlyContinue
# if (-not $armGcc) {
#   throw "arm-none-eabi-gcc is not on PATH. Install GNU Arm Embedded Toolchain or add the STM32 toolchain bin directory to PATH."
# }

$toolchainFile = Join-Path $PSScriptRoot "freertos-src\cmake\gnu-tools-for-stm32.cmake"

cmake `
  -S .\freertos-src `
  -B .\build\rtos-gcc `
  -DCMAKE_TOOLCHAIN_FILE="$toolchainFile" `
  -DCMSIS_Dcore=Cortex-M4 `
  -DCMSIS_Dfpu=NO_FPU `
  -DCMSIS_Dendian=Little-endian `
  -DCMAKE_BUILD_TYPE=Debug
