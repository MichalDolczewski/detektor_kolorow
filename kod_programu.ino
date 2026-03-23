#include <math.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);


const int RGB_RED = 9;
const int RGB_GREEN = 10;
const int RGB_BLUE = 11;


const int ldrPin = A0;


const int delayLed = 200;
const int samples = 10;


const int colorDisplayTime = 2000;


const float maxDistance = 50.0;


struct ColorSample
{
  const char* name;
  int r;
  int g;
  int b;
};

// Wzorce kolorów
ColorSample patterns[] =
{
  {"POMARANCZ", 977, 801, 750},
  {"ZIELONY",   954, 904, 799},
  {"NIEBIESKI", 888, 872, 894},
  {"ZOLTY",     974, 905, 880},
  {"FIOLETOWY", 957, 880, 882},
};

const int patternCount = sizeof(patterns) / sizeof(patterns[0]);

void setup()  
{
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);

  lcd.init();
  lcd.backlight();

  turnOFFLED();

  lcd.setCursor(0, 0);
  lcd.print("START");
  delay(1000);
  lcd.clear();
}

void loop()
{
  int rValue, gValue, bValue;
  measureRGB(rValue, gValue, bValue);

  float bestDistance;
  int bestIndex = findClosestColor(rValue, gValue, bValue, bestDistance);

  // Jeśli kolor NIE został wykryty, to nic nie zatrzymujemy
  if (bestDistance > maxDistance)
  {
    lcd.setCursor(0, 0);
    lcd.print("Szukam koloru... ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    return;   // natychmiast wraca do kolejnego pomiaru
  }

  // Jeśli kolor został wykryty
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Kolor:");
  lcd.setCursor(0, 1);
  lcd.print(patterns[bestIndex].name);

  delay(colorDisplayTime);  // zatrzymanie tylko po wykryciu koloru

  lcd.clear();
}



void turnOFFLED()
{
  analogWrite(RGB_RED, 255);
  analogWrite(RGB_GREEN, 255);
  analogWrite(RGB_BLUE, 255);
}

int readLDRAverage()
{
  long sum = 0;

  for (int i = 0; i < samples; i++)
  {
    sum += analogRead(ldrPin);
    delay(5);
  }

  return sum / samples;
}

void measureRGB(int &rValue, int &gValue, int &bValue)
{
  // Pomiar czerwonego
  analogWrite(RGB_RED, 0);
  analogWrite(RGB_GREEN, 255);
  analogWrite(RGB_BLUE, 255);
  delay(delayLed);
  rValue = readLDRAverage();
  turnOFFLED();
  delay(20);

  // Pomiar zielonego
  analogWrite(RGB_RED, 255);
  analogWrite(RGB_GREEN, 0);
  analogWrite(RGB_BLUE, 255);
  delay(delayLed);
  gValue = readLDRAverage();
  turnOFFLED();
  delay(20);

  // Pomiar niebieskiego
  analogWrite(RGB_RED, 255);
  analogWrite(RGB_GREEN, 255);
  analogWrite(RGB_BLUE, 0);
  delay(delayLed);
  bValue = readLDRAverage();
  turnOFFLED();
  delay(20);
}

float euclideanDistance(int r1, int g1, int b1, int r2, int g2, int b2)
{
  return sqrt(
    pow(r1 - r2, 2) +
    pow(g1 - g2, 2) +
    pow(b1 - b2, 2)
  );
}

int findClosestColor(int rValue, int gValue, int bValue, float &bestDistance)
{
  int bestIndex = 0;

  bestDistance = euclideanDistance(
    rValue, gValue, bValue,
    patterns[0].r, patterns[0].g, patterns[0].b
  );

  for (int i = 1; i < patternCount; i++)
  {
    float dist = euclideanDistance(
      rValue, gValue, bValue,
      patterns[i].r, patterns[i].g, patterns[i].b
    );

    if (dist < bestDistance)
    {
      bestDistance = dist;
      bestIndex = i;
    }
  }

  return bestIndex;
}