/*
 * ============================================================
 *  Wireless AC Power Detector
 *  Author  : Saksham Chamoli
 *  Hardware: ESP32 DevKit v1 + Coil Antenna + NPN Transistor
 *            + 16x2 I2C LCD + Buzzer + LED
 * ============================================================
 *  Principle:
 *    A coil (antenna) placed near a live AC wire picks up the
 *    50Hz electromagnetic field. The weak induced voltage is
 *    amplified by an NPN transistor (BC547) and fed into the
 *    ESP32 ADC pin. High ADC reading = live wire detected.
 *
 *  Wiring:
 *    Coil antenna    --> Transistor Base (via 100k resistor)
 *    Transistor (BC547):
 *      Base           -- 100kΩ -- Coil
 *      Collector      -- 10kΩ  -- 3.3V (pull-up)
 *      Collector      ──────── -- GPIO34 (ADC input)
 *      Emitter        ──────── -- GND
 *
 *    Buzzer (+)       --> GPIO25
 *    Status LED (+)   --> GPIO26 (via 220Ω)
 *    LCD SDA          --> GPIO21
 *    LCD SCL          --> GPIO22
 *
 *  ⚠️ This is a NON-CONTACT detector — it detects the
 *     electromagnetic field only. Do NOT touch live wires.
 * ============================================================
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ── Pin Definitions ──────────────────────────────────────────
#define SENSOR_PIN   34   // ADC input from transistor collector
#define BUZZER_PIN   25   // Piezo buzzer
#define LED_PIN      26   // Status LED

// ── Thresholds ────────────────────────────────────────────────
// Adjust DETECTION_THRESHOLD based on your coil sensitivity
// Higher value = needs stronger field (less sensitive)
#define DETECTION_THRESHOLD  1500   // ADC value out of 4095
#define NUM_SAMPLES          20     // Samples averaged per reading

// ── LCD ───────────────────────────────────────────────────────
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ── State ─────────────────────────────────────────────────────
bool lastState     = false;
bool currentState  = false;
unsigned long lastBeep = 0;

// ── Read average ADC ──────────────────────────────────────────
int readAveraged() {
  long sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += analogRead(SENSOR_PIN);
    delayMicroseconds(200);
  }
  return (int)(sum / NUM_SAMPLES);
}

// ── Setup ────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN,    OUTPUT);
  analogSetAttenuation(ADC_11db);  // For full 0–3.3V ADC range

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("  AC  Detector  ");
  lcd.setCursor(0, 1);
  lcd.print(" Saksham Chamoli");
  delay(2000);
  lcd.clear();

  // Startup beep
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("Wireless AC Power Detector Ready.");
  Serial.println("Bring near a live wire to detect...");
}

// ── Main Loop ────────────────────────────────────────────────
void loop() {
  int adcValue = readAveraged();
  currentState = (adcValue > DETECTION_THRESHOLD);

  // Serial output
  Serial.printf("ADC: %4d | %s\n", adcValue,
    currentState ? "LIVE DETECTED ⚡" : "No Signal");

  // LCD update
  lcd.setCursor(0, 0);
  if (currentState) {
    lcd.print("  ⚡ LIVE WIRE! ");
    lcd.setCursor(0, 1);
    lcd.print("  DANGER NEARBY ");
    digitalWrite(LED_PIN, HIGH);

    // Buzz every 300ms while live wire detected
    if (millis() - lastBeep > 300) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(50);
      digitalWrite(BUZZER_PIN, LOW);
      lastBeep = millis();
    }

  } else {
    lcd.print("  No AC Signal  ");
    lcd.setCursor(0, 1);
    lcd.print("  Safe  Zone    ");
    digitalWrite(LED_PIN,    LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Print state change to Serial
  if (currentState != lastState) {
    if (currentState) {
      Serial.println(">>> LIVE AC WIRE DETECTED <<<");
    } else {
      Serial.println(">>> Signal Lost <<<");
    }
    lastState = currentState;
  }

  delay(100);
}
