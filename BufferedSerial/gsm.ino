#include <SoftwareSerial.h>
int a = 4, b = 0;

bool flag1;

//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(3, 2); //SIM800L Tx & Rx is connected to Arduino #3 & #2

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  pinMode(a, INPUT);
  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

}

void loop()
{
  b = digitalRead(a);

/**************75%*****************/
  switch (b)
  {
    case 0:
      // sensor not sense
      flag1 = 0;
     Serial.print(flag1);
      Serial.println(b);
      // remaining code
      break;
    case 1:
      // sensor sense
      if (flag1 == 0)
      { 
        Serial.println(b);

        Serial.println("Initializing...");
        delay(100);

        mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
        updateSerial();

        mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
        updateSerial();
        mySerial.println("AT+CMGS=\"+16475812594\"");//country code with phone number to sms
        updateSerial();
        mySerial.print("4410, keele street , M2N 2Y5, BIN4-10 - Garbage = 70% "); //text content
        updateSerial();
        mySerial.write(26);
        flag1 =1;
        Serial.print("flag=");
                Serial.println(flag1);
                 delay(20000);

      }
      break;
  }


}

void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (mySerial.available())
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
