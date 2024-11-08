/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
#define BLYNK_TEMPLATE_NAME         "ESP32"
#define BLYNK_AUTH_TOKEN            "HqRnzwSxTvgkD-eVrh1tT0gZAHvOCkBs"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <Servo_ESP32.h>

#define trig 4 // Replace with the correct GPIO pin number for the trigger pin
#define echo 2 // Replace with the correct GPIO pin number for the echo pin


long duration;
int distance;

static const int servoPin = 18; //printed G14 on the board

Servo_ESP32 servo1;

int angle =0;
int angleStep = 10;

int angleMin =0;
int angleMax = 180;

char auth[] = "HqRnzwSxTvgkD-eVrh1tT0gZAHvOCkBs"; // Use the BLYNK_AUTH_TOKEN you defined

// Your WiFi credentials.
// Set the password to "" for open networks.
char ssid[] = "Amshu GT 2";
char pass[] = "Amshu@399";

BlynkTimer timer;
BlynkTimer turn;
WidgetLCD lcd(V1);

void setup()
{
  pinMode(trig, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echo, INPUT);   // Sets the echoPin as an Input
  Serial.begin(9600);

  servo1.attach(servoPin);

  Blynk.begin(auth, ssid, pass,IPAddress(117,236,190,213),8080); // Change the IP address to "blynk-cloud.com" and remove IPAdress

  // Setup a function to be called every second
  timer.setInterval(100L, sendSensor);
  turn.setInterval(1000L, rotate);
}

void loop()
{
  Blynk.run();
  timer.run();
  turn.run();
  timer.run();
}

void rotate()
{
  for(int angle = 0; angle <= angleMax; angle +=angleStep) {
        servo1.write(angle);
        timer.run();
        // Serial.println(angle);
        delay(100);
    }
    timer.run();

    for(int angle = 180; angle >= angleMin; angle -=angleStep) {
        servo1.write(angle);
        timer.run();
        // Serial.println(angle);
        delay(10);
        timer.run();
    }

}

void sendSensor()
{
  digitalWrite(trig, LOW);   // Makes trigPin low
  delayMicroseconds(2);       // 2 microsecond delay

  digitalWrite(trig, HIGH);  // trigPin high
  delayMicroseconds(2);      // trigPin high for 10 microseconds
  digitalWrite(trig, LOW);   // trigPin low

  duration = pulseIn(echo, HIGH);   // Read echo pin, time in microseconds
  distance = duration * 0.034 / 2;   // Calculating actual/real distance

  Serial.print("Distance = ");        // Output distance on the Arduino serial monitor
  Serial.println(distance);

  Blynk.virtualWrite(V0, distance);

  lcd.print(0, 0, "Distance: " + String(distance) + "cm  "); // Update LCD widget with distance
}


