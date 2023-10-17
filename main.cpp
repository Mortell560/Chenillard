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

struct chenille{
    public:
    void showChenille(){
        for (int i = 0; i < 4; i++){
            leds[i] = this->chen[(this->head+i)%4];
        }
    }

    void resetChenille(){
        for (int i = 0; i < 4; i++) {this->chen[i] = 0;}
        this->len = 0;
        this->head = 3;
        this->tail = 3;
    }

    void Shift(int dir){
        int temp;
        int d = dir ? dir : -1;
        this->head = (head+dir)%4;
        this->tail = (tail+dir)%4;

        if (head < 0) {head = head * -1;}
        if (tail < 0) {tail = tail * -1;}
    }

    void Add(int dir){
        if (this->len == 4){
            return;
        }
        if (dir){
            tail = (tail-1)%4;
        }
        else{
            tail = (tail+1)%4;
        }
        this->chen[tail] = 1;
        len++;
    }

    void Remove(int dir){
        if (this->len == 0){
            return;
        }
        this->chen[tail] = 0;
        if (dir){
            tail = (tail+1)%4;
        }
        else{
            tail = (tail-1)%4;
        }
        len--;

    }

    void setPause(bool p){
        this->stopped = p;
    }

    bool getPause(){
        return this->stopped;
    }

    private:
    int chen[4] = {0,0,0,1};
    int len = 1;
    int head = 3;
    int tail = 3;
    bool stopped = false;

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

void handlePause(chenille& c){
    t.start();
    while(X.read() || t.read()<3){
    }
    t.stop();

    if (t.read() > 3){
        ResetLeds();
        c.resetChenille();
    }
    else{
        c.setPause(true);
    }

    t.reset();
}

void handleInput(chenille& c){
    int l = L.read(), r = R.read(), u = U.read(), d = D.read(), x = X.read();
    if (x) {handlePause(c);}
    else if (l) {curr_dir = 0;}
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
      wait_ms(10);

  }
}
