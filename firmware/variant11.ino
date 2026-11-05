const int LDR_PIN = A0;
const int NTC_PIN = A1;
const int PIR_PIN = 2;
const int RELAY_PIN = 7;
const int LED_PIN = 13;
const unsigned long INTERVAL_MS = 15000UL;

unsigned long lastSample = 0;
unsigned long sampleNumber = 0;
bool previousAlarm = false;

float mapFloat(float value, float inMin, float inMax,
               float outMin, float outMax) {
  return (value - inMin) * (outMax - outMin) /
         (inMax - inMin) + outMin;
}

void setup() {
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  if (millis() - lastSample < INTERVAL_MS) return;
  lastSample = millis();
  sampleNumber++;

  const int rawLight = analogRead(LDR_PIN);
  const int rawTemperature = analogRead(NTC_PIN);
  const int motion = digitalRead(PIR_PIN);
  const float light = mapFloat(rawLight, 0, 1023, 0, 100);
  const float temperature = mapFloat(rawTemperature, 0, 1023, 0, 50);

  Serial.print("DATA n="); Serial.print(sampleNumber);
  Serial.print(" s1="); Serial.print(light, 1);
  Serial.print(" s2="); Serial.print(temperature, 1);
  Serial.print(" s3="); Serial.println(motion);

  const bool alarm = motion == HIGH && light < 30.0;
  digitalWrite(RELAY_PIN, alarm ? HIGH : LOW);
  digitalWrite(LED_PIN, alarm ? HIGH : LOW);

  if (alarm != previousAlarm) {
    Serial.print("EVENT n="); Serial.print(sampleNumber);
    Serial.print(" type=");
    Serial.println(alarm ? "THRESHOLD" : "NORMAL");
    previousAlarm = alarm;
  }
}
