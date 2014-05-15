#include <Adafruit_NeoPixel.h>

#define PIN 6

//boilerplate thanks to Adafruit: https://github.com/adafruit/Adafruit_NeoPixel

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);

const int buttonPin = 4;

//for LED buttom
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

//for potentiometer
int potPin = 2;    // select the input pin for the potentiometer
int oldPotVal = 0;  

//to count loops
int c = 0;

//f.lux: daylight=6500, night=3400
//http://planetpixelemporium.com/tutorialpages/light.html
//TBD: 
//ember: 1200k
//dim incandescent: 2300
//incandescent: 2700 

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(buttonPin, INPUT); 
  Serial.begin(9600);  
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      c=0;
      buttonPushCounter++;
      //Serial.println(buttonPushCounter);
    } 
    else {
    }
  }
    if (buttonPushCounter == 1) { //side 1
    colorSelect(0,30);
    c=1;  //only run color subroutine once (allows to listen for button presses)
  } else if (buttonPushCounter == 2) { //side 2
    solid_off(15,30);
    colorSelect(0,15);
    c=1;
  } else if (buttonPushCounter == 3) { //all
    solid_off(0,14);
    colorSelect(15,30);
    c=1;
  } else if (buttonPushCounter == 4) { //night light
    solid_off(0,30);
    colorSelect(29,30);
    c=1;
  } else if (buttonPushCounter == 5) { //off
    buttonPushCounter = 0;
    solid_off(0,30); //everything off
    c=1;
  }  
  lastButtonState = buttonState;
}

void colorSelect(int side_start, int side_end) {
  if (abs(analogRead(potPin) - oldPotVal) > 5) { //if pot is changed, update lighting program. otherwise, listen for button press.
    c=0; //run program
    oldPotVal=analogRead(potPin);
  }
  
  //more elegant way to do this?
  if (analogRead(potPin) < 97) {
      solid_on(255,147,41,side_start,side_end); //candle: 1900k
  } else if (analogRead(potPin) >= 97 && analogRead(potPin) < 194) {
      solid_on(255,197,143,side_start,side_end); //40W Tungsten: 2600k
      //solid_on(255,214,170,0,30); //100W Tungsten:2850k
  } else if (analogRead(potPin) >= 194 && analogRead(potPin) < 291) {
      solid_on(255, 241, 224,side_start,side_end); //Halogen: 3200k
      //solid_on(255, 250, 244,0,30); //Carbon Arc: 5200k
  } else if (analogRead(potPin) >= 291 && analogRead(potPin) < 388) {
      solid_on(255, 255, 251,side_start,side_end); //High Noon Sun: 5400k
      //solid_on(255,255,255,side_start,side_end); //Direct Sunlight: 6000k
  } else if (analogRead(potPin) >= 388 && analogRead(potPin) < 485) {
      solid_on(201, 226, 255,side_start,side_end); //Overcast Sky: 7000k
  } else if (analogRead(potPin) >= 485 && analogRead(potPin) < 582) {
      solid_on(64,156,255,side_start,side_end); //Clear Blue Sky: 20000k
  } else {
      solid_on(167,0,255,side_start,side_end); //black light
  }
}


void solid_on(uint8_t r, uint8_t g, uint8_t b, int side_start, int side_end) {
  if (c==0){
      for (int i=side_start; i < side_end; i=i+1) {
        strip.setPixelColor(i,r,g,b); 
        delay(10);
        strip.show();
      }
  }
}

void solid_off(int side_start, int side_end) {
  if (c==0){
      for (int i=side_end; i >= side_start; i=i-1) {
        strip.setPixelColor(i, 0);
        delay(10);
        strip.show();
      }
  }
}
