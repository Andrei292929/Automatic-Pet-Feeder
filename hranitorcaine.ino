#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>

RTC_DS3231 rtc;
Servo servo;

const int FEED_HOUR_1 = 7;
const int FEED_HOUR_2 = 14;

int lastFeedHour = -1;
int lastFeedMinute = -1;

void setup() {

  Serial.begin(9600);

  servo.attach(9);
  servo.write(0);

  if (!rtc.begin()) {
    while (1);
  }

  // ===== set ora =====
  rtc.adjust(DateTime(2026, 9, 3, 13, 59, 45));
  // =====         =====

  Serial.println("Scrie feed pentru hranire manuala.");
}

void loop() {

  // ===== debug =====
  if (Serial.available() > 0) {

    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "feed") {

      servo.write(180);
      delay(7000);
      servo.write(0);
    }
  }
  // ===== debug pana aici =====

  DateTime now = rtc.now();

  if (
    (now.hour() == FEED_HOUR_1 || now.hour() == FEED_HOUR_2) &&
    now.minute() == 0
  ) {

    if (
      now.hour() != lastFeedHour ||
      now.minute() != lastFeedMinute
    ) {

      servo.write(180);
      delay(7000);
      servo.write(0);

      lastFeedHour = now.hour();
      lastFeedMinute = now.minute();
    }
  }

  delay(1000);
}