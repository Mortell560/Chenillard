#include "mbed.h"
#include "C12832.h"
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

int chen[4] = {0,0,0,1};
int len = 1;
int head = 3;
int tail = 3;
bool stopped = false;

struct chenille{
    public:
    void showChenille(){
        for (int i = 0; i < 4; i++){
            leds[i] = chen[i];
        }
    }

    void resetChenille(){
        for (int i = 0; i < 4; i++) {chen[i] = 0;}
        len = 0;
        head = 3;
        tail = 3;
    }

    void Shift(int dir){
        int temp;
        int d = dir == 0 ? -1 : 1;
        if (!dir){
            temp = chen[0];
            for (int i = 0; i < 3; i++){
                chen[i] = chen[i+1];
            }
            chen[3] = temp;
        }
        else{
            temp = chen[3];
            for (int i = 3; i >= 0; i--){
                chen[i+1] = chen[i];
            }
            chen[0] = temp;
        }
        head = (head+d)%4;
        tail = (tail+d)%4;

        // safe guard
        if (head < 0) {head = 4 + head;}
        if (tail < 0) {tail = 4 + tail;}
        
    }

    void Add(int dir){
        if (len == 3){
            return;
        }
        if (dir){
            if (tail==0){
                tail=4;
            }
            tail = tail-1;
        }
        else{
            tail = (tail+1)%4;
        }
        chen[tail] = 1;
        len++;
    }

    void Remove(int dir){
        if (len == 1){
            return;
        }
        chen[tail] = 0;
        if (dir){
            tail = (tail+1)%4;
        }
        else{
            if (tail==0){
                tail=4;
            }
            tail = tail-1;
        }
        len--;

    }

    void setPause(bool p){
        stopped = p;
    }

    bool getPause(){
        return stopped;
    }

};


void ResetLeds(){
    // Eteint toutes les leds
    for (int i = 0; i < 4; i++){
        leds[i] = 0;
    }
}

/*
void DisplayIntLeds(int j){
    // Allume les leds en fonction de leur representation binaire
    for (int i=0; i<4; i++){
        leds[i]=j%2;
        j/=2;
    }
}
*/

void showInfoInputs(){
    lcd.locate(0,12);
    lcd.printf("U: %i", U.read());

    lcd.locate(lcd.width()/5, 12);
    lcd.printf("D: %i", D.read());

    lcd.locate(lcd.width()/5*2, 12);
    lcd.printf("L: %i", L.read());

    lcd.locate(lcd.width()/5*3, 12);
    lcd.printf("R: %i", R.read());

    lcd.locate(lcd.width()/5*4, 12);
    lcd.printf("X: %i", X.read());
    
    //debug
    //lcd.locate(0, 20);
    //lcd.printf("tail: %i, head: %i, len: %i", tail, head, len);
    //lcd.printf("%i %i %i %i", chen[0], chen[1], chen[2], chen[3]);

    lcd.locate(0,0);
    lcd.printf("current direction: %s", dirs[curr_dir]);
}

void handlePause(chenille& c){
    t.start();
    while(X.read()){
    }
    t.stop();

    if (t.read() > 3){
        ResetLeds();
        c.resetChenille();
    }
    else{
        if (c.getPause()){c.setPause(false);}
        else{c.setPause(true);};
    }

    t.reset();
}

void handleInput(chenille& c){
    int l = L.read(), r = R.read(), u = U.read(), d = D.read(), x = X.read();
    if (x) {handlePause(c);}
    if (l) {curr_dir = 0;}
    else if (r) {curr_dir = 1;}
    else if (u) {c.Add(curr_dir);}
    else if (d) {c.Remove(curr_dir);}

}

int main() {
    ResetLeds();
    chenille c;
  while(1) {
      lcd.cls();
      handleInput(c);
      if (!c.getPause()) {c.Shift(curr_dir);}
      showInfoInputs();
      c.showChenille();
      wait_ms(1000);

  }
}
