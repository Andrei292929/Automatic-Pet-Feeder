# Automatic Dog Feeder

## Description

Developed an automated dog feeder based on an Arduino Nano, an SG90 servo motor, and a DS3231 Real-Time Clock (RTC). The system automatically releases food at predefined times without requiring user interaction.

---

## Features

- Accurate timekeeping using the DS3231 RTC
- Configurable feeding schedule
- Automatic food dispensing using an SG90 servo motor
- Watchdog Timer interrupt for periodic wake-ups
- Power-down sleep mode for reduced power consumption
- Servo detached when not in use to reduce unnecessary power consumption
- Designed for battery and power-bank operation
- Manual feeding functionality for development and testing

---

## System Overview

The Arduino Nano controls the feeding mechanism and communicates with the DS3231 RTC through I2C.

The ATmega328P periodically enters `SLEEP_MODE_PWR_DOWN`. The Watchdog Timer generates an interrupt approximately every 8 seconds, waking the microcontroller so it can check the current time and determine whether a feeding event is required.

When a feeding event occurs, the SG90 servo rotates the mechanical flap to release the food, waits for a predefined period, returns to its initial position, and is then detached until the next feeding cycle.

```text
                    DS3231 RTC
                        |
                       I2C
                        |
                        v
                 Arduino Nano
                   ATmega328P
                        |
             +----------+----------+
             |                     |
             v                     v
       Watchdog Timer          SG90 Servo
             |                     |
             v                     v
       Wake from Sleep        Food Release
             |
             v
      POWER-DOWN MODE
```

---

## Feeding Schedule

The current feeding schedule is:

| Feeding | Time |
|---------|------|
| Morning | 07:00 |
| Afternoon | 14:00 |

The schedule can be modified directly in the Arduino code.

---

## Power Management

To maximize battery autonomy, the microcontroller spends most of its time in Power-down sleep mode.

The Watchdog Timer periodically wakes the ATmega328P through an interrupt. After waking, the system checks the DS3231 RTC and performs a feeding operation if the current time matches a scheduled feeding period.

After completing the check, the microcontroller returns to Power-down mode.

```text
Active
  |
  v
Read RTC
  |
  v
Check Feeding Schedule
  |
  +---- Feeding Required ----> Activate Servo
  |                              |
  |                              v
  |                         Release Food
  |                              |
  |                              v
  |                         Detach Servo
  |
  v
Power-down Sleep
  |
  v
Watchdog Interrupt
  |
  +----------------------------> Active
```

---

## Hardware

- Arduino Nano (ATmega328P + CH340)
- DS3231 Real-Time Clock
- SG90 Micro Servo
- Wooden food-release mechanism
- 5V power source / power bank

---

## Software

- Arduino C/C++
- Arduino Servo Library
- RTClib
- AVR Watchdog Timer
- AVR Interrupts
- AVR Power Management
- I2C communication

---

## Future Improvements

- Persistent feeding state using EEPROM
- External RTC battery for maintaining time during power loss
- Further optimization of power consumption
- Improved mechanical food-dispensing mechanism
- Automatic battery/power-source switching
