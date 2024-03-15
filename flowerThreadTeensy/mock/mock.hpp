#pragma once
#include <cstdint>
#include <cmath>
#include <iostream>

void analogReadRes(int p);
void pinMode(int p, int m);
void attachInterrupt(int t, void (*)(), int p);

class AnalogRead {
public:
    AnalogRead() = default;
    ~AnalogRead() = default;

    using func_t = int32_t (*)(uint16_t, int);

    func_t tri = [](uint16_t prev, int dir) -> int32_t {
        return prev+dir;
    };

    func_t rand = [](uint16_t prev, int _) -> int32_t {
        return prev+ (arc4random() % 10) - 5;
    };

    uint16_t
    operator()(int pin) {
        int32_t out = rand(prev, dir);
        if (out > 1023) {
            if (cnt-- > 0) {
                out = 1024;
            } else {
                out = 1023;
                dir = -1;
                cnt = remain_cnt;
            }
        } else if (out < 0) {
            out = 0;
            dir = 1;
        }
        
        prev = static_cast<uint16_t>(out);
        return out;
    }
private:
    uint16_t prev = 0;
    int dir = 1;
    constexpr static int remain_cnt = 24;
    int cnt = remain_cnt;
};

class Adafruit_NeoPixel {
    public:
    Adafruit_NeoPixel() = default;
    ~Adafruit_NeoPixel() = default;

    void
    begin() {};

    void
    show() {};

    int
    Color(uint8_t r, uint8_t g, uint8_t b) { return 0; };

    void
    setPixelColor(int p, int c) {};

    void
    clear() {};
};

class midi_mock {
    public:
    midi_mock() = default;
    ~midi_mock() = default;

    void
    sendControlChange(uint8_t a, uint8_t b, uint8_t c) {
        std::cout << "value:" << int(b) << std::endl;
    };
};

extern AnalogRead analogRead;
extern midi_mock usbMIDI;