#include "Adafruit_TCS34725.h"
#include <LiquidCrystal_I2C.h>

// Pin tombol deteksi warna
#define TOMBOL_DETEKSI 4

// Pin LED RGb
#define RGB_RED 3
#define RGB_GREEN 5
#define RGB_BLUE 6

#define COMMON_ANODE false

LiquidCrystal_I2C lcd(0x27,16,2);

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);

byte gammatable[256];

float nilaiR, nilaiG, nilaiB;
int baca;

void setWarnaRGB(short r, short g, short b);
int bacaTombolDeteksi();

void setup() {
  Serial.begin(9600);
  
  tcs.begin();
  
  lcd.init();
  lcd.backlight();

  pinMode(TOMBOL_DETEKSI, INPUT);
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);

  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    if (COMMON_ANODE) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;
    }
  }
}

void loop() {
  while(bacaTombolDeteksi() == HIGH){
    lcd.setCursor(0,0);
    lcd.print("Mencari Warna");
    tcs.setInterrupt(false);  // turn on LED
    delay(154);
    tcs.getRGB(&nilaiR, &nilaiG, &nilaiB);
    tcs.setInterrupt(true);  // turn off LED
    setWarnaRGB((int)nilaiR,(int)nilaiG,(int)nilaiB);
    lcd.clear();
  }
  lcd.setCursor(0,0);
  lcd.print("Warna Terdeteksi");
  setWarnaRGB((int)nilaiR,(int)nilaiG,(int)nilaiB);
}

int bacaTombolDeteksi(){
  return digitalRead(TOMBOL_DETEKSI);
}

void setWarnaRGB(int r, int g, int b){
  String text = "RGB(";
  text += r;
  lcd.setCursor(0,1);
  lcd.print(text);
  text = ",";
  text += g;
  lcd.setCursor(7,1);
  lcd.print(text);
  text = ",";
  text += b;
  lcd.setCursor(11,1);
  lcd.print(text);
  text = ")";
  lcd.setCursor(15,1);
  lcd.print(text);
  
  analogWrite(RGB_RED, gammatable[r]);
  analogWrite(RGB_GREEN, gammatable[g]);
  analogWrite(RGB_BLUE, gammatable[b]);
}
