// Вариант 11. Умная лестница.
// Датчики: LDR (s1, освещённость, 0-100 %), NTC (s2, температура, -20...80 °C), PIR (s3, движение 0/1).
// Исполнительное устройство: реле (светильник), светодиод красный 15 мА.
// Порог события: PIR = 1 и освещённость < 30 %.

const unsigned long T_MS = 3000UL;   // период опроса T = 3 с

const int PIN_S1    = A1;            // LDR (по схеме EasyEDA)
const int PIN_S2    = A0;            // NTC, выход OUT
const int PIN_S3    = 2;             // PIR, выход OUT
const int PIN_RELAY = 8;             // реле, вход IN
const int PIN_LED   = 9;             // светодиод через резистор R1

unsigned long n = 0;                 // номер измерения
bool alarm = false;                  // порог сейчас превышен?
unsigned long lastMs = 0;

// Линейное преобразование кода АЦП в физическую величину
float scaleValue(int raw, float vMin, float vMax) {
  return vMin + (vMax - vMin) * raw / 1023.0;
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_S3, INPUT);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);
  digitalWrite(PIN_LED, LOW);
}

void loop() {
  if (millis() - lastMs < T_MS) return;
  lastMs += T_MS;
  n++;

  float s1 = scaleValue(analogRead(PIN_S1), 0.0, 100.0);    // освещённость, %
  float s2 = scaleValue(analogRead(PIN_S2), -20.0, 80.0);   // температура, °C
  int   s3 = digitalRead(PIN_S3);                           // движение, 0/1

  Serial.print("DATA n=");  Serial.print(n);
  Serial.print(" s1=");     Serial.print(s1, 1);
  Serial.print(" s2=");     Serial.print(s2, 1);
  Serial.print(" s3=");     Serial.println(s3);

  bool nowAlarm = (s3 == 1) && (s1 < 30.0);
  if (nowAlarm != alarm) {
    alarm = nowAlarm;
    digitalWrite(PIN_RELAY, alarm ? HIGH : LOW);
    digitalWrite(PIN_LED, alarm ? HIGH : LOW);
    Serial.print("EVENT n="); Serial.print(n);
    Serial.println(alarm ? " type=THRESHOLD" : " type=NORMAL");
  }
}
