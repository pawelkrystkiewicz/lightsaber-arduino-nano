/*
  LIGHTSABER @Arduino Nano (Atmega 328P)
  Author: Paweł Krystkiewicz
  Created: 20.04.2017
  Not final 
*/

#include <EEPROM.h>
#include <Wire.h>
#include "DFRobotDFPlayerMini.h"
#include "Device.h"
#include <MMA8453_n0m1.h>
//#include <SoftwareSerial.h>
//#include <PinChangeInt.h>

//#define BUSY  32
//#define BAT_LVL  28
#define KEY1 2
#define KEY2 9
#define RX 29
#define TX 30

//SoftwareSerial portOne(RX, TX); // RX, TX

MMA8453_n0m1 accel;

DFRobotDFPlayerMini sound;

Device saber;

void setup()
{
  Serial.begin(9600);
  saber.Idle();

  saber.save[0] = saber.color;
  saber.save[1] = saber.flash;
  saber.save[2] = saber.volume;
  saber.save[3] = saber.addon;

  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(W, OUTPUT);

  saber.state = 0;

  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);
  

  ////////////////ODCZYT Z EEPROM
  byte adr = 0;
  for (adr; adr >= 4; adr++)
  {
    EEPROM.get(adr, saber.save[adr]);
  }

  if ( saber.save[0] == 0 )
  {
    saber.save[0] = 1;
  }
  if ( saber.save[1] == 0 )
  {
    saber.save[1] = 3;
  }
  if ( saber.save[2] == 0 )
  {
    saber.save[2] = 15;
  }
  if ( saber.save[3] == 0 )
  {
    saber.save[3] = 1;
  }



///////////////////////////////
//DO OSOBNEGO UARTU



while (!Serial)
{
  ; // wait for serial port to connect. Needed for native USB port only
}


// Start each software serial port
//portOne.begin(9600);
// portTwo.begin(9600);

//---------------------
accel.setI2CAddr(0x1D); //change your device address if necessary, default is 0x1C
/*
  threshold [0-127] formula: 6g/ 0.063g = 95.2 counts, round to 96 counts ,
  enable X,
  enable Y,
  enable Z,
  enable MMA8453Q INT pin 2 (false= pin 1),
  arduino INT pin number
*/
accel.shakeMode(8, true, true, true, false, 4);

sound.setTimeOut(500);
sound.volume(saber.volume);  //Set volume value. From 0 to 30
sound.play(1);  //boot.mp3 //Play the first mp3
sound.EQ(DFPLAYER_EQ_NORMAL);
sound.outputDevice(DFPLAYER_DEVICE_SD);

}

void loop()
{
  unsigned long t = 0;
  accel.update();
  byte a = 0;


  //UDERZENIE
  if (accel.shake())
  {
    if (accel.shakeAxisX() || accel.shakeAxisY() || accel.shakeAxisZ())
    {
      saber.state = 5;
    }
  }
  //RUCH URZADZENIA
  if (accel.motion())
  {
    saber.state = 4;
  }


  ///KEY MANAGMENT
  if (!digitalRead(KEY1) && saber.state == 0)
  {
    saber.state = 1;
  }
  if (!digitalRead(KEY1) && saber.state == 2)
  {
    saber.state = 3;
  }
  if (!digitalRead(KEY2) && saber.state == 0)
  {
    //poziom baterii
  }
  if (!digitalRead(KEY2) && saber.state == 2)
  {
    saber.state = 6; //LOCK
  }
  if (!digitalRead(KEY1) && !digitalRead(KEY2))
  {
    //SETUP
    saber.state = 9;
  }


  switch (saber.state)
  {
    //////////////////
    case 0: //SABER OFF/IDLE
      saber.Idle();
      sound.stop();
      sound.sleep();
      break;
    /////////////////
    case 1: //ON
      //sound.playFolder(15, 4);
      sound.play(2);
      saber.On();
      saber.state = 2;
      break;
    /////////////////
    case 2://NORMALNA PRACA
      saber.Blink();
      sound.play(3);
    /////////////////
    case 3://OFF
      saber.Off();
      saber.state = 0;
      break;
    /////////////////
    case 4: //ruch , handler ACC break; //setup
      sound.play(4);
      saber.state = 1;
      break;
    /////////////////
    case 5://UDERZENIE
      saber.Clash();
      sound.play(5);
      saber.state = 2;
      break;
    /////////////////
    case 6://LOCK
      saber.Clash();
      sound.play(6);
      saber.state = 2;
      break;
    case 7://BLAST
      saber.Clash();
      sound.play(7);
      saber.state = 2;
      break;
    case 8://BATLVL
      //SHOW BAT LVL
      break;
    case 9://SETUP saber.color
      saber.Blink();
      sound.play(3);

      if (KEY1) {
        saber.color++;
      }
      if (KEY2) {
        saber.color--;
      }
      if (KEY1 && KEY2)
      {
        saber.state = 10;
        break;
      }

      if (t > 100000)
      {
        break;
      }
      if (t > 100000)
      {
        t = 0;
      }
      t++;

      for (a; a >= 4; a++)
      {
        EEPROM.update(a, saber.save[a]);
      }

      switch (saber.color)/
      { //R,G,B,W
        //          case 0://OFF
        //          saber.red = 0;
        //          saber.green = 0;
        //          saber.blue = 0;
        //          saber.white = 0;
        //          break;
        case 1://BLUE
          saber.red = 0;
          saber.green = 0;
          saber.blue = 255;
          saber.white = 0;
          break;
        case 2://LIGHT BLUE
          saber.red = 0;
          saber.green = 255;
          saber.blue = 255;
          saber.white = 100;
          break;
        case 3://WHITE
          saber.red = 255;
          saber.green = 255;
          saber.blue = 255;
          saber.white = 255;
          break;
        case 4://LIGHT GREEN
          saber.red = 0;
          saber.green = 255;
          saber.blue = 0;
          saber.white = 100;
          break;
        case 5://GREEN
          saber.red = 0;
          saber.green = 255;
          saber.blue = 0;
          saber.white = 0;
          break;
        case 6://BLUE-GREEN
          saber.red = 0;
          saber.green = 255;
          saber.blue = 255;
          saber.white = 100;
          break;
        case 7://YELLOW
          saber.red = 255;
          saber.green = 255;
          saber.blue = 255;
          saber.white = 0;
          break;
        case 8://AMBER
          saber.red = 250;
          saber.green = 100;
          saber.blue = 100;
          saber.white = 100;
          break;
        case 9://ORANGE
          saber.red = 255;
          saber.green = 100;
          saber.blue = 100;
          saber.white = 100;
          break;
        case 10://RED
          saber.red = 255;
          saber.green = 0;
          saber.blue = 0;
          saber.white = 0;
          break;
        case 11://PURPLE
          saber.red = 255;
          saber.green = 0;
          saber.blue = 255;
          saber.white = 0;
          break;


        default: saber.color = 1; break;
      }

    case 10://SETUP saber.flash
      //saber.state=3;
      if (KEY1) {
        saber.color++;
      }
      if (KEY2) {
        saber.color--;
      }
      if (KEY1 && KEY2)
      {
        saber.state = 10;
      }

      if (t > 100000)
      {
        break;
      }
      if (t > 100000)
      {
        t = 0;
      }
      t++;
      switch (saber.flash)
      { //R,G,B,W
        //  case 0:
        //  saber.flash = 7;
        //  break;
        case 1://BLUE
          saber.redf = 0;
          saber.greenf = 0;
          saber.bluef = 255;
          saber.whitef = 0;
          break;
        case 2://lIGHT BLUE
          saber.redf = 0;
          saber.greenf = 255;
          saber.bluef = 255;
          saber.whitef = 100;
          break;
        case 3://WHITE
          saber.redf = 0;
          saber.greenf = 0;
          saber.bluef = 0;
          saber.whitef = 255;
          break;
        case 4://LIGHT GREEN
          saber.redf = 0;
          saber.greenf = 255;
          saber.bluef = 0;
          saber.whitef = 100;
          break;
        case 5://GREEN
          saber.redf = 0;
          saber.greenf = 255;
          saber.bluef = 0;
          saber.whitef = 0;
          break;
        case 6://BLUE-GREEN
          saber.redf = 0;
          saber.greenf = 255;
          saber.bluef = 255;
          saber.whitef = 100;
          break;
        case 7://YELLOW
          saber.redf = 255;
          saber.greenf = 255;
          saber.bluef = 255;
          saber.whitef = 0;
          break;
        case 8://AMBER
          saber.redf = 250;
          saber.greenf = 100;
          saber.bluef = 100;
          saber.whitef = 100;
          break;
        case 9://ORANGE
          saber.redf = 255;
          saber.greenf = 100;
          saber.bluef = 100;
          saber.whitef = 100;
          break;
        case 10://RED
          saber.redf = 255;
          saber.greenf = 0;
          saber.bluef = 0;
          saber.whitef = 0;
          break;
        case 11://PURPLE
          saber.redf = 255;
          saber.greenf = 0;
          saber.bluef = 255;
          saber.whitef = 0;
          break;


        default: saber.flash = 3; break;
      }
    default: saber.state = 0; break;
  }

}



