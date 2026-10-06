# STM32 Real-Time Sensor and Communication Platform

A hands-on embedded software project based on the STM32F407 microcontroller, focused on developing practical expertise in Embedded C, ARM Cortex-M architecture, peripheral drivers, real-time systems, communication, debugging, and embedded software architecture.

The project is being developed incrementally, starting from low-level peripheral control and progressing toward a modular, RTOS-based embedded system.

## Project Goals

The primary goal of this project is to build a complete embedded software platform on the STM32F407 while developing a deeper understanding of how embedded software interacts with microcontroller hardware.

The project focuses on:

- Embedded C and low-level programming
- ARM Cortex-M4 architecture
- STM32F407 peripherals
- Memory-mapped I/O and peripheral registers
- GPIO and interrupt handling
- UART, I2C and SPI communication
- Timers and PWM
- DMA
- Peripheral driver development
- FreeRTOS and real-time systems
- Inter-task communication and synchronization
- State machines
- Error handling and diagnostics
- Watchdog implementation
- Embedded software architecture
- Hardware debugging
- Unit and integration testing
- Git-based software development and documentation

The long-term objective is to develop software using engineering practices representative of professional embedded systems.

## Current Status

The project currently includes:

- STM32F407 development environment
- GPIO configuration
- LED control
- Push button GPIO input
- UART initialization
- UART transmission from STM32 to PC
- UART reception from PC to STM32
- Bidirectional PC-to-STM32 UART communication
- Interrupt Control for Push Button and UART Reception

The current implementation provides the foundation for progressively developing reusable drivers and a larger embedded software architecture.

## Current Hardware

### Microcontroller

- STM32F407
- ARM Cortex-M4
- STM32 development board


### Currently Used Peripherals

- GPIO
- Push button
- LED
- USART/UART
- USB-TTL RS232 

### PC Communication

UART is currently used as the communication interface between the STM32 and a PC.

The current communication path is:

```text
PC
 |
 | UART (via USB-TTL RS232)
 |
STM32F407
 |
 +-- GPIO
     |
     +-- Push Button
     |
     +-- LED

## Coming Soon

- CAN Drivers
- CAN Tx/Rx using CAN Transciever and USB-CAN Adapter (with a Scapy script or Python-CAN Script)
- Integrate GNSS Reciever 

## About Me

I am a Senior Embedded and Automotive Software Engineer with 9+ years of experience in automotive software development, ECU integration, validation, embedded communication, and real-time systems.

My professional experience includes Embedded C/C++, AUTOSAR, RTOS-based systems, SOME/IP, DoIP, UDS, CAN, V2X, and embedded Linux.

This project is focused on strengthening my hands-on embedded software development skills, particularly at the microcontroller and driver level. It provides a practical environment to work with ARM Cortex-M, STM32 peripherals, Embedded C, real-time systems, hardware interfaces, debugging, and embedded software architecture.

The long-term goal is to combine my existing automotive system and integration experience with deeper low-level embedded software expertise.
