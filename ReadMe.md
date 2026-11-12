# STM32F407 – FreeRTOS GNSS-to-CAN Gateway

## 1. Overview

This branch implements a FreeRTOS-based GNSS-to-CAN communication application running on the STM32F407 microcontroller.

The application receives GNSS data over USART2, processes the incoming NMEA sentences using a custom GNSS parser, and forwards the extracted GNSS information through the CAN1 peripheral.

The project also introduces a migration from a traditional superloop-based architecture towards a FreeRTOS-based application structure.

### Key Features

- FreeRTOS integration on ARM Cortex-M4F.
- Custom register-level STM32 peripheral drivers.
- GNSS data reception through USART2.
- NMEA sentence parsing and GNSS data extraction.
- CAN1 peripheral initialization and message transmission.
- Modular application and driver architecture.
- Python-based GNSS data simulator for testing.

## 2. Hardware Platform

| Component | Description |
|---|---|
| Microcontroller | STM32F407 |
| CPU | ARM Cortex-M4F |
| Development board | STM32F407G-Discovery |
| RTOS | FreeRTOS |
| GNSS interface | USART2 |
| CAN interface | CAN1 |
| CAN transceiver | External CAN transceiver |
| Development IDE | STM32CubeIDE |
| Language | Embedded C |

### Peripheral Configuration

**USART2 – GNSS Reception**

USART2 is used to receive GNSS NMEA data from an external GNSS source or a Python-based GNSS simulator.

**CAN1 – GNSS Data Transmission**

CAN1 is used to transmit the processed GNSS information to an external CAN network through a CAN transceiver.

CAN1 GPIO configuration:
- PB8 – CAN1_RX
- PB9 – CAN1_TX

An external CAN transceiver is required to interface the STM32 CAN peripheral with the physical CAN bus.

## 3. System Architecture

```text
             GNSS DATA SOURCE
                    |
                    | NMEA Sentences
                    |
                    v
             +-------------+
             |   USART2    |
             | UART Driver |
             +-------------+
                    |
                    v
             +-------------+
             | GNSS Parser |
             |             |
             | NMEA Parsing|
             | Checksum    |
             | Validation  |
             +-------------+
                    |
                    v
             +-------------+
             | GNSS Data   |
             | Structure   |
             +-------------+
                    |
                    v
             +-------------+
             | CAN1 Driver |
             |             |
             | CAN Payload |
             | Preparation |
             | Transmission|
             +-------------+
                    |
                    v
             +-------------+
             | CAN         |
             | Transceiver |
             +-------------+
                    |
                    v
               CAN NETWORK
```

### GNSS Data Structure

The parser extracts GNSS information into the following data structure:

```c
typedef struct
{
    UTCTime_t utc_time;
    float latitude;
    float longitude;
    uint8_t fix_quality;
    uint8_t num_satellites;
    float altitude;
    uint8_t hdop;
    float geoidal_separation;

} GNSS_Data_t;
```

The extracted GNSS information is used by the application for CAN message preparation and transmission.

## 4. FreeRTOS Integration

This branch introduces FreeRTOS as the application execution environment.

### Integrated Components

- FreeRTOS kernel.
- ARM Cortex-M4F GCC port.
- FreeRTOS heap management using `heap_4.c`.
- FreeRTOS configuration.
- Application task module.

### FreeRTOS Directory Structure

```text
FreeRTOS/
│
├── include/
│
├── portable/
│   ├── GCC/
│   │   └── ARM_CM4F/
│   │       ├── port.c
│   │       └── portmacro.h
│   │
│   └── MemMang/
│       └── heap_4.c
│
├── event_groups.c
├── list.c
├── queue.c
├── stream_buffer.c
├── tasks.c
└── timers.c
```

### Application RTOS Files

```text
Inc/
└── FreeRTOSConfig.h
└── app_tasks.h

Src/
└── app_task.c
```

The application task module provides the separation between RTOS configuration and application execution logic.

## 5. Software Architecture

The firmware is organized into three primary software layers.

### Application Layer

Responsible for application-level functionality:

- Application initialization.
- GNSS processing.
- CAN data forwarding.
- Application task management.

Files:

```text
Src/
├── main.c
└── app_task.c

Inc/
└── app_tasks.h
```

### Driver Layer

Contains custom peripheral drivers implemented for the STM32F407.

| Driver | Responsibility |
|---|---|
| GPIO | GPIO configuration and control |
| USART | UART communication |
| GNSS | NMEA parsing and GNSS data extraction |
| CAN | CAN peripheral configuration and communication |
| Utils | Common utility functions |

Files:

```text
Inc/
├── stm32f407xx_gpio_driver.h
├── stm32f407xx_usart_driver.h
├── stm32f407xx_gnss_driver.h
├── stm32f407xx_can_driver.h
└── utils.h

Src/
├── stm32f407xx_gpio_driver.c
├── stm32f407_usart_driver.c
├── stm32f407xx_gnss_driver.c
├── stm32f407xx_can_driver.c
└── utils.c
```

### Hardware Abstraction

CMSIS device headers provide the STM32F407 peripheral register definitions and Cortex-M core access.

## 6. GNSS Simulation

A Python script is provided to simulate GNSS data reception without requiring a physical GNSS receiver.

Location:

```text
PyScripts/
└── fake_gnss.py
```

The script acts as a GNSS data source and sends simulated NMEA data to the STM32 through the serial interface.

This enables testing of the UART reception and GNSS parsing functionality.

## 7. Repository Structure

```text
STM32F407_Dev/
│
├── CMSIS/
│   └── Core/
│       ├── Include/
│       └── Template/
│
├── FreeRTOS/
│   ├── include/
│   ├── portable/
│   ├── event_groups.c
│   ├── list.c
│   ├── queue.c
│   ├── stream_buffer.c
│   ├── tasks.c
│   └── timers.c
│
├── Inc/
│   ├── FreeRTOSConfig.h
│   ├── app_tasks.h
│   ├── stm32f407xx.h
│   ├── stm32f407xx_can_driver.h
│   ├── stm32f407xx_gnss_driver.h
│   ├── stm32f407xx_gpio_driver.h
│   ├── stm32f407xx_usart_driver.h
│   └── utils.h
│
├── Src/
│   ├── main.c
│   ├── app_task.c
│   ├── stm32f407_usart_driver.c
│   ├── stm32f407xx_can_driver.c
│   ├── stm32f407xx_gnss_driver.c
│   ├── stm32f407xx_gpio_driver.c
│   ├── system_stm32f4xx.c
│   └── utils.c
│
├── PyScripts/
│   └── fake_gnss.py
│
├── .cproject
└── .project
```

## 8. Development Environment

### Requirements

- STM32CubeIDE
- ARM GCC toolchain
- STM32F407 development board
- ST-LINK debugger
- USB-to-UART interface or GNSS serial source
- External CAN transceiver
- CAN analyzer or CAN interface
- Python 3

### Clone the Repository

```bash
git clone https://github.com/vathsa17/STM32F407_Dev.git

cd STM32F407_Dev
```

### Checkout This Branch

```bash
git checkout feature/freertos-migration
```

### Build and Flash

1. Open STM32CubeIDE.
2. Import the project as an existing STM32CubeIDE project.
3. Select the appropriate build configuration.
4. Build the firmware.
5. Connect the STM32F407 board through ST-LINK.
6. Flash and debug the application.

## 9. Current Development Scope

The main objective of this branch is to establish a FreeRTOS-based GNSS-to-CAN gateway.

| Feature | Implementation |
|---|---|
| STM32F407 platform | Integrated |
| CMSIS device support | Integrated |
| FreeRTOS kernel | Integrated |
| FreeRTOS ARM Cortex-M4F port | Integrated |
| GPIO driver | Integrated |
| USART2 driver | Integrated |
| GNSS NMEA parser | Integrated |
| CAN1 driver | Integrated |
| Python GNSS simulator | Added |
| GNSS-to-CAN forwarding | Application functionality |
| Advanced RTOS synchronization | Future development |

## 10. Future Improvements

- Introduce queue-based communication between GNSS processing and CAN transmission.
- Improve non-blocking UART reception.
- Implement robust UART ring-buffer handling.
- Introduce dedicated communication and application tasks.
- Add CAN reception and message filtering.
- Improve GNSS validity and error handling.
- Add communication timeout monitoring.
- Integrate watchdog supervision.
- Implement unit tests for GNSS parsing and CAN payload encoding.

---

**Branch:** `feature/freertos-migration`

**Platform:** STM32F407 / ARM Cortex-M4F

**Primary functionality:** GNSS UART2 reception and CAN1 forwarding using FreeRTOS.