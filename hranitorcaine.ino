#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

RTC_DS3231 rtc;
Servo servo;

const int SERVO_PIN = 9;


// ===== ORE DE TEST =====

const int FEED_HOUR_1 = 14;
const int FEED_MINUTE_1 = 0;

const int FEED_HOUR_2 = 14;
const int FEED_MINUTE_2 = 5;

// ========================


int lastFeedHour = -1;
int lastFeedMinute = -1;


// ===== WATCHDOG INTERRUPT =====

volatile bool watchdogTriggered = false;

ISR(WDT_vect) {
  watchdogTriggered = true;
}

// ===============================


void feed() {

  servo.attach(SERVO_PIN);

  servo.write(20);
  delay(7000);

  servo.write(0);
  delay(500);

  servo.detach();
}


void setup() {

  Serial.begin(9600);

  servo.attach(SERVO_PIN);
  servo.write(0);
  delay(500);
  servo.detach();


  // ===== INITIALIZARE RTC =====

  if (!rtc.begin()) {
    Serial.println("RTC NU A FOST GASIT!");
    while (1);
  }

  // ============================


  // ===== SETARE INITIALA A OREI =====
  // 13:59:45 -> primul feed la 14:00:00

  rtc.adjust(DateTime(2026, 9, 4, 13, 59, 55));

  // ==================================


  // ===== CONFIGURARE WATCHDOG =====

  MCUSR &= ~(1 << WDRF);

  WDTCSR |= (1 << WDCE) | (1 << WDE);

  WDTCSR = (1 << WDIE) |
           (1 << WDP3) |
           (1 << WDP0);

  // =================================


  // ===== CONFIGURARE SLEEP =====

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  // ==============================


  Serial.println("RTC pornit.");
  Serial.println("Test feed: 14:00 si 14:05.");
  Serial.println("Watchdog + POWER_DOWN activ.");
}


void loop() {

  DateTime now = rtc.now();


  // ===== AFISARE ORA =====

  Serial.print(now.hour());
  Serial.print(":");

  if (now.minute() < 10)
    Serial.print("0");

  Serial.print(now.minute());
  Serial.print(":");

  if (now.second() < 10)
    Serial.print("0");

  Serial.println(now.second());

  // ========================


  // ===== FEED 1 =====

  if (
    now.hour() == FEED_HOUR_1 &&
    now.minute() == FEED_MINUTE_1
  ) {

    if (
      now.hour() != lastFeedHour ||
      now.minute() != lastFeedMinute
    ) {

      Serial.println(">>> FEED 14:00 <<<");

      feed();

      lastFeedHour = now.hour();
      lastFeedMinute = now.minute();
    }
  }


  // ===== FEED 2 =====

  if (
    now.hour() == FEED_HOUR_2 &&
    now.minute() == FEED_MINUTE_2
  ) {

    if (
      now.hour() != lastFeedHour ||
      now.minute() != lastFeedMinute
    ) {

      Serial.println(">>> FEED 14:05 <<<");

      feed();

      lastFeedHour = now.hour();
      lastFeedMinute = now.minute();
    }
  }


  // ===== WATCHDOG =====

  if (watchdogTriggered) {

    watchdogTriggered = false;

    Serial.println("Watchdog interrupt!");
  }

  // =================


  // ===== TERMINA TRANSMISIA SERIAL =====

  Serial.flush();

  // ======================================


  // ===== SLEEP =====

  noInterrupts();

  sleep_enable();

  interrupts();

  sleep_cpu();

  sleep_disable();

  // =================
}