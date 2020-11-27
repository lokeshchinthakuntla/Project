
#include "mbed.h"
#include "TextLCD.h"
#include "HCSR.h"
#include <string>
#define PHONE_NUMBER "+16475812594"

void sendText(char *message);
Serial pc (USBTX, USBRX);
Serial sim(p13, p14);
DigitalOut myled(p25);
HCSR04  usensor(p9,p10);
TextLCD lcd(p19, p20, p21, p22, p23, p24,TextLCD::LCD16x2);
unsigned int dist; 
int a ;
int b ;
bool flag;

int main()
{  
    sim.baud(9600);
    pc.baud(9600);
   // flag=0;
    pc.printf("lets begin\n\r");
    
    while(1) {
        usensor.start();       //Turns on ultrasonic sensor
        wait_ms(500);          //pauses for 500ms
        dist=usensor.get_dist_cm();    //Measured distance is recorded from sensor
        
        lcd.cls();              //Clears LCD screen


        a = (( dist * 100 ) / 30 )  ;
        b = 100 - a ;       
        

        if(dist <=30) {         //Checks if measured distance is less than or equal to 30
            lcd.cls();          //Clears LCD screen
            lcd.locate(0,0);    //Sets cursor on 0,0 position on the LCD screen
            lcd.printf("Smart Bin");
            lcd.locate(0,1);
            lcd.printf("garbage  : %ld%%",b);
            pc.printf("garbage  : %ld%%\n",b);
            
           
        }
        if(9< dist >=30) {         //Checks if measured distance is less than or equal to 30
          
            flag=0;
            pc.printf("garbage  : %ld%%\n",b);

           
        }

        if ( dist > 30 ) {
            lcd.cls();
            lcd.locate(0,0);    //Sets cursor on 0,0 position on the LCD screen
            lcd.printf("Smart Bin");
            lcd.locate(0,1);
            lcd.printf(" no data ");
            flag=0;
                
            }
        if(dist<8 ) 
        {
            pc.printf("garbage  : %ld%%\n",b);

            //pc.printf("flag=%d",flag);
            if(flag==0){
               
            sim.printf("AT+CMGF=1\r");
            wait(0.1f);
            sim.printf("AT+CMGS=\"");
            sim.printf(PHONE_NUMBER);
            sim.printf("\"\r");
            wait(0.1f);
            sim.printf("4410, keele street , M2N 2Y5, BIN4-10, garbage is getting full please change as soon as possible");
            wait(0.1f);
            sim.putc(0x1A);  
            pc.printf("message sent");
            flag=1;
            //pc.printf("flag=%d",flag);
            }

        }
    
}


}
