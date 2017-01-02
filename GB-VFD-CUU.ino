#include <CUU_Interface.h>
#include <CUU_Serial.h>
#include <Noritake_VFD_CUU.h>
#include <util/delay.h>

CUU_Serial interface(3, 4, 5); // SIO, STB, SCK

Noritake_VFD_CUU vfd;

int           last;
char          pos;

static  byte c12[8] = {
  B01110,
  B10101,
  B10101,
  B10101,
  B10001,
  B10001,
  B01110,
};

static  byte c3[8] = {
  B01110,
  B10001,
  B10001,
  B10111,
  B10001,
  B10001,
  B01110,
};

static  byte c6[8] = {
  B01110,
  B10001,
  B10001,
  B10101,
  B10101,
  B10101,
  B01110,
};

static  byte c9[8] = {
  B01110,
  B10001,
  B10001,
  B11101,
  B10001,
  B10001,
  B01110,
};

static  byte heart[8] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100,
 };

static  byte ghostup[8] = {
  B01110,
  B10001,
  B10001,
  B10101,
  B01010,
};

static  byte ghostdn[8] = {
  B00000,
  B01110,
  B10001,
  B10001,
  B10101,
  B01010,
};

void
setup()
{
  _delay_ms(500);      // wait for device to power up
  vfd.begin(16, 2);    // 20x2 character module
  vfd.interface(interface); // select which interface to use
  
  // Uncomment if the target module supports brightness boost
  //vfd.brightnessBoost(); // module has brightness boost
 
  vfd.CUU_init();      // initialize module
  vfd.CUU_brightness(1);

  vfd.CUU_createChar(0, c12);
  vfd.CUU_createChar(1, c3);
  vfd.CUU_createChar(2, c6);
  vfd.CUU_createChar(3, c9);
  vfd.CUU_createChar(4, heart);
  vfd.CUU_createChar(5, ghostup);
  vfd.CUU_createChar(6, ghostdn);

  srandom(analogRead(2));
  pos = 0;      
}

void
loop()
{
  int       m = millis() / 1000;
  boolean   leading;
  uint16_t  val;
  uint16_t  digit;

  if (m != last) {
    leading = false;
    vfd.CUU_clearScreen();
    vfd.CUU_setCursor(0, 0);
    vfd.print("CP, P Set=");
    val = random() % 1000;
    digit = val / 100;
  
    if (digit == 0) {
      vfd.print(' ');
      leading = true;
    } else {
      vfd.print(digit + '0');
    }
  
    val -= digit * 100;
    digit = val / 10;
  
    if (leading && (digit == 0)) {
      vfd.print(' ');
    } else {
      vfd.print(digit + '0');
    }
  
    val -= digit * 10;
    vfd.print(val + '0');
    vfd.print(".");
    vfd.print((random() % 10) + '0');
    //vfd.print("V");
    vfd.CUU_setCursor(15, 0);
    vfd.print((char) ((random() % 3) + 4));
    vfd.CUU_setCursor(0, 1);
    vfd.print("LOAD V=");
    val = random() % 100;
    digit = val / 10;

    if (digit == 0) {
      vfd.print(' ');
    } else {
      vfd.print(digit + '0');
    }
  
    val -= digit * 10;
    vfd.print(val + '0');
    vfd.print(".");
     val = random() % 100;
    digit = val / 10;
    vfd.print(digit + '0');
    val -= digit * 10;
    vfd.print(val + '0');
    vfd.CUU_setCursor(13, 1);
    //vfd.print((char) (random() % 0x100));
    vfd.print(pos);
    pos += 1;
    if (pos > 3) {
      pos = 0;
    }
    last = m;  // remember current time
  }
}
