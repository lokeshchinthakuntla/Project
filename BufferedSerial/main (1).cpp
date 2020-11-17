
#include "mbed.h"
#include "TextLCD.h"
#include "HCSR.h"
#include "BufferedSerial.h"
#include <string>

HCSR04  usensor(p9,p10);//allocating the lpc pins for the ultrasonic sensor
//rs, e, d4-d7 using lpc pins for interfacing with LCD16X2 to display required text on
TextLCD lcd(p19, p20, p21, p22, p23, p24,TextLCD::LCD16x2);
Serial pc(p28, p27);
Serial SIM800L(p13, p14);
unsigned int dist; //Distance variable declaration
int a ;
int b ;
long GSMBAUD = 9600;
long PCBAUD  = 9600;


int main()
{
    while(1) {
        usensor.start();       //Turns on ultrasonic sensor
        wait_ms(500);          //pauses for 500ms
        dist=usensor.get_dist_cm();    //Measured distance is recorded from sensor
        pc.baud(PCBAUD);
        SIM800L.baud(GSMBAUD);
        lcd.cls();              //Clears LCD screen


        a = (( dist * 100 ) / 30 )  ;
        b = 100 - a ;

        if(dist <=30) {         //Checks if measured distance is less than or equal to 30
            lcd.cls();          //Clears LCD screen
            lcd.locate(0,0);    //Sets cursor on 0,0 position on the LCD screen
            lcd.printf("Smart Bin");
            lcd.locate(0,1);
            lcd.printf("garbage  : %ld%%",b);

        }

        if ( dist > 30 ) {
            lcd.cls();
            lcd.locate(0,0);    //Sets cursor on 0,0 position on the LCD screen
            lcd.printf("Smart Bin");
            lcd.locate(0,1);
            lcd.printf(" no data ");
        }
        if(dist<15) {
            SIM800L.printf("AT");
            SIM800L.printf("\r\n");
            wait(0.1f);

            SIM800L.printf("AT+CMGF=1");
            SIM800L.printf("\r\n");
            wait(0.1f);

            SIM800L.printf("AT+CMGS=\"+16475812594\"");
            SIM800L.printf("\r\n");
            wait(0.1f);

            SIM800L.printf("4410, keele street , M2N 2Y5, BIN4-10 Garbage bin- 70%");
            SIM800L.printf("\r\n");
            wait(0.1f);
            wait_ms(500);
        }

        while(pc.readable()) {
            SIM800L.putc(pc.getc());
            SIM800L.printf("%c", SIM800L.putc(pc.getc()));
            SIM800L.printf("\r\n");
        }
        wait(0.1f);
        while(SIM800L.readable()) {
            pc.putc(SIM800L.getc());
            pc.printf("%c", pc.putc(SIM800L.getc()));
        }
        wait(0.1f);


    }
}


