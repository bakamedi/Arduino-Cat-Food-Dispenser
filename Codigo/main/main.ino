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
Thread myThread = Thread();
LiquidCrystal lcd(21, 20, 19, 18, 17, 16); // LCD arduino library
//ultrasonido
Ultrasonic ultraComidaNotifica(10,8);//555555555555555555555555555555555555555555555555555555555555555 (Trig PIN,Echo PIN)
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
int long tiempoEsperaServo = 15000;//3 segundos
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
  lcd.begin(16, 2);
  servo.attach(9);
  lcd.print(tiempo);
  pinMode(valvula,OUTPUT);
  myThread.onRun(abreServo);
  myThread.setInterval(tiempoEsperaServo); 
  //Inicializamos la comunicacion por Serial
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void abreValvula(){
  digitalWrite(valvula,HIGH);
  delay(5000);
  digitalWrite(valvula,LOW);
}

void abreServo(){
  servo.write(0);
  delay(1500);
  servo.write(90);
}


void loop(){
  unsigned int cmComidaNotifica,cmAguaNotifica,cmElectrovalvula;
  
  cmComidaNotifica = ultraComidaNotifica.Ranging(CM);
  cmAguaNotifica = ultraAguaNotifica.Ranging(CM);
  cmElectrovalvula = ultraElectroValvula.Ranging(CM);
  //Serial.print("COMIDA: ");
  //Serial.print(cmComidaNotifica); // CM or INC
  //Serial.println(" cm" );
  delay(500);
  
  //Ultrasonido COMIDA
  if(cmComidaNotifica >= 15){
    Serial.println(int(0));
    delay(1000);
  }
  if(cmComidaNotifica >= 4 && cmComidaNotifica<=12){
    Serial.println(int(1));
    delay(1000);
  }
  if(cmComidaNotifica  < 4){
    Serial.println(int(2));
    delay(1000);
  }
  //Ultrasonido Agua
  if(cmAguaNotifica >= 20){
    Serial.println(int(3));
    delay(1000);
  }
  if(cmAguaNotifica >= 4 && cmAguaNotifica <=12){
    Serial.println(int(4));
    delay(1000);
  }
  if(cmAguaNotifica < 4){
    Serial.println(int(5));
    delay(1000);
  }
  //Ultrasonido ElectroValvula AGUA
  if(cmElectrovalvula == 9){
    abreValvula();
  }
  else{
  }
  
  char key = myKeypad.getKey();
  if (key=='1'||key=='2'||key=='3'||key=='4'||key=='5'||key=='6'||key=='7'||key=='8'||key=='9'||key=='0'){

      lcd.clear();
        if(tiempo[cont]==':'){
          cont++;
        }
        tiempo[cont] = key;
        cont++;
        Serial.print(tiempo);
        Serial.print("\n");
        lcd.print(tiempo);
        Serial.print("Tiempo : ");
        Serial.println(tiempo);
  }

  if(cont == 6){
    //coje el valor del teclado
    lcd.clear();
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
    horaTotalSegundo = hora.toInt()*1000*360;
    minTotalSegundo = minu.toInt()*1000*60;
    tiempoEsperaServo = horaTotalSegundo+minTotalSegundo;
    myThread.setInterval(tiempoEsperaServo);
    tiempo[0] = 0;
    tiempo[1] = 0;
    tiempo[3] = 0;
    tiempo[4] = 0;
    lcd.print(horaTotalSegundo);
    cont = 0;
    lcd.clear();
  }

  
  if(BTSerial.available()){
      //Serial.write(BTSerial.read());
  }
  if (Serial.available()){
      char a = Serial.read();
      tiempoHoraMinBlueTooth = String(a);
      tiempoEsperaServo = tiempoHoraMinBlueTooth.toInt();
      Serial.print(tiempoEsperaServo);
      myThread.setInterval(tiempoEsperaServo);
      myThread.onRun(abreServo);
      tiempoEsperaServo = tiempoHoraMinBlueTooth.toInt();
  }
  
  if( myThread.shouldRun()){
      myThread.run();
      //Hilo Servo Arduino Stop
  }
  
}