#include <Arduino.h>

#include "led.h"
#include <FastLED.h>

#include <ArtnetWiFi.h>

// WiFi stuff
const char *ssid = "filsdecrea";
const char *pwd = "";
// const char *ssid = "riri_new";
// const char *pwd = "B2az41opbn6397";
const IPAddress ip(2, 0, 0, 106);
const IPAddress gateway(2, 0, 0, 1);
const IPAddress subnet(255, 0, 0, 0);

ArtnetWiFiReceiver artnet_in;
uint8_t universe_in = 5;

// const uint16_t size = 512;
// uint8_t data[size];
// uint8_t value = 0;

// void onArtnet(const uint8_t *data_in, const uint16_t length_in)
// {
//   for (uint8_t i = 10; i < 22; i++)
//   {
//     data[i] = data_in[i];
//   }
// }

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few
// of the kinds of animation patterns you can quickly and easily
// compose using FastLED.
//
// This example also shows one easy way to define multiple
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#define DATA_PIN 32
// #define CLK_PIN   4
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS 64
CRGB leds[NUM_LEDS];

// #define BRIGHTNESS          96
// #define FRAMES_PER_SECOND  120
// // List of patterns to cycle through.  Each is defined as a separate function below.
// uint8_t gHue = 0; // rotating "base color" used by many of the patterns
// uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
// #include "effect.h"

void test_fast_led(){
  leds[0] = CRGB::Red;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Green;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Blue;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Black;
  FastLED.show();
  delay(500);
};

void setup()
{

  Serial.begin(115200);
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  // FastLED.setBrightness(BRIGHTNESS);

  // WiFi stuff
  WiFi.begin(ssid, pwd);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("WiFi connected, IP = ");
  Serial.println(WiFi.localIP());

  String NAME = "Bouteille";
  artnet_in.shortname(NAME);
  artnet_in.longname(NAME);

  artnet_in.begin(); // artnet.begin(net, subnet); // optionally you can change net and subnet
                     // artnet_in.subscribe(universe_in, onArtnet);
  artnet_in.forward(universe_in, leds, NUM_LEDS);

  // init led
  init_led();

  // test fast led
  test_fast_led();
}

void loop()
{
  // // Call the current pattern function once, updating the 'leds' array
  // gPatterns[gCurrentPatternNumber]();

  // // send the 'leds' array out to the actual LED strip
  // FastLED.show();
  // // insert a delay to keep the framerate modest
  // FastLED.delay(1000 / FRAMES_PER_SECOND);

  // // do some periodic updates
  // EVERY_N_MILLISECONDS(20) { gHue++; }   // slowly cycle the "base color" through the rainbow
  // EVERY_N_SECONDS(10) { nextPattern(); } // change patterns periodically

  artnet_in.parse(); // check if artnet packet has come and execute callback
  FastLED.show();
  onboard_led.on = millis() % 2000 < 1000;
  onboard_led.update();
}
