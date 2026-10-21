# STM32F407G-DISCOVERY — GNSS NMEA Parser and UART Forwarding

**Version:** v1.1.0  
**Platform:** STM32F407G-DISCOVERY  
**Language:** Embedded C  
**Architecture:** Bare-metal Superloop  
**Status:** GNSS reception and UART forwarding implemented; CAN integration in progress.

## Overview

This project demonstrates GNSS data acquisition, NMEA sentence parsing, interrupt-driven UART reception and serial data forwarding using the STM32F407G-DISCOVERY development board.

A GNSS receiver transmits NMEA sentences to the STM32 over UART2. The microcontroller stores incoming bytes in a ring buffer, processes the data byte by byte, validates and parses NMEA sentences, and extracts relevant information such as latitude, longitude, altitude, fix status and the number of satellites.

The parsed information is transmitted to a PC or another device through UART3. The on-board blue LED provides visual feedback by turning on briefly when a valid NMEA sentence has been received and parsed successfully.

## System Architecture

![STM32F407 GNSS NMEA Parser and UART Forwarding System](./docs/images/gnss-uart-forwarding.png)

### Data Flow

```text
GNSS Receiver
     |
     | NMEA sentences @ 9600 baud
     v
   UART2
     |
     v
UART2 Interrupt Handler
     |
     v
   Ring Buffer
     |
     v
  NMEA Parser
     |
     v
GNSS Data Structure
     |
     +----> Blue LED Status Indication
     |
     +----> UART3 Transmission @ 9600 baud
                       |
                       v
                  PC / Terminal
```

## Features

- **UART communication:** UART2 receives GNSS data, while UART3 forwards parsed information to the PC.
- **Interrupt-driven reception:** Incoming UART data is handled through an interrupt service routine.
- **Ring buffer:** Circular buffering decouples byte reception from sentence processing.
- **NMEA parsing:** Processes incoming data byte by byte, validates sentences and extracts relevant GNSS information.
- **Status indication:** The blue LED turns on briefly following successful sentence parsing.
- **Timer-based operations:** TIM6 and TIM7 are used for periodic timing operations in the application.
- **Bare-metal architecture:** The application currently uses a superloop without an RTOS.

## UART Configuration

| Parameter | UART2 — GNSS Input | UART3 — PC Output |
|---|---|---|
| Baud rate | 9600 bps | 9600 bps |
| Data bits | 8 | 8 |
| Parity | None | None |
| Stop bits | 1 | 1 |
| Reception | Interrupt-driven | — |
| Purpose | Receive NMEA sentences | Transmit parsed GNSS data |

## Example Output

The following is an example of the data transmitted to the PC through UART3:

```text
Lat: 48.715504
Lon: 11.504339
Alt: 374.500000 m
Fix: 1
Sat: 9
```

The output represents the parsed GNSS latitude, longitude, altitude, fix status and satellite count.

*Note: These values are an example captured from the implementation, not guaranteed live readings.*

## Implementation Details

### 1. UART Reception and Interrupt Handling

UART2 is configured to receive data from the GNSS receiver. The interrupt handler performs minimal processing and stores incoming bytes in the ring buffer, allowing the application to process the data separately.

### 2. Ring Buffer

The ring buffer provides a circular storage mechanism for incoming UART data. It decouples asynchronous byte reception from NMEA parsing and helps prevent data loss when processing takes longer than receiving individual bytes.

Correct buffer sizing, overflow detection and synchronization between the interrupt handler and application code are important for reliable operation.

### 3. NMEA Parser

The parser processes incoming bytes incrementally, identifies sentence boundaries, validates supported NMEA sentences and extracts relevant fields.

The extracted data is stored in a GNSS data structure for use by the application and communication interfaces.

### 4. LED Status Indication

The on-board blue LED provides visual feedback when a valid NMEA sentence has been parsed successfully.

### 5. Timer-Based Operations

TIM6 and TIM7 are used for periodic timing operations, including scheduled communication activities in the application. These mechanisms provide a foundation for extending the system with additional communication and processing tasks.

## What I Learned

### UART Communication

- Configuring UART peripherals and their associated GPIO alternate functions.
- Setting baud rates and serial communication parameters.
- Implementing interrupt-driven reception on an STM32 microcontroller.
- Separating interrupt handling from application-level processing.

### Ring Buffer Implementation

- Managing continuous incoming data using circular buffers.
- Decoupling data acquisition from processing.
- Understanding buffer overflow, producer-consumer behavior and shared data between interrupt and application contexts.

### NMEA Sentence Parsing

- Processing structured serial data incrementally.
- Identifying sentence boundaries and validating incoming data.
- Extracting latitude, longitude, altitude, fix status and satellite information.
- Handling communication data independently of the transport layer.

### Timers and Application Scheduling

- Configuring STM32 timers for periodic operations.
- Separating timing requirements from data acquisition and parsing.
- Understanding the limitations of scheduling activities using a bare-metal superloop.

## Current Architecture and Future Work

The firmware currently runs in **bare-metal Superloop mode**. FreeRTOS has not yet been integrated.

Planned enhancements include:

- Complete and validate CAN driver functionality.
- Transmit selected GNSS information over the CAN bus.
- Improve ring-buffer overflow handling and communication error recovery.
- Add unit tests for the NMEA parser and other testable software modules.
- Introduce FreeRTOS tasks, queues and synchronization primitives.
- Integrate an additional physical sensor through I²C or SPI.
- Add structured logging, fault handling and watchdog supervision.

## Project Goal

This implementation is part of a broader STM32 Real-Time Sensor and Communication Platform intended to explore embedded C, peripheral driver development, interrupt handling, communication protocols, real-time software architecture and systematic verification on real hardware.
