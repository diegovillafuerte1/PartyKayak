#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define STARBOARD_PIN 13
#define STARBOARD_LED_COUNT 117
#define CENTER_PIN 12
#define CENTER_LED_COUNT 72
#define PORT_PIN 11
#define PORT_LED_COUNT 117
#define COMBING_PIN 10
#define COMBING_LED_COUNT 180
//#define OUTER_STRIP_PIN 8
//#define OUTER_STRIP_LED_COUNT 0
//CHANGE

Adafruit_NeoPixel starboard(STARBOARD_LED_COUNT, STARBOARD_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel center(CENTER_LED_COUNT, CENTER_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel port(PORT_LED_COUNT, PORT_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel combing(COMBING_LED_COUNT, COMBING_PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel outer_strip(OUTER_STRIP_LED_COUNT, OUTER_STRIP_PIN, NEO_GRB + NEO_KHZ800);

//Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)

void setup() {
    // open the serial port at 9600 bps:
  Serial.begin(9600);
  // Initialize all pixels to 'off'
  starboard.begin();
  starboard.setBrightness(10);
  starboard.show();
  center.begin();
  center.setBrightness(10);
  center.show();
  port.begin();
  port.setBrightness(10);
  port.show();
  combing.begin();
  combing.setBrightness(10);
  combing.show(); 
}

void loop() {
  rainbow(20);
//  theaterChaseRainbow(20);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<206; i++) { // 206 total pixels in star or port
      setPixel(i, Wheel((i+j) & 255));
    }
    starboard.show();
    center.show();
    port.show();
    combing.show();
    delay(wait);
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < 206; i=i+3) {
        setPixel(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      starboard.show();
      center.show();
      port.show();
      combing.show();

      delay(wait);

      for (uint16_t i=0; i < 206; i=i+3) {
        setPixel(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// this sets the pixels of three neopixel strips down a kayak and a strip around the combing of the kayak as if they were all a single strip.
void setPixel(uint8_t pixelIndex, uint8_t j) {
  
  uint8_t bow_port_star_led_count = 58;
  uint8_t bow_center_led_count = 53;
  uint8_t combing_port_star_led_count = COMBING_LED_COUNT/2; 
  uint8_t middle_center_led_count = 4;
  uint8_t hatch_led_count = 39;
  uint8_t stern_port_star_led_count = 59;
  uint8_t stern_center_led_count = 15;
  uint8_t after_hatch_port_star_led_count = 20;

  // 58
  uint8_t after_bow = bow_port_star_led_count;
  // 148
  uint8_t after_combing = combing_port_star_led_count + after_bow;
  // 152
  uint8_t after_middle = middle_center_led_count + after_combing;
  // 191
  uint8_t after_hatch = hatch_led_count + after_middle;

  // because the port and starboard strips are the longest, everything will be addressed in terms of them
  if (pixelIndex > 0 && pixelIndex < after_bow) {
    
    // to set off center pixel 53 in sync with port and starboard pixel 58 we multiply the index we're at by the ratio of those lengths
    uint8_t centerIndex = pixelIndex * bow_center_led_count/bow_port_star_led_count;
    starboard.setPixelColor(pixelIndex, Wheel((pixelIndex+j) & 255));
    center.setPixelColor(centerIndex, Wheel((centerIndex+j) & 255));
    port.setPixelColor(pixelIndex, Wheel((pixelIndex+j) & 255));
  }
  // we're looking at the range from the bow through the combing
  else if (pixelIndex >= after_bow && pixelIndex < after_combing) {
    // port side will start at halfway through the combing and increase linearly with our pixelIndex (although we must account for the number of lights we've already passed in the bow)
    uint8_t combingPortIndex = combing_port_star_led_count + (pixelIndex - after_bow);
    // starboard side will decrease linearly
    uint8_t combingStarIndex = combing_port_star_led_count - (pixelIndex - after_bow);
    // we want the color on both sides to be the same, so we're passing in the starboard index into the color wheel
    combing.setPixelColor(combingPortIndex, Wheel((combingStarIndex+j) & 255));
    combing.setPixelColor(combingStarIndex, Wheel((combingStarIndex+j) & 255));
  }
  // after we've passed the combing we have a small section of the center strip between the combing and the hatch
  else if (pixelIndex >= after_combing && pixelIndex < after_middle) {
    uint8_t middleIndex = pixelIndex - combing_port_star_led_count;
    // the center strip was 5 LEDS shorter in the bow, so its starting index is 5 less than the port and starboard strips
    uint8_t middleCenterIndex = pixelIndex - combing_port_star_led_count - 5;
    starboard.setPixelColor(middleIndex, Wheel((middleIndex+j) & 255));
    center.setPixelColor(middleCenterIndex, Wheel((middleIndex+j) & 255));
    port.setPixelColor(middleIndex, Wheel((middleIndex+j) & 255));
  }
  // for the length of the hatch we just set the port and starboard strips
  else if (pixelIndex >= after_middle && pixelIndex <  after_hatch){
    uint8_t hatchIndex = pixelIndex - combing_port_star_led_count;
    starboard.setPixelColor(hatchIndex, Wheel((hatchIndex+j) & 255));
    port.setPixelColor(hatchIndex, Wheel((hatchIndex+j) & 255));
  }
  else if (pixelIndex >= after_hatch){
    uint8_t sternIndex = pixelIndex - combing_port_star_led_count;
    // the center index, while offset, will still color itself in sync with the starboard and port side
    uint8_t sternCenterIndex = pixelIndex - combing_port_star_led_count - 45;
    starboard.setPixelColor(sternIndex, Wheel((sternIndex+j) & 255));
    center.setPixelColor(sternCenterIndex, Wheel((sternIndex+j) & 255));
    port.setPixelColor(sternIndex, Wheel((sternIndex+j) & 255));
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return Adafruit_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return Adafruit_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return Adafruit_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
