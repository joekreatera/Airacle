#include <Adafruit_NeoPixel.h>

#define DIM_MODE 0 
#define CHASE_MODE 1
#define PIN   7
#define N_LEDS 12 // 5 meter reel @ 30 LEDs/m

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

int led_steps = 0;
int led_direction = 1;
int updateLedSteps(int step_max, bool do_pong) {

  if( !do_pong ) led_direction = 1;
  
  led_steps += led_direction;

  if (led_steps >= step_max ) {
    if ( do_pong )
      led_direction *= -1;
    else
      led_steps = 0;
  }
  if (led_steps <= 0 && do_pong ) {
    if(led_direction < 0) // normally this would not be needed but there is a possibility in which the update gets called twice with a rare combination of do_pong
      led_direction *= -1;
  }
  return step_max;
}



void ledChase(uint32_t chase_color, uint32_t base_color, int line_length, int led_step, int led_total_steps) {

  uint16_t color_step = (uint16_t)((led_step * 1.0 / led_total_steps) * strip.numPixels());
  int line_length_2 = line_length >> 1;

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i  , base_color); // Draw new pixel
  }

  for (uint16_t i = color_step - line_length_2 ; i <= color_step + line_length_2 ; i++ ) {
    int c = i % strip.numPixels();
    strip.setPixelColor(i  , chase_color); // Draw new pixel

  }
  strip.show();
}

void dimLed(uint32_t from, uint32_t to , int led_step, int led_total_steps ) {

  int rF = (int)((from & 0xFF0000) >> 16);
  int gF = (int)((from & 0x00FF00) >> 8);
  int bF = (int)((from & 0x0000FF)) ;

  int rT = (int)((to & 0xFF0000) >> 16);
  int gT = (int)((to & 0x00FF00) >> 8);
  int bT = (int)((to & 0x0000FF)) ;

  float fs = led_step * 1.0 / led_total_steps;
  int finalR = rF + (int)((rT - rF) * fs);
  int finalG = gF + (int)((gT - gF) * fs);
  int finalB = bF + (int)((bT - bF) * fs);

  //Serial.print(fs);Serial.print("::--->"); Serial.print(finalR);Serial.print(":");Serial.print(finalG);Serial.print(":");Serial.println(finalB);

  uint32_t finalR32 = ((uint32_t)finalR) << 16;
  uint32_t finalG32 = ((uint32_t)finalG) << 8;
  uint32_t finalB32 = (uint32_t)finalB;


  uint32_t final_color = finalR32 + finalG32 + finalB32 ;

  //Serial.print(from, BIN);Serial.print("::"); Serial.print(finalR32);Serial.print(":");Serial.print(finalG32);Serial.print(":");Serial.println(finalB32);
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i  , final_color); // Draw new pixel
  }
  strip.show();
}



void showLEDs(int mode, uint32_t color1 , uint32_t color2 , int color_width, int max_steps, bool pong ) {



 if( mode == DIM_MODE ){
    dimLed(color1,color2, led_steps, updateLedSteps(max_steps,pong));
 }
 if( mode == CHASE_MODE ){
    ledChase(color1, color2, color_width, led_steps, updateLedSteps(max_steps, pong));
 }
  
  //
  /*
    if( col == 0 ){
    chase(0xFF0000); // Red
    chase(0x00FF00); // Green
    chase(0x0000FF); // Blue
    }
    if( col == 1 ){
    chase(0xFF0000); // Red
    }
    if( col == 2 ){
    chase(0x00FF00); // Green
    }

    if( col == 3 ){
    chase(0x0000FF); // Blue
    }*/

}

void setupLED(){
   strip.begin();
}

static void chase(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels() + 4; i++) {
    strip.setPixelColor(i  , c); // Draw new pixel
    strip.setPixelColor(i - 4, 0); // Erase pixel a few steps back
    strip.show();
    delay(10);
  }
}


void showOneColorLED(uint32_t col){
       showLEDs(DIM_MODE, col, 0x000000 , 0 , 0, false);  // steps are the amount of divisions that the color grading shoould take. On each DT(delta time) one of this divisions should be added. 
}

void showBreathingLED(uint32_t from, uint32_t to ){
      showLEDs(DIM_MODE, from, to , 0 , 30, true);  // steps are the amount of divisions that the color grading shoould take. On each DT(delta time) one of this divisions should be added. 
}

void showQuickBreathingLED(uint32_t from, uint32_t to ){
      showLEDs(DIM_MODE, from, to , 0 , 15, true);  // steps are the amount of divisions that the color grading shoould take. On each DT(delta time) one of this divisions should be added. 
}

void showFlickerLED(uint32_t from, uint32_t to ){
      showLEDs(DIM_MODE, from, to , 0 , 4, true);  // steps are the amount of divisions that the color grading shoould take. On each DT(delta time) one of this divisions should be added. 
}

void showChaseLED(uint32_t front, uint32_t back){
  showLEDs(CHASE_MODE, front, back , 2 , 15, false);  // steps are the amount of divisions that the color grading shoould take. On each DT(delta time) one of this divisions should be added. 
}
