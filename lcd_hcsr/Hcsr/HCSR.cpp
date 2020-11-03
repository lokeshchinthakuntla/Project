
 
 
#include "HCSR.h"
 
 
HCSR04::HCSR04(PinName TrigPin,PinName EchoPin):
    trigger(TrigPin), echo(EchoPin)
{
    pulsetime.stop();        //stops pulsetime
    pulsetime.reset();       //resets pulsetime
    echo.rise(callback(this,&HCSR04::isr_rise)); // when the pin goes from high to low
    echo.fall(callback(this,&HCSR04::isr_fall)); // when the pin goes from low to high
    trigger=0;
}
 
HCSR04::~HCSR04()
{
}
 
void HCSR04::isr_rise(void)  //
{
    pulsetime.start();
}
void HCSR04::start(void)
{
    trigger=1;     // Turns ON
    wait_us(10);   //pauses 10 micro seconds
    trigger=0;     // Turns OFF
}
 
void HCSR04::isr_fall(void)
{
    pulsetime.stop();                //stops pulstime
    pulsedur = pulsetime.read_us();  //Reads pulsetime in micro seconds
    distance= (pulsedur*343)/20000;  //calculates distance
    pulsetime.reset();               //Resets pulsetime
}
 
void HCSR04::rise (void (*fptr)(void))
{
    echo.rise(fptr);
}
void HCSR04::fall (void (*fptr)(void))
{
    echo.fall(fptr);
}
 
 unsigned int HCSR04::get_dist_cm()  //Function declaration to measure distance
{
    return distance;
}
 unsigned int HCSR04::get_pulse_us() //Function to get pulse duration
{
    return pulsedur;
}
