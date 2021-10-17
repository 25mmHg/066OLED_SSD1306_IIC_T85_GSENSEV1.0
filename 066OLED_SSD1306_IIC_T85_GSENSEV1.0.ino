/* 
* CHEAP 2DIGIT OLED G-FORCE COUNTER FOR A SOZIAL MANUFACTORY BY 25mmHg
* Version: 1.00
*  
* for ATtiny85 with 8MHz via David A. Mellis attiny board package
* https://github.com/damellis/attiny
*               
*           3V3                   LPF 3Hz
*            |
*            |
*        .---o---.    47k     47k     47k     47k
*        |       |    ___     ___     ___     ___
*        |MMA7361|o--|___|-o-|___|-o-|___|-o-|___|-o------.
*        |       |         |       |       |       |      |
*        '---o---'        ---     ---     ---     ---     |
*            |            ---     ---     ---     ---     |
*            |             | 220n  | 220n  | 220n  | 220n |
*            |             |       |       |       |      |
*           ===           ===     ===     ===     ===     |
*           GND           GND     GND     GND     GND     |
*                                                         |
*                                                         |
*                                                         |
*           3V3                               3V3         |
*            |                                 |          |
*            |    .---------.         .--------o---.      |
*            |  -o|.-------.|o-       |   __   |   |      |
*            |  -o|| HELLO ||o-  SCL  '-o|° |o-'   |      |  Piezo
*            |  -o|| AGAIN ||o----------o|  |o-----)------'
*            |  -o||       ||o----------o|  |o-    |  ___    .---|
*            |  -o|'-------'|o-  SDA  .-o|__|o-----)-|___|---|   |
*            |  -o|         |o-       |            |      .--|   |
*            |  -o|OLED64x48|o--.     | ATTINY    --- 1k  |  '---|
*            '---o|         |o- |     |   85      ---     |
*                 '---------'   |     |       220n |      |
*                               |     |            |      |
*                              ===   ===          ===    ===
*                              GND   GND          GND    GND
*
*
*
*            <------------------- X-axis -------------------->
*
*
*                             TOOL MECHANIC
*
*                           .---.
*              .---------.  |###|
*              | G-Sensor|---------------------------.@@@@@@@
*              |         |  | o |      o      lever  |@@@@@@@@
*              | MMA7361 |-------------|-------------'@@@@@@@
*              '---------'  |###|      |
*                           |###|      |
*                           |###|   .-.|.-. linear guide
*                           |###|   | ||| |
*                           |###|   | ||| |
*                           |###|   | ||| |
*                           |###|   | ||| |
*                           |###|   | ||| |
*                           |###|   '-'-'-' blade
*                           |###|   |     | inside
*                           |###|   |  o  |
*                           |###|   |     |
*             .-------------'---'---'-----'--------------.
*             |##########################################|
*             |##########################################|
*             '------------------------------------------'
*
*                          drill stand
*
*
*    (created with AACircuit v1.28.7 beta 10/23/16 www.tech-chat.de)
*/

#include <Arduino.h>
#include <U8x8lib.h>
#include "fir.h"
#define SENSORPIN A1
#define PIEZOPIN 0

const int16_t MAX_TH = 25;
const int16_t MIN_TH = -20;
const uint8_t MAXCOUNTER = 64;

/* Constructor of U8x8 Display lib for ATtiny85 soft I2C and 64x48 OLED*/
U8X8_SSD1306_64X48_ER_SW_I2C u8x8(/*clock=*/3,/*data=*/4,/*reset=*/U8X8_PIN_NONE);

/* short Beep */
void beep()
{
  for (uint8_t i = 0; i < 100; i++)
  {
    digitalWrite(PIEZOPIN, !digitalRead(PIEZOPIN));
    delayMicroseconds(250);
  }
}


/* Big Number Font  */
void showDigit(uint8_t digit, uint8_t value)
{
  if (value == 0)
  {
    u8x8.drawString(digit, 0, " ** ");
    u8x8.drawString(digit, 1, "*  *");
    u8x8.drawString(digit, 2, "*  *");
    u8x8.drawString(digit, 3, "*  *");
    u8x8.drawString(digit, 4, "*  *");
    u8x8.drawString(digit, 5, " ** ");
  }
  else if (value == 1)
  {
    u8x8.drawString(digit, 0, "  * ");
    u8x8.drawString(digit, 1, " ** ");
    u8x8.drawString(digit, 2, "* * ");
    u8x8.drawString(digit, 3, "  * ");
    u8x8.drawString(digit, 4, "  * ");
    u8x8.drawString(digit, 5, " ***");
  }
  else if (value == 2)
  {
    u8x8.drawString(digit, 0, " ** ");
    u8x8.drawString(digit, 1, "*  *");
    u8x8.drawString(digit, 2, "   *");
    u8x8.drawString(digit, 3, "  * ");
    u8x8.drawString(digit, 4, " *  ");
    u8x8.drawString(digit, 5, "****");
  }
  else if (value == 3)
  {
    u8x8.drawString(digit, 0, " ** ");
    u8x8.drawString(digit, 1, "   *");
    u8x8.drawString(digit, 2, "  * ");
    u8x8.drawString(digit, 3, "   *");
    u8x8.drawString(digit, 4, "*  *");
    u8x8.drawString(digit, 5, " ** ");
  }
  else if (value == 4)
  {
    u8x8.drawString(digit, 0, "  * ");
    u8x8.drawString(digit, 1, " *  ");
    u8x8.drawString(digit, 2, "* * ");
    u8x8.drawString(digit, 3, "****");
    u8x8.drawString(digit, 4, "  * ");
    u8x8.drawString(digit, 5, "  * ");
  }
  else if (value == 5)
  {
    u8x8.drawString(digit, 0, " ***");
    u8x8.drawString(digit, 1, " *  ");
    u8x8.drawString(digit, 2, " ** ");
    u8x8.drawString(digit, 3, "   *");
    u8x8.drawString(digit, 4, "*  *");
    u8x8.drawString(digit, 5, " ** ");
  }
  else if (value == 6)
  {
    u8x8.drawString(digit, 0, " ** ");
    u8x8.drawString(digit, 1, "*   ");
    u8x8.drawString(digit, 2, "*** ");
    u8x8.drawString(digit, 3, "*  *");
    u8x8.drawString(digit, 4, "*  *");
    u8x8.drawString(digit, 5, " ** ");
  }
  else if (value == 7)
  {
    u8x8.drawString(digit, 0, "****");
    u8x8.drawString(digit, 1, "   *");
    u8x8.drawString(digit, 2, "  * ");
    u8x8.drawString(digit, 3, " *  ");
    u8x8.drawString(digit, 4, " *  ");
    u8x8.drawString(digit, 5, " *  ");
  }
  else if (value == 8)
  {
    u8x8.drawString(digit, 0, " ** ");
    u8x8.drawString(digit, 1, "*  *");
    u8x8.drawString(digit, 2, " ** ");
    u8x8.drawString(digit, 3, "*  *");
    u8x8.drawString(digit, 4, "*  *");
    u8x8.drawString(digit, 5, " ** ");
  }
  else if (value == 9)
  {
    u8x8.drawString(digit, 0, " ** ");
    u8x8.drawString(digit, 1, "*  *");
    u8x8.drawString(digit, 2, " ***");
    u8x8.drawString(digit, 3, "   *");
    u8x8.drawString(digit, 4, "   *");
    u8x8.drawString(digit, 5, " ** ");
  }
  else
  {
    u8x8.drawString(digit, 0, " ** ");
    u8x8.drawString(digit, 1, "*  *");
    u8x8.drawString(digit, 2, "  * ");
    u8x8.drawString(digit, 3, "  * ");
    u8x8.drawString(digit, 4, "    ");
    u8x8.drawString(digit, 5, "  * ");
  }
}

/* Sampling + HPF */
int16_t readSensor()
{
  while (millis() & 63); //wait 64ms = 16Hz samplerate
  return (fir_filt(analogRead(SENSORPIN)));
}

/* Threshold prozessing  */
bool armIsNotMoved()
{
  uint16_t counter = MAXCOUNTER;
  bool isNotMoved = true;
  int16_t temp = readSensor();
  while ((temp >= MAX_TH) || ((counter > 0) &&  (counter < MAXCOUNTER )))
  {
    counter--;
    if (temp < MIN_TH)
    {
      isNotMoved = false;
      break;
    }
    temp = readSensor();
  }
  return (isNotMoved);
}

void setup(void)
{
  pinMode(PIEZOPIN, OUTPUT);
  u8x8.begin();
  u8x8.setFont(u8x8_font_saikyosansbold8_u);
  u8x8.drawString(1, 2, "HELLO");
  u8x8.drawString(1, 4, "AGAIN");
  fir_init();
  for (uint16_t i = 0; i < 50; i++) readSensor();
  u8x8.clear();
}

void loop(void)
{
  for (uint8_t i = 0; i < 51; i++)
  {
    if (i / 10) showDigit(0, i / 10);
    showDigit(4, i % 10);
    while (armIsNotMoved());
    beep();
    u8x8.clear();
  }
}
