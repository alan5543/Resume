#define LED_1 3 //define 1st LED pin
#define LED_2 8 //define 2nd LED 6->8
#define LED_3 9 //define 3rd LED 7->9

static const unsigned char PROGMEM logo_bmp[] =
{ 
0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0XFF,0xFF,0XF8,0X7F,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0XFF,0XFF,0XF8,0X3F,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0XFE,0XFF,0XF8,0X3F,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0XFE,0X1F,0XFC,0X7F,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0XFC,0X03,0XFD,0XFF,0XFF,0X0F,0XFF,0XFF,0XFF,0XF0,0X00,0X0F,0XFF,0XFF,0XFF,0XF0,
0XE4,0X00,0XFB,0XFF,0XFF,0X0F,0XFF,0XFF,0XFF,0XF0,0X00,0X0F,0XFF,0XFF,0XFF,0XF0,
0XE4,0X00,0X1B,0XFF,0XFF,0X0F,0XFF,0XFF,0XFF,0XF0,0X00,0X0F,0XFF,0XFF,0XFF,0XF0,
0XCC,0X00,0X03,0XFF,0XFF,0X0F,0XFF,0XFF,0XFF,0XF0,0X00,0X0F,0XFF,0XFF,0XFF,0XF0,
0XC8,0X7C,0X00,0XFF,0XFF,0X0F,0XFF,0XFF,0XFF,0XF0,0X00,0X0F,0XFF,0XFF,0XFF,0XF0,
0XC8,0XFE,0X00,0X1F,0XFF,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0X99,0XFF,0X00,0X07,0XFF,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0X91,0XFF,0X00,0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0X91,0XFF,0X00,0X00,0X1F,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0XE1,0XFF,0X00,0X00,0X1F,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0XE0,0XFE,0X01,0X80,0X1F,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0XE0,0X7C,0X03,0XE0,0X3F,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0XC0,0X38,0X07,0XE0,0X3F,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0XF0,0X00,0X07,0XF0,0X4F,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0XFE,0X00,0X07,0XE0,0X4F,0X00,0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,
0XC7,0XC0,0X07,0XE0,0X9F,0X00,0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,
0X81,0XF0,0X03,0XC0,0X9F,0X00,0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,
0X80,0X3E,0X00,0X01,0X9F,0X00,0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,
0XC0,0X07,0X80,0X01,0X3F,0X00,0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,
0XF8,0X01,0XF0,0X01,0X31,0X00,0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,
0XBF,0X00,0X3E,0X03,0XF1,0X00,0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,
0X8F,0XC0,0X0F,0X83,0XF1,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0XCC,0XE0,0X01,0XF7,0XF3,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0XCC,0X1C,0X00,0X3F,0XF1,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0X8D,0XC7,0XC0,0X0F,0XF0,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0X8E,0X38,0XF8,0X0F,0XF8,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0X83,0X8E,0X1E,0X0F,0XFF,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0X80,0X71,0XC7,0XDF,0XF3,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0X80,0X1C,0X67,0XFF,0XF9,0x00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0X81,0XE3,0X8C,0X7F,0XC7,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0X81,0XFC,0XF8,0X1F,0X83,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,
0X80,0XFE,0X13,0X1C,0X47,0X0F,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X00,0XFF,0X00,0X00,
0X80,0X3E,0X00,0X24,0X6F,0X0F,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X00,0XFF,0X00,0X00,
0X83,0X3E,0X00,0X64,0X7F,0X0F,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X00,0XFF,0X00,0X00,
0X83,0X70,0X00,0X47,0XFF,0X0F,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X00,0XFF,0X00,0X00,
0X87,0XF0,0X00,0XC7,0XFF,0X0F,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X00,0XFF,0X00,0X00,
0X87,0XF0,0X00,0X8F,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X81,0XE0,0X01,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X80,0X60,0X03,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X80,0X00,0X03,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X80,0X00,0X67,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00, };

//************************************************************************************
//LED control
void turn_off(){
  digitalWrite(LED_1,LOW);
  digitalWrite(LED_2,LOW);
  digitalWrite(LED_3,LOW);
}

void first_LED(){
  digitalWrite(LED_1,HIGH);
}

void middle_LED_Blink(){
  digitalWrite(LED_2, HIGH);
  delay(200);
  digitalWrite(LED_2,LOW);
  delay(200);
}

void three_LED_on(){
  digitalWrite(LED_1,HIGH);
  digitalWrite(LED_2,HIGH);
  digitalWrite(LED_3,HIGH);
}

void three_blink(){
  for(int i = 0;i<3;i++){
    digitalWrite(LED_1,HIGH);
    digitalWrite(LED_2,HIGH);
    digitalWrite(LED_3,HIGH);
    delay(200);
    digitalWrite(LED_1,LOW);
    digitalWrite(LED_2,LOW);
    digitalWrite(LED_3,LOW);
    delay(200);
   }
}
