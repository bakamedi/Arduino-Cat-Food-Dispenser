#include <Servo.h>
#include <Keypad.h>
#include <Ultrasonic.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // LCD arduino library
Ultrasonic ultrasonic(10,8); // (Trig PIN,Echo PIN)
Servo servo;  // Crea un Objeto servo

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
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

void setup() {
lcd.begin(16, 2);
servo.attach(9);
lcd.setCursor(0, 0);
lcd.print("INCIANDO...");
//Inicializamos la comunicacion por Serial
Serial.begin(9600);
}

void loop(){
  int cm;
  //Lo llevamos a un extremo  
  //servo.write(0);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  cm = ultrasonic.Ranging(CM);
  lcd.print(ultrasonic.Ranging(CM)); // CM or INC
  lcd.print("cm");
  if(cm > 5){
    //Lo volvemos a centrar  
    servo.write(90);
    //Le damos tiempo a llegar a esa posicion  
    delay(1000);
  }else{
    //Lo volvemos a centrar  
    servo.write(0);
    //Le damos tiempo a llegar a esa posicion  
    delay(1000);
  }
  
}