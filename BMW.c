#include "BMW.h"


uint8_t DATA7[15]={0x01,0x8B,0x04,0x88,0x07,0x81,0x0E,0x82,0x0D,0x87,0x08,0x84,0x8D,0x02,0x8E};
uint8_t DATA_7_Index = 0;
uint8_t DATA_4 = 0x30;


uint8_t A = 0, B = 1, C = 0;; 

uint8_t ENC_BUTTON = 0,ENC_BUTTON_OLD = 0;

void Create_Message(uint8_t* DATA, uint16_t ID)
{
  DATA[0]=DATA[1]=DATA[2]=DATA[4]=DATA[5]=DATA[7]=DATA[8]=0;
  DATA[3]=0xFC;
  DATA[6]=0x1F;
  
  DATA[4]=DATA_4;
  DATA_4++;
  if(DATA_4==0x3F)
  {
    DATA_4=0x30;
  }
  
  DATA[7]=DATA7[DATA_7_Index];
  DATA_7_Index++;
  if(DATA_7_Index==15)
  {
    DATA_7_Index=0;
  }
  DATA[5]=TIM2->CNT/4;
  //**********************************//T_CANCEL
  if ((GPIOB->IDR & (1<<12)) == 0)
  {
    DATA[1]+=1;
  }
  //**********************************//T_LEFT          PB13
  if ((GPIOB->IDR & (1<<13)) == 0)
  {
   DATA[1]+=0x10;
  }
  //**********************************//T_RIGHT         PB14
  if ((GPIOB->IDR & (1<<14)) == 0)
  {
    DATA[1]+=0x20; 
  }
  //**********************************//SPEED_EN        PB15
  if ((GPIOB->IDR & (1<<15)) == 0)
  {
    DATA[1]+=0x08;
  }
  //**********************************//SPEED_SET       PA9
  if ((GPIOA->IDR & (1<<9)) == 0)
  {
    DATA[2]+=0x08;
  }
  //**********************************//SPEED_RESET     PA8
  if ((GPIOA->IDR &(1<<8)) == 0)
  {
    DATA[2]+=0x04;
  }
  //**********************************//ABS             PA10
  if ((GPIOA->IDR & (1<<10)) == 0)
  {
    DATA[1]+=0x02;
  }
  //**********************************//SUSPENTION      PA11
  if ((GPIOA->IDR & (1<<11)) == 0)
  {
    DATA[1]+=0x01;
  }
  //**********************************//HAZARD          PA12
  if ((GPIOA->IDR & (1<<12)) == 0)
  {
    DATA[1]+=0x04;
  }
  //**********************************//LIGHT           PA15
  if ((GPIOA->IDR & (1<<15)) == 0)
  {
    DATA[2]+=0x02;
  }
  //**********************************//HORN            PB3
  if ((GPIOB->IDR & (1<<8)) == 0)
  {
    DATA[1]+=0x80;
  }
  //**********************************//WHEEL_UP        PB4
  if ((GPIOB->IDR & (1<<4)) == 0)
  {
    DATA[3]+=2;
  }
  //**********************************//WHEEL_DOWN      PB5
  if ((GPIOB->IDR & (1<<5)) == 0)
  {
    DATA[3]+=1;
  }  
  //**********************************//MENU_UP         PB6
  if ((GPIOB->IDR & (1<<6)) == 0)
  {
    DATA[2]+=0x20;
  } 
  //**********************************//MENU_DOWN       PB7
  if ((GPIOB->IDR & (1<<7)) == 0)
  {
    DATA[2]+=0x10;
  }
  //uint8_t ENC_BUTTON = 0,ENC_BUTTON_OLD = 0;
  
  if ((GPIOC->IDR & (1<<13)) == 0)
  {
    ENC_BUTTON = 1;
  }
  else
  {
    ENC_BUTTON = 0;
  }
  if(ENC_BUTTON!=ENC_BUTTON_OLD)
  {
    ENC_BUTTON_OLD = ENC_BUTTON;
    if(ENC_BUTTON==1)
    {
      DATA[3]+=2;
    }
  }
  
  
  for (uint8_t i = 0; i < 8; i++)
  {
    ID += DATA[i];
    if (ID >= 256)
    {
      ID -= 255;
    }
  }
  DATA[8]=~ID & 0xFF;;
}