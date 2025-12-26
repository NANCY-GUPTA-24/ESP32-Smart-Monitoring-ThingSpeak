#include <WiFi.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <ESP32Servo.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

unsigned long channelID = YOUR_CHANNEL_ID;
const char* writeAPIKey = "YOUR_WRITE_API_KEY";

WiFiClient client;

// ---------- Pins ----------
#define TRIG_PIN 5
#define ECHO_PIN 18
#define SERVO_PIN 23
#define LED_PIN 2
#define LDR_PIN 34

#define MPU_ADDR 0x68

Servo myServo;

// ---------- Variables ----------
long duration;
int distance;
int ldrValue;
int16_t ax, ay, az;
int tiltStatus = 0;

// ---------- Setup ----------
void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(0);

  // MPU
  Wire.begin(21, 22);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  ThingSpeak.begin(client);
}

// ---------- Tilt Function ----------
bool tiltDetected() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  ax = Wire.read() << 8 | Wire.read();
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();

 if (abs(ax) > 25000 || abs(ay) > 25000)
{
    return true;
  }
  return false;
}

// ---------- Loop ----------
void loop() {

  // LDR
  ldrValue = analogRead(LDR_PIN);

  // Ultrasonic
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000);
  distance = duration * 0.034 / 2;

  bool tilt = tiltDetected();
  tiltStatus = tilt ? 1 : 0;

  // ---------- Local Control ----------
  if (ldrValue < 2000) { // Dark
    digitalWrite(LED_PIN, HIGH);

    if (tilt) {
      myServo.write(0);
      digitalWrite(LED_PIN, LOW);
      delay(200);
      digitalWrite(LED_PIN, HIGH);
    }
    else if (distance > 0 && distance < 30) {
      myServo.write(90);
    }
    else {
      myServo.write(0);
    }
  } 
  else {
    digitalWrite(LED_PIN, LOW);
    myServo.write(0);
  }

  // ---------- ThingSpeak ----------
  ThingSpeak.setField(1, distance);
  ThingSpeak.setField(2, ldrValue);
  ThingSpeak.setField(3, tiltStatus);

  int response = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (response == 200) {
    Serial.println("ThingSpeak Update Success");
  } else {
    Serial.println("ThingSpeak Error");
  }

  Serial.print("Distance: "); Serial.print(distance);
  Serial.print(" | LDR: "); Serial.print(ldrValue);
  Serial.print(" | Tilt: "); Serial.println(tiltStatus);

  delay(15000);  // ThingSpeak limit
}

