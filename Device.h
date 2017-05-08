/*
  Library for LIGHTSABER - DEVICE class v0.1
  Author: Paweł Krystkiewicz
  Created: 22.04.2017
  Released:-
*/
#ifndef Device_h
#define Device_h

#include "Arduino.h"

//DEFINICJA WYJŚĆ PWM

#define R 10
#define G 13
#define B 14
#define W 15






class Device
{
  public:    
    
    //ZMIENNE
    byte red, green, blue, white;
    byte redf, greenf, bluef, whitef;
    byte i = 0, j = 0, k = 0;
    byte color, state, flash;
    byte a=120;
    //TABELE
    byte channel[4] = {R, G, B, W};
    byte slope[19] = {0, 2, 4, 5, 7, 9, 11, 15, 18, 20, 25, 30, 36, 41, 45, 50, 55, 60, 67};
    byte flicker[22] = {230, 225, 220, 218, 210, 205, 200, 190, 185, 180, 170, 170, 180, 185, 190, 200, 205, 210, 218, 220, 225, 230};
    byte CurrentBladeColor[4] = {red, green, blue, white};
    byte CurrentFlashColor[4] = {redf, greenf, bluef, whitef};
	byte save[4];
	byte volume, addon;

    //FUNKCJE
    void On();
    void Off();
    void Blink();
    void Clash();
    void Idle();
    
       
   };

void Device::On()
{
  if (red > a)
  {
    for (j; j <= sizeof(slope); ++j)
    {
      analogWrite(R, slope[j]);
    }
  }

  if (green > a)
  {
    for (j; j <= sizeof(slope); ++j)
    {
      analogWrite(G, slope[j]);
    }
  }

  if (blue > a)
  {
    for (j; j <= sizeof(slope); ++j)
    {
      analogWrite(B, slope[j]);
    }
  }
  if (white > a)
  {
    for (j; j <= sizeof(slope); ++j)
    {
      analogWrite(W, slope[j]);
    }
  }
}

void Device::Off()
{
  if (red > 0)
  {
    for (j; j >= 0; --j)
    {
      analogWrite(R, slope[j]);
    }
  }

  if (green > 0)
  {
    for (j; j >= 0; --j)
    {
      analogWrite(G, slope[j]);
    }
  }

  if (blue > 0)
  {
    for (j; j >= 0; --j)
    {
      analogWrite(B, slope[j]);
    }
  }
  if (white > 0)
  {
    for (j; j >= 0; --j)
    {
      analogWrite(W, slope[j]);
    }
  }
}

void Device::Blink()
{

  if (red > a)
  {
    for (k; k <= sizeof(flicker); ++k)
    {
      analogWrite(R, flicker[k]);
    }
  }

  if (green > a)
  {
    for (k; k <= sizeof(flicker); ++k)
    {
      analogWrite(G, flicker[k]);
    }
  }

  if (blue > a)
  {
    for (k; k <= sizeof(flicker); ++k)
    {
      analogWrite(B, flicker[k]);
    }
  }
  if (white > a)
  {
    for (k; k <= sizeof(flicker); ++k)
    {
      analogWrite(W, flicker[k]);
    }
  }
}

void Device::Clash()
{
  for (i; i <= 4; ++i)
  {
    analogWrite(channel[i], redf);
    analogWrite(channel[i], greenf);
    analogWrite(channel[i], bluef);
    analogWrite(channel[i], whitef);
  }
}
void Device::Idle()
{
  for (i; i <= 4; ++i)
  {
    analogWrite(channel[i], 0);
    analogWrite(channel[i], 0);
    analogWrite(channel[i], 0);
    analogWrite(channel[i], 0);
  }
}


#endif
