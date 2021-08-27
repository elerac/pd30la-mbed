#include "mbed.h"
#include "pd30la.h"
#include <cstdlib>

PD30LA pd30la = PD30LA(PB_5, PF_0); // pinStartStop, pinExtClock
InterruptIn tact_switch(PB_0);
Timer switch_timer;
Serial pc(USBTX, USBRX); // tx, rx

// Toggle motor state start-stop
// with switch debouncing
void toggle_start_stop()
{
    static const int MIN_TIME_DIFF = 200; // minimum rise time [ms]
    static int timediff = 0;
    timediff = switch_timer.read_ms();
    if(timediff > MIN_TIME_DIFF) {
        pd30la.flipStartStop(); // flip motor start-stop
    }
    switch_timer.reset();
}

// Receive and set rotation-speed from the PC
void callback () {
    float rotation_speed;
    static int success = 0;
    
    if (pc.readable()) {
        success = pc.scanf("%f", &rotation_speed);
        
        if (success) {
            pd30la.setRotationSpeed(rotation_speed);
            pc.printf("OK -> %f\r\n", rotation_speed);
        } else {
            pc.printf("NG\r\n");
        }
    }
}

int main()
{
    // tactile switch
    switch_timer.start();
    tact_switch.mode(PullUp);
    tact_switch.rise(&toggle_start_stop);
    
    // PC connection
    pc.baud(9600);
    pc.format(8, Serial::None, 1);
    pc.attach(&callback, Serial::RxIrq);
    
    // PD30LA
    pd30la.stop();
    
    while(1);
}
