
#include "mbed.h"
#include "TextLCD.h"
#include "HCSR.h"
#include <string>
#include "ESP8266.h"               // Include header file from Author: Antonio Quevedo


#define APIKEY OACI719X0K6W05TB    //Put "Write key" of your channel in thingspeak.com 
#define IP "184.106.153.149"       // IP Address of "api.thingspeak.com\"
#define WIFI_SSID "YASHPATEL"      // wifi SSID
#define WIFI_PASS "SWAMINARAYAN1008"   // Wifi password
#define PHONE_NUMBER "+16475812594"   // phone number to get SMS alert

void sendText(char *message);
Serial pc (USBTX, USBRX);   // to monitor on serial port
Serial sim(p13, p14);       // GSM pin declaration
ESP8266 esp(p28, p27, 115200); // ESP8266 pin declaration and baud rate for wifi
char snd[255],rcv[1000],snd_Data[255];
HCSR04  usensor(p9,p10);  // ultrasonic sensor pin declaration
TextLCD lcd(p19, p20, p21, p22, p23, p24,TextLCD::LCD16x2); //LCD pin declaration
unsigned int dist; // to get distance sensed by sensor
int a ;
int b ;
int c ; 
bool flag;

void esp_initialize(void); // Function used to initialize ESP8266 wifi module 


int main()
{  
    sim.baud(9600);
    pc.baud(115200);
    pc.printf("START\r\n"); 
  
    pc.printf("lets begin\n\r");
    esp_initialize();

    
    while(1) {
        usensor.start();       //Turns on ultrasonic sensor
        wait_ms(500);          //pauses for 500ms
        dist=usensor.get_dist_cm();    //Measured distance is recorded from sensor
        
        lcd.cls();              //Clears LCD screen
        
        // percentage conversion calculation     
        a= dist-26;
        c=a*5;  
        b = -c ;

        if(dist <=26) {         //Checks if measured distance is less than or equal to 30
            lcd.cls();          //Clears LCD screen
            lcd.locate(0,0);    //Sets cursor on 0,0 position on the LCD screen
            lcd.printf("Smart Bin");  // display smart bin 
            lcd.locate(0,1);
            lcd.printf("garbage  : %ld%%",b);
            pc.printf("garbage  : %ld%%\n",b);
            
            wait(15);        
            strcpy(snd,"AT+CIPSTART=");// send command to GSM 
            strcat(snd,"\"TCP\",\"");
            strcat(snd,IP);
            strcat(snd,"\",80");
            
            esp.SendCMD(snd); 
            pc.printf("S\r\n%s",snd);
            //wait(2);                                                    
            esp.RcvReply(rcv, 1000);
            pc.printf("R\r\n%s",rcv);
            wait(1);
            
            sprintf(snd,"GET https://api.thingspeak.com/update?key=OACI719X0K6W05TB&field1=%d\r\n",b);
           
            int i=0;
            for(i=0; snd[i]!='\0'; i++);
            i++;
            char cmd[255];

            sprintf(cmd,"AT+CIPSEND=%d",i);       //Send Number of open connection and Characters to send
            esp.SendCMD(cmd);
            pc.printf("S\r\n%s",cmd);
            while(i<=20 || rcv == ">") {
                esp.RcvReply(rcv, 1000);
                wait(100);
                i++;
            }
            pc.printf("R\r\n%s",rcv);

            esp.SendCMD(snd);                     //Post value to thingspeak channel
            pc.printf("S\r\n%s",snd);

            while(i<=20 || rcv == "OK") {
                esp.RcvReply(rcv, 1000);
                wait(100);
                i++;
            }
            pc.printf("R\r\n%s",rcv);
      
           
        }
        
        
        
        if(6< dist >=26) {         //Checks if measured distance is less than or equal to 30
          
            flag=0;
            pc.printf("garbage  : %ld%%\n",b);

           
        }

        if ( dist > 26 ) {
            lcd.cls();
            lcd.locate(0,0);    //Sets cursor on 0,0 position on the LCD screen
            lcd.printf("Smart Bin");
            lcd.locate(0,1);
            lcd.printf(" no data ");
            flag=0;
                
            }
        if(dist<12 ) 
        {
            pc.printf("garbage  : %ld%%\n",b);
            
            if(flag==0){    
            sim.printf("AT+CMGF=1\r");
            wait(0.1f);
            sim.printf("AT+CMGS=\"");
            sim.printf(PHONE_NUMBER);
            sim.printf("\"\r");
            wait(0.1f);
            sim.printf("4410, keele street , M2N 2Y5, BIN4-10, garbage is getting full please change as soon as possible"); // Message
            wait(0.1f);
            sim.putc(0x1A);  
            pc.printf("message sent\n");
            flag=1;
            }

        }
    
}


}

void esp_initialize(void)
{    
    pc.printf("Initializing ESP\r\n"); 
      
    pc.printf("Reset ESP\r\n"); 
    esp.Reset();                   //RESET ESP
    esp.RcvReply(rcv, 400);        //receive a response from ESP
    pc.printf(rcv);             //Print the response onscreen 
    wait(2);
    
    strcpy(snd,"AT");
    esp.SendCMD(snd);
    pc.printf(snd);
    //wait(2);
    esp.RcvReply(rcv, 400);       
    pc.printf(rcv);      
    wait(0.1);
    
    strcpy(snd,"AT+CWMODE=1");
    esp.SendCMD(snd);
    pc.printf(snd);
    wait(2);

    strcpy(snd,"AT+CWJAP=\"");
    strcat(snd,WIFI_SSID);
    strcat(snd,"\",\"");
    strcat(snd,WIFI_PASS);
    strcat(snd,"\"");
    
    esp.SendCMD(snd);
    pc.printf(snd);
    wait(5);
    esp.RcvReply(rcv, 400);       
    pc.printf("\n %s \n", rcv); 
    
    strcpy(snd,"AT+CIPMUX=0");
    esp.SendCMD(snd);
    pc.printf(snd);
    //wait(2);
    esp.RcvReply(rcv, 400);       
    pc.printf("\n %s \n", rcv); 

}

