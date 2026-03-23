const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int ldrPin = A0;

const int samples = 10;
const int delayLed = 200;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.begin(9600);

  turnOff();

  Serial.println("START KALIBRACJI");
  Serial.println("Przyloz kolor przed czujnikiem");
  delay(2000);
}

void loop() {
  int r, g, b;

  // POMIAR DLA CZERWONEGO
  analogWrite(redPin, 0);       // ON
  analogWrite(greenPin, 255);   // OFF
  analogWrite(bluePin, 255);    // OFF
  delay(delayLed);
  r = averageLED();
  turnOff();
  delay(100);

  // POMIAR DLA ZIELONEGO
  analogWrite(redPin, 255);     // OFF
  analogWrite(greenPin, 0);     // ON
  analogWrite(bluePin, 255);    // OFF
  delay(delayLed);
  g = averageLED();
  turnOff();
  delay(100);

  // POMIAR DLA NIEBIESKIEGO
  analogWrite(redPin, 255);     // OFF
  analogWrite(greenPin, 255);   // OFF
  analogWrite(bluePin, 0);      // ON
  delay(delayLed);
  b = averageLED();
  turnOff();
  delay(100);

  Serial.print("R = ");
  Serial.print(r);
  Serial.print(" | G = ");
  Serial.print(g);
  Serial.print(" | B = ");
  Serial.println(b);

  delay(3000);
}

void turnOff() {

  analogWrite(redPin, 255);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 255);
}

int averageLED() {
  long sum = 0;

  for (int i = 0; i < samples; i++) {
    sum += analogRead(ldrPin);
    delay(10);
  }

  return sum / samples;
}