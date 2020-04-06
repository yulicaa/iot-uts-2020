#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); //atau 0x3F
#include <dht.h>

#define triggerPin 14
#define echoPin 12
#define MerahRGB 2  // mendefinisikan LED Merah RGB
#define HijauRGB 13 // mendefinisikan LED Hijau RGB
#define BiruRGB 16 // mendefinisikan LED Biru RGB
#define Kuning 0 // mendefinisikan LED Kuning
#define sensorLDR A0

int sensorSuhu = 15;
int nilaiSensor;

dht DHT;


void setup() {
  Serial.begin(9600);
  Wire.begin(2,1);
  lcd.init();
  lcd.backlight();
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(MerahRGB, OUTPUT);
  pinMode(HijauRGB, OUTPUT);
  pinMode(BiruRGB, OUTPUT);
  pinMode(Kuning, OUTPUT);
}

void loop() {
  cahaya();
  suhu();
  ultra();  

}

void cahaya() {
  nilaiSensor = analogRead(sensorLDR);
  Serial.print("Nilai LDR: ");
  Serial.print(nilaiSensor);
  cahayaHasil();
}

void cahayaHasil(){
  if (nilaiSensor <= 400){
    digitalWrite(MerahRGB, HIGH);
    digitalWrite(HijauRGB, HIGH);
    digitalWrite(BiruRGB, HIGH);
    delay(1000);
    if (sensorSuhu > 25){
      digitalWrite(MerahRGB, HIGH);
    }
  }
  else if (nilaiSensor > 400) {
    digitalWrite(Kuning, HIGH);
    delay(1000);
    if (sensorSuhu < 23){
      digitalWrite(MerahRGB, LOW);
    }
  }
}

void suhu() {
  DHT.read11(sensorSuhu);
  Serial.print("Suhu:" + String(DHT.temperature, 0) + "\xDF");
  Serial.print(" | Humid:" + String(DHT.humidity, 0) + "%");
  delay(1000);
  suhuHasil();
}

void suhuHasil() {
  if (sensorSuhu > 25){  
    digitalWrite(MerahRGB, HIGH);
    delay(1000);
  }
  else if (nilaiSensor < 23){
    digitalWrite(BiruRGB, HIGH);
    delay(1000);
  }
  else if (nilaiSensor >= 23 && nilaiSensor <= 25){
    digitalWrite(MerahRGB, HIGH);
    digitalWrite(BiruRGB, HIGH);
   delay(1000);
  }
}

void ultra() {
  lcd.clear();
     long duration, jarak;
     digitalWrite(triggerPin, LOW);
     delayMicroseconds(2);
     digitalWrite(triggerPin, HIGH);
     delayMicroseconds(10);
     digitalWrite(triggerPin, LOW);
     duration = pulseIn(echoPin, HIGH);
     jarak = (duration/2) / 29.1;
     lcd.setCursor(0,0);
     lcd.print("Jarak : ");
     lcd.print(jarak);
     lcd.print(" cm");
     delay(1000);

   if(jarak<=15){ // LED merah menyala
    lcd.setCursor(0,1);
    lcd.print("Awass..");
   digitalWrite(MerahRGB, HIGH);
   digitalWrite(HijauRGB, LOW);
   digitalWrite(BiruRGB, LOW);
   delay(2000); // waktu jeda 2 detik
   } else if(jarak>15 && jarak<=50){ // LED Hijau menyala
   lcd.setCursor(0,1);
   lcd.print("Batas aman..");
   digitalWrite(MerahRGB, LOW);
   digitalWrite(HijauRGB, HIGH);
   digitalWrite(BiruRGB, LOW);
   delay(2000); // waktu jeda 2 detik
   }else{ // LED RGB akan menyala semua berkedip – kedip 
   digitalWrite(MerahRGB, HIGH);
   digitalWrite(HijauRGB, HIGH);
   digitalWrite(BiruRGB, HIGH);
   delay(1000); // waktu jeda 1 detik
   digitalWrite(MerahRGB, LOW);
   digitalWrite(HijauRGB, LOW);
   digitalWrite(BiruRGB, LOW);
   }
   delay(1000); // waktu jeda 1 detik
}
