#include "mbed.h"

class PD30LA {
private:
    // Pin No.: 3
    // Symbol : S
    // Signal : START/STOP
    DigitalOut _pinStartStop;
    
    // Pin No.: 8
    // Symbol : C
    // Signal : EXT.CLOCK
    PwmOut _pinExtClock;
    
public:
    PD30LA(PinName pinStartStop, PinName pinExtClock) : _pinStartStop(pinStartStop), _pinExtClock(pinExtClock) {
        stop(); // initialize to stop
        _pinExtClock.write(0.5f); // PWM duty ratio = 0.5
        setRotationSpeed(rotation_speed_default); // Default speed
    }
    
    // Start the motor
    void start() {
        _pinStartStop = 0;
    }
    
    // Stop the motor
    void stop() {
        _pinStartStop = 1;
    }
    
    void flipStartStop() {
        _pinStartStop = !_pinStartStop;
    }
    
    void setRotationSpeed(const float rotation_speed) {
        float rotation_speed_to_set = clamp(rotation_speed, rotation_speed_min, rotation_speed_max);
        float f = cvtRotationSpeed2Frequency(rotation_speed_to_set);
        _pinExtClock.period(1.0f/f);
    }
    
private:
    static const float rotation_speed_min = 1000.0f; // [r/min]
    static const float rotation_speed_max = 4000.0f; // [r/min]
    static const float rotation_speed_default = 3600.0f; // [r/min]

    float cvtRotationSpeed2Frequency(const float rotation_speed) {
        return rotation_speed / 2.5f; // [Hz]
    }
    
    float clamp(const float x, const float x_min, const float x_max) {
        if( x_min > x_max) return clamp(x, x_max, x_min);
        float x_clamp = x;
        if( x_clamp < x_min ) x_clamp = x_min;
        if( x_clamp > x_max ) x_clamp = x_max;
        return x_clamp;
    }
};
