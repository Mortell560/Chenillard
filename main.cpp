#include "mbed.h"
#include "C12832.h"

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

int main() {
  while(1) {

  }
}
