const byte unityLEDPin = 9;
const byte unityLightInputPin = 7;
const byte potentiometer1Pin = A7;

const byte redPin = 4;
const byte greenPin = 3;
const byte bluePin = 2;

const byte value = 20;

//variables for color adjustments on potentiometer
byte hue = 0;
byte red = 0;
byte blue = 0;
byte green = 0;

//light feedback
bool poweredOn = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(unityLEDPin, OUTPUT);
  pinMode(unityLightInputPin, INPUT);
  pinMode(potentiometer1Pin, INPUT);

  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  //process on/off, prototype unity feedback
  byte unityLightInput = digitalRead(unityLightInputPin);
  Serial.print(unityLightInput);
  Serial.print(" ");
  if (unityLightInput == 1 && poweredOn == false) {
    poweredOn = true;
    digitalWrite(unityLEDPin, HIGH);
  } else if (unityLightInput == 0 && poweredOn == true) {
    poweredOn = false;
    digitalWrite(unityLEDPin, LOW);
  }

  //process color
  int potIn = analogRead(potentiometer1Pin);
  hue = map(potIn, 0, 1023, 0, 255);
  Serial.print(potIn);
  Serial.print(" ");
  Serial.println(hue);
  calculateColors(hue);
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);

}

//Takes target hue value and processes into which format to use
void calculateColors(int hueIn) {
  //hue = hueIn;
  //HSV to RGB conversion formula found here: https://www.rapidtables.com/convert/color/hsv-to-rgb.html
  //All code is original

  //because value and saturation are both assumed to be 100%, the math involves A LOT of 1s
  double s = 1.0;
  double v = value / 100.0;
  double C = v * s;
  double X = C * (1.0 - abs(fmod(((double)hueIn / 60.0), 2.0) - 1.0));
  //Serial.println(fmod(((double) hueIn / 60.0), 2.0));
  int m = v - C;

  //assigns arrangement of coefficients C, X, and 0 based on input hue
  //then calls writeColors with that arrangement, which will convert that to RGB values
  if (hueIn >= 0 && hueIn < 60) {
    writeColors(C, X, 0, m);
  } else if (hueIn >= 60 && hueIn < 120) {
    writeColors(X, C, 0, m);
  } else if (hueIn >= 120 && hueIn < 180) {
    writeColors(0, C, X, m);
  } else if (hueIn >= 180 && hueIn < 240) {
    writeColors(0, X, C, m);
  } else if (hueIn >= 240 && hueIn < 300) {
    writeColors(X, 0, C, m);
  } else if (hueIn >= 300 && hueIn < 360) {
    writeColors(C, 0, X, m);
  }
}

//Convert input arrangement of rgb (on a float scale of 0.0 to 1.0) and zone to int values 0-255 for that zone
void writeColors(double thisRed, double thisGreen, double thisBlue, double m) {
  //map 0.0-1.0 to 0-255
  //(thisRed + m) *= 255;
  //(thisBlue + m) *= 255;
  //(thisGreen + m) *= 255;
  thisRed = (thisRed + m) * 255;
  thisGreen = (thisGreen + m) * 255;
  thisBlue = (thisBlue + m) * 255;

  red = thisRed;
  blue = thisBlue;
  green = thisGreen;
  /*
  Serial.print(red);
  Serial.print(" ");
  Serial.print(green);
  Serial.print(" ");
  Serial.println(rgbTimer);
  */
}
