#include <Servo.h>
#include <Keypad.h>
#include <Ultrasonic.h>
#include <LiquidCrystal.h>
#include "TimerObject.h"
TimerObject *timer1 = new TimerObject(2000);
TimerObject *timer2 = new TimerObject(3000);

LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // LCD arduino library
Ultrasonic ultrasonic(10,8); // (Trig PIN,Echo PIN)
Servo servo;  // Crea un Objeto servo
//Teclado
const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','*'}
};
byte rowPins[rows] = {A0, A1, A2, A3}; //connect to the row pinouts of the keypad
byte colPins[cols] = {A4, A5, 11}; //connect to the column pinouts of the keypad
Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );
char tiempo[6]="00:00";


void setup() {
lcd.begin(16, 2);
servo.attach(9);
lcd.print(tiempo);
delay(1000);
lcd.clear();
//Inicializamos la comunicacion por Serial
Serial.begin(9600);
}

void Puerta1(){
  servo.write(90);
}
void Puerta2(){
  servo.write(0);
}

void loop(){
  int cm;
  
  //t.update();
  //Lo llevamos a un extremo  
  //servo.write(0);
  //cm = ultrasonic.Ranging(CM);
  //lcd.print(ultrasonic.Ranging(CM)); // CM or INC
  //lcd.print("cm");
  char key = myKeypad.getKey();
  if (key=='1'||key=='2'||key=='3'||key=='4'||key=='5'||key=='6'||key=='7'||key=='8'||key=='9'||key=='0'){
      lcd.print(key);
      for(int i = 0;i < 6;i++){
        if(tiempo[i]!=':'){
          tiempo[i]=key;
        }
      }
  }/*
  if(cm > 5){
    //Lo volvemos a centrar  
    //Le damos tiempo a llegar a esa posicion  
    //t.pulse(9, 0.1 * 60 * 1000, HIGH); // 5 segundos
    //servo.write(90);
    //delay(1000);
    timer1->setOnTimer(&Puerta1);
    timer1->Start();
    timer2->Stop();
    timer2->Update();
  }else{
    //Lo volvemos a centrar  
    
    //Le damos tiempo a llegar a esa posicion  
    //t.pulse(9, 0.3 * 60 * 1000, HIGH); // 5 segundos
    //servo.write(0);
    timer2->setOnTimer(&Puerta2);
    timer2->Start();
    timer1->Stop();
    timer1->Update();
  }
  timer1->Update();
  timer2->Update();*/
  
}
