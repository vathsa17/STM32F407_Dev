# STM32F407_Dev

A hands-on learning and development repository for the **STM32F407G** (Cortex-M4F, 168 MHz) development board. Each module is built to understand the peripheral underneath it, then reused in larger applications.

## Hardware

| Item | Details |
|------|---------|
| MCU | STM32F407VGT6 (Cortex-M4F, 1 MB Flash, 192 KB RAM) |
| Board | STM32F407G dev board |
| Debug probe | ST-LINK (on-board or external) |
| Sensors / modules | IR motion sensor (e.g. PIR/IR), CAN transceiver (e.g. SN65HVD230 / TJA1050) |

## Project Status

| Module | Status | Notes |
|--------|--------|-------|
| GPIO (input/output) | ✅ Working | LEDs, push button |
| UART | ✅ Working | Serial debug / logging |
| External interrupts (ISR) | ✅ Working | EXTI configuration and handlers |
| IR motion sensor | ✅ Working | Interrupt-driven detection |
| CAN | 🚧 In progress | Bring-up and loopback testing |

## Repository Structure

```
STM32F407_Dev/
├── Inc/    # Header files (peripheral drivers, pin definitions, config)
├── Src/    # Source files (main application, drivers, ISR handlers)
└── README.md
```



## Pin Mapping

| Function | Pin | Peripheral | Notes |
|----------|-----|-----------|-------|
| UART TX | PA2 | USART2 | |
| UART RX | PA3 | USART2 | |
| IR sensor OUT | PXn | EXTIn | Rising-edge interrupt |
| LED | PXn | GPIO out | |
| CAN RX | PB8 / PA11 | CAN1 | |
| CAN TX | PB9 / PA12 | CAN1 | |

## Getting Started

### Prerequisites
- Toolchain: STM32CubeIDE / `arm-none-eabi-gcc` 
- STM32CubeF4 HAL/CMSIS (or register-level, if used) 
- ST-LINK drivers, OpenOCD or STM32CubeProgrammer
- Serial terminal (PuTTY, minicom, etc.) at **9600 8N1** 


## External Hardwares
### USB to TTL RS232 Adapter
A Secondary USB to TTL RS232 Adapter is necessary if you are trying to establish connection between your PC and USART/UART communication driver from STM32F407G Board 

### CAN Rx/Tx Transiver and USB-CAN Adapter
The bxCAN peripheral on board can only facilitate upto DataLinkLayer. You would need a CAN transceiver to drive the CANH and CANL bus. Aditionally, a secondary USB CAN Interface which can act as second Node.

### Running
1. Connect the board over USB (ST-LINK).
2. Wire the peripherals per the pin mapping above.
3. Open the serial terminal and reset the board.

## Module Notes

### GPIO
Output and input configuration, with pull-up/pull-down and speed settings.

### UART
Used for debug output. Describe whether polling, interrupt or DMA is used.

### Interrupts (EXTI/NVIC)
EXTI line mapping, NVIC priority configuration and keeping ISRs short (flag set, work in main loop).

### IR Motion Sensor
Sensor output triggers an EXTI interrupt; the handler sets a flag processed in the main loop. Include any debounce or hold-time behavior.

### CAN (in progress)
Planned scope:
- [ ] Bit timing configuration (target bitrate: e.g. 500 kbit/s)
- [ ] Loopback / silent mode self-test
- [ ] Transmit and receive with filters
- [ ] Two-node test on a real bus (120 Ω termination at both ends)
- [ ] Error handling (bus-off recovery, error counters)

## Roadmap
- [ ] Finish CAN bring-up
- [ ] Timers and PWM
- [ ] ADC / DMA
- [ ] SPI / I2C
- [ ] RTOS exercise (FreeRTOS)
- [ ] Higher-level CAN protocols (e.g. UDS over CAN)

## References
- [STM32F407 Reference Manual (RM0090)](https://www.st.com/resource/en/reference_manual/dm00031020.pdf)
- [STM32F407 Datasheet](https://www.st.com/resource/en/datasheet/stm32f407vg.pdf)
- [Cortex-M4 Generic User Guide](https://developer.arm.com/documentation/dui0553/latest/)



## About

I'm Shrivathsa, a senior embedded software engineer with about 9 years of experience in automotive systems (AUTOSAR Classic, SOME/IP, UDS/DoIP diagnostics, embedded Linux). This repo is where I work with STM32 bare-metal peripherals hands-on, from GPIO and UART to interrupts and CAN, to build intuition for what sits underneath the stacks I use professionally.

- LinkedIn: https://www.linkedin.com/in/shrivathsa-udupa/
