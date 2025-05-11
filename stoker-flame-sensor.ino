#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD: alamat I2C default (0x27) dan ukuran 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definisikan pin-pin yang digunakan
const int buzzerPin = 9;        // Buzzer VCC dihubungkan ke port 9
const int flameSensorPin = 8;   // Sensor flame (wokwi-photoresistor-sensor) DO ke port 8
const int gasSensorPin = A0;    // Sensor MQ2 AOUT ke port A0

// Nilai threshold untuk kategori gas (sesuaikan dengan kalibrasi sensor)
const int gasThresholdLow = 300;    // Jika nilai di bawah 300, maka kategori "Low"
const int gasThresholdMedium = 600; // Jika nilai di antara 300 dan 600, maka kategori "Medium"
// Nilai >= gasThresholdMedium dianggap kategori "High"

void setup() {
  // Inisialisasi pin-mode
  pinMode(buzzerPin, OUTPUT);
  pinMode(flameSensorPin, INPUT);
  // Untuk sensor gas, kita menggunakan analogRead sehingga tidak perlu inisialisasi pin khusus

  // Mulai komunikasi serial untuk debugging
  Serial.begin(9600);

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();

  // Informasi awal di serial monitor
  Serial.println("KELOMPOK 8");
  Serial.println("Pendeteksi Api dengan Sensor Api");
  Serial.println("Project Arduino dimulai...");
  // informasi di lcd 
  lcd.setCursor(0, 0);
  lcd.print("KELOMPOK 8");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pendeteksi Api");
  lcd.setCursor(0, 1);
  lcd.print("dengan Sensor Api");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Project Arduino ");
  lcd.setCursor(0, 1);
  lcd.print("dimulai...");
  delay(1500);


}

void loop() {
  // Baca nilai dari sensor flame dan sensor gas
  int flameState = digitalRead(flameSensorPin);
  int gasValue = analogRead(gasSensorPin);

  // Tentukan kategori gas berdasarkan nilai yang terbaca
  String gasStatus;
  if (gasValue < gasThresholdLow) {
    gasStatus = "LOW";
  } else if (gasValue < gasThresholdMedium) {
    gasStatus = "MEDIUM";
  } else {
    gasStatus = "HIGH";
  }

  // Tampilkan informasi pada Serial Monitor
  Serial.print(flameState == HIGH ? "NO FIRE DETECTED" : "FIRE DETECTED!!!");
  Serial.print(" | SMOKE : ");
  Serial.print(gasValue);
  Serial.print(" (");
  Serial.print(gasStatus);
  Serial.println(")");

  // Perbarui tampilan LCD
  lcd.clear();
  lcd.setCursor(0, 1);
  // lcd.print("Flame: ");
  lcd.print(flameState == HIGH ? "NO FIRE DETECTED" : "FIRE DETECTED");
  lcd.setCursor(0, 0);
  lcd.print("SMOKE : ");
  lcd.print(gasStatus);

  // Logika untuk buzzer:
  // Buzzer akan aktif jika sensor flame mendeteksi api (output LOW)
  // atau jika status gas adalah Medium atau High.
if (flameState == LOW || gasStatus != "LOW") {
  tone(buzzerPin, 1000);  // Menghasilkan bunyi 1 kHz
} else {
  noTone(buzzerPin);
}

  // Delay 1 detik antar pembacaan
  delay(1000);
}
