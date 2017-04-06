#include <OctoWS2811.h>
#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define WHITE  0xFFFFFF

const int octoPins[8] = {2, 14, 7, 8, 6, 20, 21, 5};
const int demoPin = 19;
bool demoState = false;
const int boardNumPins[3] = {18, 17, 16};
int boardNum = 0;

const int numLEDSPerStrip = 120;
DMAMEM int displayMemory[numLEDSPerStrip * 6];
int drawingMemory[numLEDSPerStrip * 6];

//OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config); 
//ledsPerStrip: The number of LEDs connected to each pin, or maximum number if different on each pin. 
//displayMemory: The memory used for display data. Use an array of "int" 6 times ledsPerStrip. 
//drawingMemory: The memory used for drawing operations. Use either an array of "int" 6 times ledsPerStrip, or NULL to perform all drawing directly to the display memory. 
//config: Configure the WS2811 speed and LED color order. Options are WS2811_RGB, WS2811_RBG, WS2811_GRB, WS2811_GBR, WS2811_800kHz, WS2811_400kHz. 
OctoWS2811 leds(numLEDSPerStrip, drawingMemory, NULL, WS2811_GRB); 

void setup() {
  // initialize the digital pin as an output.
  for (int i=0;i<8;i++){
    pinMode(octoPins[i], OUTPUT);
  }
  
  pinMode(demoPin, INPUT);
  
  for(int i=0;i<3;i++){
    pinMode(boardNumPins[i], INPUT);
  }
  
  pinState(&demoState, &boardNum);
  Serial.begin(115200);
  Serial.print("board number: ");
  Serial.print(boardNum);
  
  leds.begin();
  leds.show();
}

void loop() {
  pinState(&demoState, &boardNum);
  if(demoState){
    //board is in demo mode, ignore communication, do LED demo test
    demo();
  } else {
    //normal operating condition
  }
}

void pinState(bool* demoMode, int* boardNumber){
  int ones, twos, fours;
  *demoMode = (bool)!digitalRead(demoPin) == HIGH ? true : false;
  ones = !digitalRead(boardNumPins[0]);
  twos = !digitalRead(boardNumPins[1]);
  fours = !digitalRead(boardNumPins[2]);
  *boardNumber = ones + (twos * 2) + (fours * 4);
}

void demo(){
  Serial.println("Demo mode");
  colorWipe(WHITE);
  colorWipe(RED);
  colorWipe(GREEN);
  colorWipe(BLUE);
}

void colorWipe(int color)
{
  int LEDdelay = 50000 / leds.numPixels(); //each color wipe is in .5 seconds (50K microseconds)
  
  for (int i=0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
    leds.show();
    delayMicroseconds(LEDdelay);
  }
}
