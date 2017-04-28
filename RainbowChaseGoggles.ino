//My custom Goggles Sketch with 2 modes
//Pixels chase around the lenses beginning with a random color and offset
//Random multi color sparks flash across both lenses

#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif

#define PIN 0

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(32, PIN);

uint8_t  mode   = 0, // Current animation effect
         offset = 0; // Position of spinny eyes
uint32_t prevTime;

void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.setBrightness(64); // 1/4 brightness
  prevTime = millis();
  pixels.show();
}

void loop() {
  uint8_t  i;
  uint8_t  c;
  uint32_t t;

  switch(mode) {
    
    case 0: //RainbowChase
    c = random(255); //initial color seed
    offset = random(16); //difference between LED position on the two lenses
    for (uint8_t j=0; j < 16; j++) {  //step through color wheel   
    for (uint8_t i=0; i < 16; i++) {
       pixels.setPixelColor((16-i)%16,0);
       pixels.setPixelColor(16+(i+offset)%16,0);
       for (uint8_t q=1; q<6; q++) { //turn on next 5
          pixels.setPixelColor((32-(i+q))%16, Wheel( (c+i+q+j*16) % 255));
          pixels.setPixelColor(16+(i+q+offset)%16, Wheel( (c+i+q+j*16) % 255)); 
       }
       pixels.show();
       delay(125);
    }
    }
    break;

    case 1: //Random Sparks
    i = random(32);
    c = random(256);
    pixels.setPixelColor(i,Wheel(c));
    pixels.show();
    delay(10);
    pixels.setPixelColor(i,0);
    break;
  }
  
  t = millis();
  if((t - prevTime) > 30000) {      // Every 30 seconds...
    mode++;                        // Next mode
    if(mode > 1) {                 // End of modes?
      mode = 0;                    // Start modes over
      }
    for(i=0; i<32; i++) pixels.setPixelColor(i, 0);
    pixels.show();
    prevTime = t;
  }
}

/** Input a value 0 to 255 to get a color value.
The colours are a transition r - g - b - back to r.
This isn't my algorithm; original author unknown **/
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
