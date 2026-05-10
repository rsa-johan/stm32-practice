# PROPOSED: Software Structure

This document describes the intended source structure for the firmware. The top-level organization follows a hardware abstraction and feature split.

## src

`src/` contains the main application and platform-independent modules.

- `main.c`
  - Program entry point.
  - Initializes the board, clock, peripherals, and scheduler.
  - Starts the main application loop or RTOS tasks.

- `util/` or `util.c`
  - Helper functions used across modules.
  - Common software utilities, conversions, logging support, or data helpers.

- `base/`
  - Low-level core support code.
  - May include startup code, system initialization, exception/interrupt wrappers, and basic runtime services.

- `functions/`
  - Higher-level application logic.
  - Implements feature coordination between drivers and the main system.

- `isr/`
  - Interrupt service routine handlers.
  - Maps hardware interrupts to software callbacks or event signaling.


## drivers and peripheral modules

The platform-specific drivers are grouped by function. Each module provides a simple interface for the rest of the firmware.

- `clock.c`
  - System clock and PLL configuration.
  - Sets up bus clocks, timers, and clock sources.

- `gpio.c`
  - General-purpose I/O configuration.
  - Controls pin modes, input/output state, and pin-level operations.

- `i2c.c`
  - I2C bus initialization and transfers.
  - Supports master mode communication with sensors or external devices.

- `adc.c`
  - Analog-to-digital converter setup.
  - Reads analog signals and converts them to digital values.

- `can.c`
  - CAN bus initialization and messaging.
  - Sends and receives CAN frames for vehicle or network communication.

- `spi.c`
  - SPI peripheral configuration.
  - Performs serial transfers for SPI devices.

## logical feature modules

These modules depend on the drivers above, providing one step of abstraction for application features.

- `led.c`
  - Controls status LEDs and their blink patterns.
  - Wraps GPIO operations for LED feedback.

- `audio.c`
  - Audio output or codec support.
  - Converts audio data into peripheral commands.

- `motor.c`
  - Motor control and drive logic.
  - Interfaces with PWM, GPIO, and sensor feedback.

- `lcd.c`
  - LCD display driver and text rendering.
  - Provides functions to show information on a screen.

## recommended file responsibilities

- Keep hardware access and register-level code in the low-level driver modules (`clock`, `gpio`, `i2c`, `adc`, `can`, `spi`).
- Keep application behavior in `main.c`, `functions/`, and feature modules like `led`, `audio`, `motor`, and `lcd`.
- Group shared utilities in `util/` and core platform support in `base/`.
- Place interrupt dispatch and callback handling in `isr/`, with minimal logic in the handlers themselves.

This structure helps separate peripheral control, application features, and shared runtime support while keeping each file responsibility small and clear.