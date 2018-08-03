
#include <RTCZero.h>

/* Create an rtc object */
RTCZero rtc;

/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 00;
const byte hours = 17;

/* Change these values to set the current initial date */
const byte day = 17;
const byte month = 11;
const byte year = 15;

const int timeStep = 1;
int nextAlarmSecond = seconds + timeStep;
bool on = false;
void setup()
{
  //SerialUSB.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  rtc.begin();

  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);

  rtc.setAlarmSeconds(nextAlarmSecond);
  rtc.enableAlarm(rtc.MATCH_SS);

  rtc.attachInterrupt(alarmMatch);

  //rtc.standbyMode();
}

void loop()
{
  rtc.standbyMode();    // Sleep until next alarm match
  ///SerialUSB.println();
  //SerialUSB.println(rtc.getSeconds());
  //nextAlarmSecond = rtc.getSeconds() + timeStep;
  nextAlarmSecond = (nextAlarmSecond + timeStep) % 60;
  rtc.setAlarmSeconds(nextAlarmSecond);
  rtc.enableAlarm(rtc.MATCH_SS);
  //SerialUSB.println(nextAlarmSecond);
  //SerialUSB.println(millis());
  
}

void alarmMatch()
{
  if (on) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  on = !on;
}
