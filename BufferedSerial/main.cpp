
#include "mbed.h"
#include "TextLCD.h"
#include "HCSR.h"
//#include "BufferedSerial.h" 
#include <string>
DigitalOut myled(p25);
HCSR04  usensor(p9,p10);//allocating the lpc pins for the ultrasonic sensor
//rs, e, d4-d7 using lpc pins for interfacing with LCD16X2 to display required text on
TextLCD lcd(p19, p20, p21, p22, p23, p24,TextLCD::LCD16x2);
//Serial pc(p28, p27);
//Serial SIM800L(p13, p14);
unsigned int dist; //Distance variable declaration
int a ;
int b ;
//long GSMBAUD = 9600;
//long PCBAUD  = 9600;

int main()
{  
    myled=0;
    while(1) {
        usensor.start();       //Turns on ultrasonic sensor
        wait_ms(500);          //pauses for 500ms
        dist=usensor.get_dist_cm();    //Measured distance is recorded from sensor
        //pc.baud(PCBAUD);
        //SIM800L.baud(GSMBAUD);
        lcd.cls();              //Clears LCD screen


        a = (( dist * 100 ) / 30 )  ;
        b = 100 - a ;

        if(dist <=30) {         //Checks if measured distance is less than or equal to 30
            lcd.cls();          //Clears LCD screen
            lcd.locate(0,0);    //Sets cursor on 0,0 position on the LCD screen
            lcd.printf("Smart Bin");
            lcd.locate(0,1);
            lcd.printf("garbage  : %ld%%",b);
            myled=0;
           
        }

        if ( dist > 30 ) {
            lcd.cls();
            lcd.locate(0,0);    //Sets cursor on 0,0 position on the LCD screen
            lcd.printf("Smart Bin");
            lcd.locate(0,1);
            lcd.printf(" no data ");
            myled=0;    
                
            }
        if(dist<=8) 
        {
              myled=1;
        }
    
}
}

