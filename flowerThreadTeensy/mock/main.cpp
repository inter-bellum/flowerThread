
#include "Module.h"
#include <thread>

Module mod;

int main()
{
    using namespace std::chrono_literals;
    mod.initialize(new Adafruit_NeoPixel, 0, 0, 10, 0.1);

    while (true) {
        mod.read();
        mod.sendMidi();
        mod.updateColor();
        std::this_thread::sleep_for(5ms);
    }
}