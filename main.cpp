#include "mbed.h"
#include "C12832.h"
#include "mbed2/299/platform/wait_api.h"
#include <algorithm>

/* Inputs */
DigitalIn U(p15);
DigitalIn D(p12);
DigitalIn L(p13);
DigitalIn R(p16);
DigitalIn X(p14);
Timer t;

/* Outputs */
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

C12832 lcd(p5, p7, p6, p8, p11);
DigitalOut leds[4]={led1, led2, led3, led4};
char* dirs[2] = {"Left", "Right"};
int curr_dir = 0;
int val = 1;


void ResetLeds(){
    // Eteint toutes les leds
    for (int i = 0; i < 4; i++){
        leds[i] = 0;
    }
}

void DisplayIntLeds(int j){
    // Allume les leds en fonction de leur representation binaire
    for (int i=0; i<4; i++){
        leds[i]=j%2;
        j/=2;
    }
}

void ShiftRight(){

}

void showInfoInputs(){
    lcd.locate(0,lcd.height()-4);
    lcd.printf("%i", U.read());

    lcd.locate(lcd.width()/5, lcd.height()-4);
    lcd.printf("%i", D.read());

    lcd.locate(lcd.width()/5*2, lcd.height()-4);
    lcd.printf("%i", L.read());

    lcd.locate(lcd.width()/5*3, lcd.height()-4);
    lcd.printf("%i", R.read());

    lcd.locate(lcd.width()/5*4, lcd.height()-4);
    lcd.printf("%i", X.read());

    lcd.locate(0,0);
    lcd.printf("current direction: %s", dirs[curr_dir]);
}

int main() {
    ResetLeds();
    DisplayIntLeds(val);
  while(1) {
      lcd.cls();
      if (L.read()) {curr_dir = 0;}
      else if (R.read()) {curr_dir = 1;}
      showInfoInputs();
      wait_ms(10);

  }
}
