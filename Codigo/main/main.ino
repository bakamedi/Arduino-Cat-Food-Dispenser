#include <Servo.h>
#include <Keypad.h>
#include <Ultrasonic.h>
#include <LiquidCrystal.h>
#include <Thread.h>
#include <NewList.h>
#include <String.h>
//#include <DueTimer.h>

Thread myThread = Thread();
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
int cont = 0;
char tiempoEscodigo[6];
char h1[2] = {0};
char h2[2] = {0};
char m1[2] = {0};
char m2[2] = {0};
//h1h2:m1m2
long int horaTotalSegundo = 0;
long int minTotalSegundo = 0;

void setup() {
  lcd.begin(16, 2);
  servo.attach(9);
  lcd.print(tiempo);
 
  //delay(2000);
  lcd.clear();
  myThread.onRun(abreServo);
  myThread.setInterval(3000); //5000 segundos
  //Inicializamos la comunicacion por Serial
  Serial.begin(9600);
}

void abreServo(){
  servo.write(90);
  delay(500);
  servo.write(0);
}

int calculaHoraAsegundo(int hora){
  return hora * 360 * 1000;
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
      //lcd.print(key);
      lcd.clear();
        if(tiempo[cont]==':'){
          cont++;
        }
        
        tiempo[cont] = key;
        cont++;
        Serial.print(tiempo);
        Serial.print("\n");
        lcd.print(tiempo);
  }

  if(cont == 6){
    //coje el valor del teclado
    h1[0] = tiempo[0];
    h2[0] = tiempo[1];
    m1[0] = tiempo[3];
    m2[0] = tiempo[4];
    char combined[30] = {0};/*
    if(h1[0] == '0'){
      char vacio[1] = {0};
      strcat(combined, vacio);
    }
    if(h2[0]){
      
    }
    else{
      strcat(combined, h1);
    }
    */
    
    strcat(combined, h2);
    String hora = String(combined);
    lcd.begin(0, 0);
    horaTotalSegundo = hora.toInt()*1000*360;
    //minTotalSegundo = minTotalSegundo.toInt()*10000*60;
    Serial.print(horaTotalSegundo);
    //delay(5000);
    
    Serial.print("\n");
    lcd.print(horaTotalSegundo);
  }
  
  if(myThread.shouldRun()){
      myThread.run();
  }
}