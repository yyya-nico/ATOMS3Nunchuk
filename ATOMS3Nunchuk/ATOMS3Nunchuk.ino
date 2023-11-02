#include <M5AtomS3.h>
#include "Wire.h"
#include <BleMouse.h>

BleMouse bleMouse("Nunchuck Mouse", "Created by yyya_nico");

#define INITIAL_COLOR 0x080000
#define NUNCHK_ADDR (0x52)
#define COMPLETE_COUNT (5)
#define NUNCHK_Z_MASK 0x01
#define NUNCHK_C_MASK 0x02
#define POSITION_MARGIN 3

static int initXposi = 0;
static int initYposi = 0;
static int intervalCount = 0;
static bool disableCount = false;
static bool disableScroll = false;

void setup() {
  bleMouse.begin();
  initNunchuk();
  M5.begin(false, true, false, true);
  M5.dis.drawpix(INITIAL_COLOR);
  M5.dis.show();
}

void loop() {
  int     x      = 0;
  int     y      = 0;
  uint8_t button = 0;
  if(bleMouse.isConnected()){
    M5.dis.drawpix(0x000026);
    M5.dis.show();
    if(nunchuckIsAvailable(&x, &y, &button) ){
      int xPosi = x - initXposi;
      int yPosi = y - initYposi;

      if(button & NUNCHK_C_MASK) { // c button
        if(intervalCount >= 1000/10) {
          bleMouse.press(MOUSE_RIGHT);
          intervalCount = 0;
          disableCount = true;
          disableScroll = true;
          delay(10);
          bleMouse.release(MOUSE_RIGHT);
        }
        else {
          if(abs(xPosi) > POSITION_MARGIN || abs(yPosi) > POSITION_MARGIN) {
            intervalCount = 0;
            disableCount = true;
            if(yPosi > POSITION_MARGIN && !disableScroll) {
              bleMouse.move(0, 0, 1);
            }
            else if(yPosi < -POSITION_MARGIN && !disableScroll) {
              bleMouse.move(0, 0, -1);
            }
            int delay1 = max(-3 * abs(yPosi) + 240, 0);
            if(xPosi > POSITION_MARGIN && !disableScroll) {
              bleMouse.move(0, 0, 0, 1);
            }
            else if(xPosi < -POSITION_MARGIN && !disableScroll) {
              bleMouse.move(0, 0, 0, -1);
            }
            int delay2 = max(-3 * abs(xPosi) + 240, 0);
            delay(min(delay1, delay2));
          }
          else if(!disableCount) {
            intervalCount++;
          }
        }
      }
      else {
        if(abs(xPosi) > POSITION_MARGIN || abs(yPosi) > POSITION_MARGIN) {
          bleMouse.move((signed char)(xPosi)*50/127, -(signed char)(yPosi)*50/127); // stick position x y
        }
        intervalCount = 0;
        disableCount = false;
        disableScroll = false;
      }

      if(button & NUNCHK_Z_MASK) { // z button 
        bleMouse.press(MOUSE_LEFT);
      }
      else {
        bleMouse.release(MOUSE_LEFT);
      }
    }
    else {
      bleMouse.release(MOUSE_ALL);
    }
  } else {
    M5.dis.drawpix(INITIAL_COLOR);
    M5.dis.show();
  }
  delay(10);
}

void initNunchuk(void) {
  uint8_t     dummy = 0;

  Wire.begin();
  Wire.beginTransmission(NUNCHK_ADDR);
  Wire.write((uint8_t)0x40);
  Wire.write((uint8_t)0x00);
  Wire.endTransmission();
  getInitPosition(&initXposi, &initYposi, &dummy);
  delay(5);
  getInitPosition(&initXposi, &initYposi, &dummy);
}

char decodeNunchukData (char x)
{
    x = (x ^ 0x17) + 0x17;
    return x;
}

boolean nunchuckIsAvailable(int *x, int *y, uint8_t *button)
{
  static uint8_t nunchuck_buf[6];
  int cnt = 0;

  Wire.requestFrom (NUNCHK_ADDR, 6);
  while (Wire.available ()) {
      nunchuck_buf[cnt] = decodeNunchukData( Wire.read() );
      cnt++;
  }
  
  Wire.beginTransmission(NUNCHK_ADDR);
  Wire.write((uint8_t)0x00);
  Wire.endTransmission();
  
  if(cnt >= COMPLETE_COUNT){
    *button = 0;
    if(!(nunchuck_buf[5] & 0x01)) *button  = NUNCHK_Z_MASK;    // z_button 
    if(!(nunchuck_buf[5] & 0x02)) *button |= NUNCHK_C_MASK;   // c_button  
    *x = nunchuck_buf[0];
    *y = nunchuck_buf[1];
    return true;
  }
  else{
    return false;
  }
}

void getInitPosition(int *x, int *y, uint8_t *button)
{
    nunchuckIsAvailable(x, y, button);
}

