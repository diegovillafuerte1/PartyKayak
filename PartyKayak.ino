#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define STARBOARD_PIN 13
#define STARBOARD_LED_COUNT 94
#define CENTER_PIN 12
#define CENTER_LED_COUNT 72
#define PORT_PIN 11
#define PORT_LED_COUNT 94
#define COMBING_PIN 10
#define COMBING_LED_COUNT 180
//#define HATCH_PIN 9
//#define HATCH_LED_COUNT 0
////CHANGE
//#define OUTER_STRIP_PIN 8
//#define OUTER_STRIP_LED_COUNT 0
//CHANGE

Adafruit_NeoPixel starboard(STARBOARD_LED_COUNT, STARBOARD_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel center(CENTER_LED_COUNT, CENTER_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel port(PORT_LED_COUNT, PORT_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel combing(COMBING_LED_COUNT, COMBING_PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel hatch(HATCH_LED_COUNT, HATCH_PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel outer_strip(OUTER_STRIP_LED_COUNT, OUTER_STRIP_PIN, NEO_GRB + NEO_KHZ800);

//Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)

void setup() {
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
  uint8_t wait = 500;
  uint8_t bow_port_star_led_count = 58;
  uint8_t bow_center_led_count = 53;
  uint8_t middle_port_star_led_count = 16;
  uint8_t middle_center_led_count = 4;
  uint8_t stern_port_star_led_count = 20;
  uint8_t stern_center_led_count = 15;
  
  // bow
  chase(wait, 0, 0, bow_port_star_led_count, bow_center_led_count);
  
  combingChase(wait);
  // middle
  chase(wait, bow_port_star_led_count, bow_center_led_count, middle_port_star_led_count, middle_center_led_count);
  // stern
  chase(wait, bow_port_star_led_count + middle_port_star_led_count, bow_center_led_count + middle_center_led_count, stern_port_star_led_count, stern_center_led_count);
}

void chase(uint8_t wait, uint8_t port_star_starting_led, uint8_t center_starting_led, uint8_t port_led_count, uint8_t center_led_count){
  uint8_t center_counter, starboard_counter, port_counter;
  for (uint8_t i = 0; i < port_led_count; i++) {
    center_counter = i * center_led_count/port_led_count;
    starboard_counter = i;
    port_counter = i;
    starboard.setPixelColor(port_star_starting_led + starboard_counter, 214, 2, 112); // pink
    center.setPixelColor(center_starting_led + center_counter, 214, 2, 112); // pink
    port.setPixelColor(port_star_starting_led + port_counter, 214, 2, 112); // pink
    starboard.show();
    center.show();
    port.show();
    delay(wait);
  }
}

void combingChase(uint8_t wait){
  uint8_t combing_starboard_counter = COMBING_LED_COUNT/2; //90
  uint8_t combing_port_counter = COMBING_LED_COUNT/2 + 1; //91

  for (uint8_t i = 0; i < COMBING_LED_COUNT/2; i++) { // 0-90
    combing.setPixelColor(combing_starboard_counter, 214, 2, 112); // pink
    combing.setPixelColor(combing_port_counter, 214, 2, 112); // pink
    combing_starboard_counter = combing_starboard_counter--; // 89
    combing_port_counter = combing_port_counter++; 92
    combing.show();
    delay(wait);
  }
}

