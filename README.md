Automatic Dog Feeder
Description

Developed an automated dog feeder based on an Arduino Nano, an SG90 servo motor, and a DS3231 real-time clock (RTC). The system automatically releases a predefined portion of food at scheduled times without requiring user interaction.

Main Features
DS3231 RTC for accurate timekeeping and scheduled feeding
SG90 servo motor controlling the food-release mechanism
Configurable feeding schedule, currently 07:00 and 14:00
Watchdog Timer interrupt used to periodically wake the microcontroller
Power-down sleep mode between checks to reduce energy consumption
Servo is detached when not in use to reduce unnecessary power consumption
Designed for battery/power-bank operation and extended autonomy
Manual feeding functionality used during development and testing
Technical Implementation

The Arduino periodically enters SLEEP_MODE_PWR_DOWN, significantly reducing microcontroller activity between feeding checks. The ATmega328P Watchdog Timer is configured in interrupt mode and periodically wakes the microcontroller, allowing it to read the DS3231 and determine whether a feeding event is due.

The feeding mechanism is activated by rotating the SG90 servo to open the food-release flap for a predefined period, after which the servo returns to its initial position and is detached until the next feeding cycle.

Hardware
Arduino Nano (ATmega328P + CH340)
DS3231 Real-Time Clock
SG90 servo motor
Wooden food-release mechanism
Portable 5V power source / power bank
