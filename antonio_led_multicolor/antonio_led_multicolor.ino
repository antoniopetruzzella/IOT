#include <Adafruit_NeoPixel.h>
#define PIN D6
#define NUMPIXELS 60
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()  {
  pixels.begin();
  pixels.show();

}

void loop() {
  fadeAll(255,0,0,0,255,0,50,100);     //r1, g1, b1, r2, g2 ,b2 , fade rate , steps
  fadeAll(0,255,0,255,0,0,50,100);
}

void fadeAll(int r1, int g1, int b1, int r2, int g2, int b2, int fadeRate, int steps) {
  for (int i = 1; i < steps; i++)
  {
    uint8_t red = (((r1 * (steps - i)) + (r2 * i)) / steps);
    uint8_t green = (((g1 * (steps - i)) + (g2 * i)) / steps);
    uint8_t blue = (((b1 * (steps - i)) + (b2 * i)) / steps);
    // Sets the pixels to the color adjusted in the fade
    for (int x = 0; x < NUMPIXELS; x++) {
      pixels.setPixelColor(x, red, green, blue);
    }
    pixels.show();
    delay(fadeRate);
  }
}
