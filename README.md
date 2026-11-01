# STM32 Blackpill: MD49 Dual Motor Driver Firmware & Telemetry

High-performance firmware developed for **STM32 (Blackpill)** to interface with a dual-motor controller **MD49** via UART (38400 baud). It handles asynchronous speed commands over USB-CDC, queries motor velocities and states, and streams robot odometry and power telemetry in real-time.

---

## Key Features

* **Bidirectional Motor Control:** Receives velocity setpoints via USB-CDC strings, parses variables, and updates motor speeds dynamically (`MD49_SetSpeedMotors`).
* **Telemetry Streaming:** Periodically fetches and transmits encoder counts, battery voltage, motor currents, and error states.
* **USB-CDC Integration:** Formats and outputs structured data strings over the native virtual COM port.
* **Non-Blocking Visual Indicators:** Heartbeat status LED toggled safely using `HAL_GetTick()`.

---

## Hardware Stack & Connections

* **Microcontroller:** STM32 Blackpill (`STM32F401` / `STM32F411`)
* **Motor Controller:** MD49 Dual Motor Driver (UART communication at `38400` baud)
* **Connectivity:** Native USB (Virtual COM Port / CDC) connected to a PC or Single Board Computer (e.g., for ROS2 integration).

### Pinout Configuration:
* **USART2 TX / RX:** Connected to the MD49 UART communication lines (Baud rate: `38400`).
* **USB Pins (PA11 / PA12):** Native USB interface for CDC transmission.

---

## Data Frame Format (USB-CDC)

The microcontroller outputs a formatted string over the virtual serial port containing encoder and power telemetry:

```text
E,enc1,enc2;V,volt;I,i1,i2;S,err,mod,ac\r\n
