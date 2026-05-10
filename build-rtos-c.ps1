$ErrorActionPreference = "Stop"

$toolchainFile = Join-Path $PSScriptRoot "freertos-src\cmake\gnu-tools-for-stm32.cmake"

cmake `
  -S .\freertos-src `
  -B .\build\rtos-gcc `
  -G Ninja `
  -DCMAKE_TOOLCHAIN_FILE="$toolchainFile" `
  -DCMSIS_Dcore=Cortex-M4 `
  -DCMSIS_Dfpu=NO_FPU `
  -DCMSIS_Dendian=Little-endian `
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON `
  -DCMAKE_BUILD_TYPE=Debug

cmake --build .\build\rtos-gcc
