#include <Servo.h>
#include <Keypad.h>
#include <Ultrasonic.h>
#include <LiquidCrystal.h>
#include <Thread.h>
#include <NewList.h>
#include <String.h>
#include <SoftwareSerial.h>
#include <NewPing.h>

#define RxD 0
#define TxD 1
#define RST 5

SoftwareSerial BTSerial(RxD,TxD);
//TimedAction timedAction1;
Thread myThread = Thread();
//LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // LCD arduino library
//ultrasonido
Ultrasonic ultraComidaNotifica(10,8);// (Trig PIN,Echo PIN)
Ultrasonic ultraAguaNotifica(6,7);// (Trig PIN,Echo PIN)
Ultrasonic ultraElectroValvula(4,5);// (Trig PIN,Echo PIN)
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
String tiempoHoraMinBlueTooth;
int long tiempoEsperaServo = 10000;//3 segundos
char tiempo[6]="00:00";
char tiempoBluetooth[10] ={0};
int cont = 0;
char tiempoEscodigo[6];
char h1[2] = {0};
char h2[2] = {0};
char m1[2] = {0};
char m2[2] = {0};
//h1h2:m1m2
long int horaTotalSegundo = 0;
long int minTotalSegundo = 0;
String hora = "";
String minu = ""; 
//hora electrovalvula
int valvula=12;       //Utilizamos el Pin 12 para conectar la electroválvulas


void setup() {
 // lcd.begin(16, 2);
  servo.attach(9);
//  lcd.print(tiempo);
  //delay(2000);
  //lcd.clear();
  pinMode(valvula,OUTPUT);
  //digitalWrite(valvula,LOW);
  myThread.onRun(abreServo);
  myThread.setInterval(tiempoEsperaServo); 
  //Inicializamos la comunicacion por Serial
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void abreValvula(){
  //valvula
  //digitalWrite(valvula,LOW);
  //delay(3000);
  
  //delay(2000);
  //Serial.println("ELECTROVALVULA");
  digitalWrite(valvula,HIGH);
  delay(5000);
  digitalWrite(valvula,LOW);
  //delay(2000);
  //delay(3000);
}

void abreServo(){
  servo.write(90);
  delay(500);
  servo.write(0);
}

int calculaHoraAsegundo(int hora){
  return hora * 360 * 1000;
}

int calculaMinAsegundo(int minu){
  return minu * 60 * 1000;
}

void loop(){
  unsigned int cmComidaNotifica,cmAguaNotifica,cmElectrovalvula;
  
  cmComidaNotifica = ultraComidaNotifica.Ranging(CM);
  cmAguaNotifica = ultraAguaNotifica.Ranging(CM);
  cmElectrovalvula = ultraElectroValvula.Ranging(CM);
  
  //Serial.print("COMIDA: ");
  //Serial.print(cmComidaNotifica); // CM or INC
  //Serial.print(" cm     " );
  delay(50);
  //Serial.print("AGUA: ");
  //Serial.print(cmAguaNotifica); // CM or INC
  //Serial.println(" cm" );
  //delay(50);
  //Serial.print("ELECTROVALVULA: ");
  //Serial.print(cmElectrovalvula); // CM or INC
  //Serial.println(" cm" );
  
  
  //Ultrasonido COMIDA
  if(cmComidaNotifica == 10){
    Serial.println(int(0));
    delay(1000);
  }
  if(cmComidaNotifica == 6){
    Serial.println(int(1));
    delay(1000);
  }
  if(cmComidaNotifica == 3){
    Serial.println(int(2));
    delay(1000);
  }
  //Ultrasonido Agua
  if(cmAguaNotifica == 10){
    Serial.println(int(3));
    delay(1000);
  }
  if(cmAguaNotifica == 6){
    Serial.println(int(4));
    delay(1000);
  }
  if(cmAguaNotifica == 3){
    Serial.println(int(5));
    delay(1000);
  }
  //Ultrasonido ElectroValvula AGUA
  if(cmElectrovalvula <= 5){
    abreValvula();
  }
  else{
  }
  
  
  
  char key = myKeypad.getKey();
  if (key=='1'||key=='2'||key=='3'||key=='4'||key=='5'||key=='6'||key=='7'||key=='8'||key=='9'||key=='0'){
      //lcd.print(key);
      //lcd.clear();
        if(tiempo[cont]==':'){
          cont++;
        }
        
        tiempo[cont] = key;
        cont++;
        Serial.print(tiempo);
        Serial.print("\n");
        //lcd.print(tiempo);
        Serial.print("Tiempo : ");
        Serial.println(tiempo);
  }

  if(cont == 6){
    //coje el valor del teclado
    h1[0] = tiempo[0];
    h2[0] = tiempo[1];
    m1[0] = tiempo[3];
    m2[0] = tiempo[4];
    char tiempoTotalHora[10] = {0};
    char tiempoTotalMinuto[10] = {0};
    
    strcat(tiempoTotalHora, h1);
    strcat(tiempoTotalHora, h2);
    strcat(tiempoTotalMinuto, m1);
    strcat(tiempoTotalMinuto, m2);
    
    hora = String(tiempoTotalHora);
    minu = String(tiempoTotalMinuto); 
    //lcd.begin(0, 0);
    horaTotalSegundo = hora.toInt()*1000*360;
    minTotalSegundo = minu.toInt()*1000*60;
    tiempoEsperaServo = horaTotalSegundo+minTotalSegundo;
    //Serial.print(tiempoEsperaServo);
    //timedAction1 = TimedAction(tiempoEsperaServo,abreServo);
    //timedAction1.check();
    myThread.setInterval(tiempoEsperaServo);
    tiempo[0] = 0;
    tiempo[1] = 0;
    tiempo[3] = 0;
    tiempo[4] = 0;
    //delay(5000);
    //Serial.print("\n");
    //lcd.print(horaTotalSegundo);
    cont = 0;
  }

  
  if(BTSerial.available()){
      //Serial.write(BTSerial.read());
  }
  if (Serial.available()){
      //BTSerial.write(Serial.read());
      char a = Serial.read();
      //Serial.print(a);    
      //Serial.print("\n");
      tiempoHoraMinBlueTooth = String(a);
      tiempoEsperaServo = tiempoHoraMinBlueTooth.toInt();
      Serial.print(tiempoEsperaServo);
      myThread.setInterval(tiempoEsperaServo);
      myThread.onRun(abreServo);
      tiempoEsperaServo = tiempoHoraMinBlueTooth.toInt();
  }
  
  if( myThread.shouldRun()){
      //myThread.setInterval(tiempoEsperaServo);
      myThread.run();
      //Hilo Servo Arduino Stop
  }
  
  
}
