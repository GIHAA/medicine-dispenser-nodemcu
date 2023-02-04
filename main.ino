#include <ESP8266WiFi.h> // Library for WiFi
#include <time.h> // Library for time functions
#include <Servo.h>

#define IN1 D2
#define IN2 D3
#define EN D4
#define touchPin D5
#define buzzer D6
#define led D7

Servo myservo;

const char* ssid = "ssid";
const char* password = "pass";

time_t now;
struct tm timeinfo;

int alarmHour = 99; // Alarm hour (24-hour format)
int alarmMinute = 99; // Alarm minute

int hour, minute, second;
int touchState = 0;

int motor_offset = 350;
int time_offset = 19800;

void setup() {

  myservo.attach(D1); 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(D9, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(touchPin, INPUT_PULLUP);


  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Connect to NTP server and set the time
configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  while (!getLocalTime(&timeinfo)) {
    delay(1000);
  }
  Serial.begin(9600);

}

void loop() {

 if (Serial.available() > 0) { 
    String incomingString = Serial.readString(); 
    //Serial.println(incomingString);
    int colonIndex = incomingString.indexOf(":");
    String hourString = incomingString.substring(0, colonIndex);
    String minuteString = incomingString.substring(colonIndex + 1);
    alarmHour = hourString.toInt();
    alarmMinute = minuteString.toInt();
    //Serial.println(alarmHour);
    //Serial.println(alarmMinute);
  }

  myservo.write(20);
  // Get the current time
  now = time(nullptr);
  localtime_r(&now, &timeinfo);

  hour = timeinfo.tm_hour;
  minute = timeinfo.tm_min;
  second = timeinfo.tm_sec;

  add_offset(hour, minute, second, time_offset);

  // Serial.print(hour);
  // Serial.print(" :: ");
  // Serial.print(minute);
  // Serial.print(" :: ");
  // Serial.print(second);
  // Serial.print("\n\n\n");
  // Serial.print("alarmHour : " );
  // Serial.print(alarmHour); 
  // Serial.print("alarmMinute : ");
  // Serial.print(alarmMinute);
  // Serial.print("\n\n\n");

  // Check if the current time matches the alarm time
  if (hour == alarmHour && minute == alarmMinute && second == 0) {
    //Serial.println("alaram on");
    alarm_triggred();
  } else {
    digitalWrite(led, LOW); 
    //Serial.println("alaram Off");
  }

}

void add_offset(int &hour, int &minute, int &second, int offset_in_seconds) {
  second += offset_in_seconds;
  while (second >= 60) {
    second -= 60;
    minute += 1;
  }
  while (minute >= 60) {
    minute -= 60;
    hour += 1;
  }
  while (hour >= 24) {
    hour -= 24;
  }
}

void alarm_triggred(){

  int timer = 0;


  while (timer != 10){

    touchState = digitalRead(touchPin);
    digitalWrite(buzzer, HIGH); 
    digitalWrite(led, HIGH); 

    if (touchState == HIGH) {
      digitalWrite(buzzer, LOW);
      //Serial.print("sensor is pressed\n");
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(EN, 128);
      delay(motor_offset);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);

      myservo.write(100);
      delay(6000);
      myservo.write(20);
      digitalWrite(led, LOW); 
      //Serial.flush();
      break;
    }
    else {
      //Serial.print("Touch sensor is not pressed\n");
    }
    delay(1000);
    timer += 1;

  }
   digitalWrite(buzzer, LOW); 
   digitalWrite(led, LOW); 
    Serial.flush();
}