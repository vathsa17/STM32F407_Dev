# STM32F407 Embedded Systems Development

A hands-on embedded software development project based on the **STM32F407 microcontroller**, focusing on register-level peripheral drivers, embedded C, real-time operating systems, communication protocols, and modular firmware architecture.

The project is developed incrementally, with the goal of building a reusable embedded software platform that demonstrates practical microcontroller programming and real-time embedded software engineering.

## 1. Project Overview

**Target platform:** STM32F407  
**Development board:** STM32F407G-Discovery  
**Programming language:** Embedded C  
**IDE:** STM32CubeIDE  
**Architecture:** ARM Cortex-M4 with FPU  
**RTOS:** FreeRTOS  
**Version control:** Git / GitHub

### Main objectives

- Understand ARM Cortex-M4 architecture and STM32 peripherals.
- Develop reusable peripheral drivers using register-level programming.
- Implement UART, GPIO, CAN and GNSS communication functionality.
- Integrate FreeRTOS for multitasking and real-time execution.
- Develop modular, maintainable and testable embedded software.
- Build practical experience in debugging and firmware development.

## 2. Repository Branches

| Branch | Purpose |
|---|---|
| `master` | Stable baseline |
| `develop` | Development integration |
| `feature/uart-demo` | UART demonstration |
| `feature/gnss-uart-forwarding` | GNSS UART parsing and forwarding |
| `feature/freertos-migration` | FreeRTOS integration and driver migration |

The current development focus is `feature/freertos-migration`.

## 3. Hardware and Software Architecture

### Hardware

- STM32F407 microcontroller
- ARM Cortex-M4 core with floating-point unit
- Onboard GPIO LEDs and push button
- UART interface for PC communication
- CAN transceiver for CAN bus communication
- External GNSS data source

### Software architecture

The firmware follows a modular, layered architecture.

```text
+--------------------------------------+
|           Application Layer          |
|                                      |
|  Main Application / Application Tasks|
|  GNSS Processing / State Management  |
+--------------------------------------+
                   |
+--------------------------------------+
|          Middleware Layer            |
|                                      |
|              FreeRTOS                |
|       Tasks / Queues / Scheduling    |
+--------------------------------------+
                   |
+--------------------------------------+
|           Driver Layer               |
|                                      |
| GPIO | USART | CAN | GNSS | Utils    |
+--------------------------------------+
                   |
+--------------------------------------+
|       STM32F407 Hardware             |
|                                      |
| GPIO | USART | CAN | Timers | RCC    |
+--------------------------------------+
                   |
+--------------------------------------+
|          Physical Interfaces         |
|                                      |
|       UART | CAN | GPIO Signals      |
+--------------------------------------+
```

## 4. Implemented Functionality

### 4.1 GPIO Driver

A custom GPIO driver provides access to STM32 GPIO peripherals.

Implemented functionality includes:

- GPIO initialization.
- Input and output configuration.
- Output type configuration.
- Pull-up and pull-down configuration.
- GPIO output control.
- GPIO input reading.
- Alternate-function configuration.

### 4.2 UART Communication

USART2 is used for bidirectional communication between the STM32 and a PC.

Configuration:
- Peripheral: USART2
- Baud rate: 9600
- Frame format: 8-N-1

Implemented functionality:

- UART initialization.
- UART transmission.
- UART reception.
- Interrupt-based receive handling.
- Communication between the STM32 and a PC.

Demonstration:

- Pressing the onboard button sends `Button Pressed` to the PC.
- Sending `LED_ON` from the PC switches on the onboard blue LED.
- Sending `LED_OFF` switches off the onboard blue LED.

### 4.3 GNSS / NMEA Parser

A custom GNSS driver processes NMEA data received through UART.

The parser maintains its internal state and extracts GNSS information into structured data.

Supported data fields include:

- UTC time
- Latitude
- Longitude
- Fix quality
- Number of satellites
- Altitude
- HDOP
- Geoidal separation

The parser uses a structured data model:

```c
typedef struct
{
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint16_t milliseconds;

} UTCTime_t;

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

A Python-based GNSS simulator (`PyScripts/fake_gnss.py`) is also included for development and testing without requiring a physical GNSS receiver.

### 4.4 CAN Communication

A custom CAN driver has been added for STM32F407 CAN peripheral communication.

Implemented development activities include:

- CAN peripheral configuration.
- CAN GPIO alternate-function configuration.
- CAN message transmission.
- CAN communication testing using an external CAN transceiver.

**Hardware interface:** CAN1 using PB8 and PB9 with an external CAN transceiver.

The GNSS data structure provides latitude and longitude information for integration with CAN messaging.

### 4.5 FreeRTOS Integration

The project is being migrated from a superloop-based execution model towards a real-time multitasking architecture.

The FreeRTOS kernel is included in the repository.

Integrated components:

- FreeRTOS kernel.
- ARM Cortex-M4F GCC port.
- FreeRTOS heap implementation (`heap_4.c`).
- FreeRTOS configuration.
- Application task module.

Relevant files:

```text
FreeRTOS/
├── event_groups.c
├── list.c
├── queue.c
├── stream_buffer.c
├── tasks.c
├── timers.c
├── include/
├── portable/
│   ├── GCC/
│   │   └── ARM_CM4F/
│   └── MemMang/
│       └── heap_4.c
```

Application-specific configuration and task definitions:

```text
Inc/
├── FreeRTOSConfig.h
└── app_tasks.h

Src/
└── app_task.c
```

The migration establishes the foundation for separating peripheral processing and application functionality into independently scheduled tasks.

## 5. Project Structure

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
│   ├── stm32f4xx.h
│   ├── system_stm32f4xx.h
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
├── .project
└── README.md
```

## 6. Development Environment

### Required tools

- STM32CubeIDE
- ARM GCC toolchain
- Git
- Python 3
- Serial terminal application
- CAN interface and transceiver for CAN testing

### Building the firmware

1. Clone the repository.
2. Checkout the required development branch.
3. Import the project into STM32CubeIDE.
4. Verify the compiler and linker configuration.
5. Build the project.
6. Connect the STM32F407 development board.
7. Flash the firmware using the ST-LINK debugger.

Example:

```bash
git clone https://github.com/vathsa17/STM32F407_Dev.git

cd STM32F407_Dev

git checkout feature/freertos-migration
```

Build and flash using STM32CubeIDE.

## 7. Testing and Debugging

Testing is performed incrementally using hardware and software tools.

| Component | Testing approach |
|---|---|
| GPIO | LED and push-button testing |
| UART | PC-to-STM32 bidirectional communication |
| GNSS | Simulated NMEA input |
| CAN | CAN transmission with external transceiver |
| FreeRTOS | Application task execution and debugging |

Python scripts are used to simulate external communication sources where appropriate.

## 8. Development Roadmap

The following capabilities are planned for further development.

### Peripheral Drivers

- [ ] Timer driver and PWM generation
- [ ] I2C driver
- [ ] SPI driver
- [ ] ADC driver
- [ ] DMA-based peripheral communication
- [ ] Non-blocking UART driver

### Real-Time Systems

- [ ] Multiple application tasks
- [ ] Task synchronization
- [ ] Queue-based inter-task communication
- [ ] Mutexes and semaphores
- [ ] Interrupt-to-task communication
- [ ] Task priority and scheduling analysis
- [ ] Stack monitoring
- [ ] Watchdog integration

### Communication

- [ ] GNSS-to-CAN message integration
- [ ] CAN reception and message filtering
- [ ] CAN message decoding
- [ ] Communication error handling
- [ ] UART ring-buffer implementation and testing

### Software Architecture and Quality

- [ ] State-machine-based application design
- [ ] Centralized error handling
- [ ] Logging framework
- [ ] Unit testing
- [ ] Hardware integration testing
- [ ] Static code analysis
- [ ] Firmware documentation

## 9. Long-Term Project Goal

The long-term objective is to evolve this repository into a reusable **STM32 Real-Time Sensor and Communication Platform**.

The final system will combine:

- STM32F407 peripheral drivers
- Sensor data acquisition
- GNSS processing
- CAN and UART communication
- FreeRTOS multitasking
- Real-time scheduling and synchronization
- Error handling and watchdog supervision
- Modular and reusable embedded software components

The project is intended to demonstrate practical embedded software engineering skills, from register-level hardware access to real-time application architecture.

---

**Repository:** https://github.com/vathsa17/STM32F407_Dev

**Current development branch:** `feature/freertos-migration`