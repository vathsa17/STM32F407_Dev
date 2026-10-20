# STM32F407: UART, Push Button and LED Control

## Overview

This project demonstrates UART communication, push-button input, and onboard LED control using the **STM32F407G-DISCOVERY** development board.

The application uses the USART2 peripheral to communicate with a PC, reads the onboard user button, and controls the onboard blue LED based on commands received over UART. USART2 interrupt handling enables asynchronous reception of serial data, while TIM6 is configured for timing operations.

## Features

- **UART Communication:** Bidirectional serial communication between the STM32 board and a PC.
- **Push-Button Input:** Pressing the onboard user button sends the message `Button Pressed` to the PC.
- **LED Control:** Control the onboard blue LED by sending commands from the PC.
- **Interrupt-Driven Reception:** USART2 handles received data using interrupt-based processing.
- **Timer Configuration:** TIM6 is initialized for timing operations.

## Hardware Requirements

- STM32F407G-DISCOVERY development board
- PC with a serial terminal application
- USB connection for board programming and serial communication, as supported by the configured hardware

## UART Configuration

| Parameter | Configuration |
|---|---|
| Peripheral | USART2 |
| Baud rate | 9600 |
| Data bits | 8 |
| Parity | None |
| Stop bits | 1 |
| Format | 8-N-1 |

## How It Works

### 1. Push-Button to UART Communication

When the onboard user button is pressed, the application transmits the following message to the PC:

```text
Button Pressed
```

The message can be observed using a serial terminal configured with the UART settings listed above.

### 2. UART-Based LED Control

The application receives commands from the PC through USART2 and controls the onboard blue LED.

| Command sent from PC | Expected behavior |
|---|---|
| `LED_ON` | Turns the onboard blue LED on |
| `LED_OFF` | Turns the onboard blue LED off |

Send the commands using the configured serial terminal. Commands must match the format expected by the firmware.

### 3. Interrupt Handling

USART2 reception uses interrupt-based handling so that incoming serial data can be processed without relying solely on continuous polling of the receive peripheral.

### 4. Timer Initialization

TIM6 is configured for timing operations within the application.

## How to Run

1. Open the project in STM32CubeIDE.
2. Build the firmware.
3. Connect the STM32F407G-DISCOVERY board to the PC.
4. Flash the firmware to the microcontroller.
5. Open a serial terminal connected to the configured USART2 interface.
6. Configure the terminal for **9600 baud, 8 data bits, no parity, and 1 stop bit**.
7. Press the onboard user button and verify that `Button Pressed` is received.
8. Send `LED_ON` and `LED_OFF` from the PC and verify the onboard blue LED behavior.

## Learning Objectives

This demonstration provides practical experience with:

- USART2 peripheral initialization and serial communication
- GPIO configuration and onboard LED control
- Push-button input handling
- Interrupt-driven UART reception
- Timer peripheral initialization
- Interaction between microcontroller firmware and PC-based serial tools

## Scope

This is a bare-metal embedded application intended for learning and demonstrating peripheral interaction on the STM32F407G-DISCOVERY board. The application uses a superloop-based execution model.

The implementation serves as a foundation for future enhancements, including reusable peripheral drivers, improved UART buffering, and real-time task-based execution using FreeRTOS.

## Platform

- **Microcontroller:** STM32F407
- **Development board:** STM32F407G-DISCOVERY
- **IDE:** STM32CubeIDE
- **Programming language:** Embedded C
