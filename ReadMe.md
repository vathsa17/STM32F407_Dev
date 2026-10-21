## STM32F407G-DISCOVERY — UART, Push Button and LED Demo

This project demonstrates UART communication, push-button input handling, and on-board LED control using the STM32F407G-DISCOVERY development board.

### Features

- **UART2 communication:** Asynchronous serial reception using USART2 interrupts.
- **Push-button handling:** Press the on-board user button (B1) to transmit `Button Pressed` to the PC.
- **LED control:** Send commands from a PC terminal to control the on-board blue LED.
- **Timer:** TIM6 is configured for timing operations.

### System Overview

![STM32F407G-DISCOVERY UART, Push Button and LED Demo](./docs/images/uart-button-led-demo.png)

### UART Configuration

| Parameter | Configuration |
|---|---|
| Peripheral | USART2 |
| Baud rate | 9600 |
| Data bits | 8 |
| Parity | None |
| Stop bits | 1 |
| Reception | Interrupt-driven |

### How to Use

1. Connect the STM32F407G-DISCOVERY board to the PC.
2. Open a serial terminal such as PuTTY and configure the appropriate COM port.
3. Press the on-board user button (B1). The terminal displays `Button Pressed`.
4. Send `LED_ON` to turn on the on-board blue LED.
5. Send `LED_OFF` to turn off the on-board blue LED.

**Note:** Ensure that the terminal settings match the UART configuration and that the selected COM port corresponds to the board's serial connection.
