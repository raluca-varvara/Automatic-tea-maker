#include <TimerOne.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27,16,2);

Servo myservo2; //servo pentru brat

int pos2 = 0; //servo pentru brat

//SENZOR
float resolutionADC = .0049 ; // rezoluția implicită (pentru
//referința 5V) = 0.049 [V] / unitate
float resolutionSensor = .01 ; // rezoluție senzor = 0.01V/°C

//BUTOANE
int BUTTON_1 = 2; // infuzie de fructe = 7 minute
int BUTTON_2 = 3; // ceai verde = 3 minute
int BUTTON_3 = 4; // ceai negru = 2 minute

int type_of_tea = 0; //1 = fructe 2 = verde 3 = negru

volatile float oldTime = 0.0;
volatile float newTime = 0.0f;
volatile float myVar = 0.0f; //cat timp sta la infuzat ceaiul
volatile float temp = 0.0;
volatile float temp1 = 100.0f;
volatile float temp2 = 100.0f;

volatile bool in_the_making = false;
volatile bool wait_to_drink = false;
volatile bool servo_on = false;

void setup() {
  // initializare UART si Timer:
  Serial.begin(9600);
  Timer1.initialize(100000); //0.1 sec
  Timer1.attachInterrupt(ShowMessage);

  //LCD SETUP
  lcd.init();                      // initialize the lcd 
  lcd.clear();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("aparat ceai");



  //BUTOANE VARIANTE
  pinMode(BUTTON_1, INPUT_PULLUP); //infuzie fructe
  pinMode(BUTTON_2, INPUT_PULLUP); //ceai verde
  pinMode(BUTTON_3, INPUT_PULLUP); //ceai negru

  //SERVO
  myservo2.attach(10);
  myservo2.write(0);
  DDRA = 0b00000000;
  PORTA = 0b11111111;
}

void loop() {
  // alegerea ceaiului:
  if(!in_the_making & !wait_to_drink){
    
    //int v1 = digitalRead(BUTTON_1);
    //int v2 = digitalRead(BUTTON_2);
    //int v3 = digitalRead(BUTTON_3);
    
    //SE ALEGE TIPUL DE CEAI
    char x = PINA;
    if(x == ~1){
      type_of_tea = 1;
      in_the_making = true;
      Serial.println("infuzie de fructe");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("infuzie de fructe");
      oldTime = newTime;     
      servo_on = true;
      
    }
    else if(x==~2){
      type_of_tea = 2;
      in_the_making = true;     
      Serial.println("ceai verde");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ceai verde");
      oldTime = newTime;
      servo_on = true;    
    }
    else if(x==~4){
      type_of_tea = 3;
      Serial.println("ceai negru");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ceai negru");
      in_the_making = true;
      oldTime = newTime;
      servo_on = true;
    }    
  }
  // se plaseaza ceaiul in cana
  if(in_the_making & !wait_to_drink & servo_on){
        if(newTime-oldTime >= 0.1){
          pos2 = pos2 + 3;
          myservo2.write(pos2);
          oldTime = newTime;
        }
        if(pos2 == 72){
           servo_on = false;
        }
        
  }
  //se asteapta infuzarea ceaiului
  else if(in_the_making & !wait_to_drink & !servo_on){
    if(newTime - oldTime < 0){
      //s-a reinitializat
      oldTime = newTime;
    }
    
    if(newTime-oldTime >= 0.1){
      myVar = myVar + 0.1;
      oldTime = newTime;
      Serial.println(myVar);
    }
    
    if(type_of_tea == 1){
      
      if(myVar >= 20.0){ //au trecut 10 secunde
        
        Serial.println("infuzie fructe gata");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("infuzie fructe");
        lcd.setCursor(0,1);
        lcd.print("gata");

        myVar = 0;
        type_of_tea = 0;
                
        in_the_making = false;      
        servo_on = true;
        wait_to_drink = true;
      }   
    }
    if(type_of_tea == 2){
      
      if(myVar >= 10.0){ //au trecut 3 secunde
        
        Serial.println("ceai verde gata");
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("ceai verde gata");

        myVar = 0;
        type_of_tea = 0;
                
        in_the_making = false;      
        servo_on = true;
        wait_to_drink = true;
      }   
    }
    if(type_of_tea == 3){
      
      if(myVar >= 7.0){ //au trecut 2 secunde
        
        Serial.println("ceai negru gata");
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("ceai negru gata");

        myVar = 0;
        type_of_tea = 0;
                
        in_the_making = false;      
        servo_on = true;
        wait_to_drink = true;
      }   
    }
  }
  //se scoate pliculetul din cana
  if(!in_the_making & wait_to_drink & servo_on){
    if(newTime-oldTime >= 0.1){
          pos2 = pos2 - 3;
          myservo2.write(pos2);
          oldTime = newTime;
        }
        if(pos2 == 0){
           servo_on = false;
           
        }
  }
  //se verifica temperatura
  if(!in_the_making & wait_to_drink & !servo_on){
    if(newTime-oldTime >= 0.5){
          Serial.println(temp);
          temp = readTempInCelsius(5, 0);         
          if(temp < 35.0 && temp1 < 35.0 && temp2<35.0 && temp > 0.1 && temp2>0.1 && temp1>0.1){
            wait_to_drink = false;
            lcd.clear();
            lcd.setCursor(1,0);
            lcd.print("bun de baut");
          }
          temp2 = temp1;
          temp1 = temp;
          oldTime = newTime;         
     }
  }
  
}

void ShowMessage(void)
{
  newTime+=0.1;
  if(newTime>1000000.f){
    newTime = 0.0f;
  }
  //if(wait_to_drink){
  //  temp = readTempInCelsius(5, 0);       
  //}
}

//CITIREA DE PE SENZOR
float readTempInCelsius(int count, int pin) {
  // citește temperatura de count ori de pe pinul analogic pin
  float sumTemp = 0;
  for (int i =0; i < count; i++) {
    int reading = analogRead(pin);
    float voltage = reading * resolutionADC;
    float tempCelsius = (voltage - 0.5) / resolutionSensor ;
    // scade deplasament, convertește în grade C
   sumTemp = sumTemp + tempCelsius; // suma temperaturilor
  }
  return sumTemp / (float)count; // media returnată
}
