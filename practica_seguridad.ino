#include <Keypad.h>
#include <LiquidCrystal.h>
#include "AsyncTaskLib.h"

#define DEBUG(a) Serial.print(millis()); Serial.print(": "); Serial.println(a);


#define LED_GREEN 9
#define LED_RED 10
#define LED_BLUE 6
#define BUZZER_PASIVO 7	
#define NOTE_C4  262

void tarea();

AsyncTask asyncTask(1000, true, tarea);







const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const char password[5] = "8989";
int intentos=0;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {24, 26, 28, 30}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {32, 34, 36}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char clave[5] = "0000";
int idx = 0;

void setup() {
  Serial.begin(9600);
Serial.println("Starting");
  asyncTask.Start();  


  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  lcd.begin(16, 2);
  pinMode(BUZZER_PASIVO, OUTPUT);	// pin 7 salida

}
void loop() {
 asyncTask.Update();

 }

int melodia[] = {		
  NOTE_C4
};
int duraciones[] = {
  2
};
void sonido() {
  for (int i = 0; i < 2; i++) {			
      int duracion = 1000 / duraciones[i];	
      tone(BUZZER_PASIVO, melodia[i], duracion);	
      int pausa = duracion * 1.30;		
      delay(pausa);				
      noTone(BUZZER_PASIVO);			
  }
}

void tarea(){
   digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BLUE, LOW);
  
  do{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ingrese contraseña: ");
    lcd.setCursor(0, 1);
    do{
    char key = keypad.getKey();
      if (key) {
      clave[idx++] = key;
      Serial.print(key);
      lcd.print(key);
      }
    } while (idx < 4);
    if(strncmp(password,clave,4)==0){
       lcd.setCursor(0, 0);
       lcd.println("clave correcta");
       digitalWrite(LED_GREEN, HIGH);
       
       sonido();       
       digitalWrite(LED_GREEN, LOW);       
       idx = 0;
       break;
    }else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("clave incorrecta");
      lcd.setCursor(0, 1);
      lcd.println("Intente de nuevo");
      digitalWrite(LED_BLUE, HIGH);
          
      intentos= intentos+1;
      idx=0;
      sonido();
      
      digitalWrite(LED_BLUE, LOW);  
    }
    if(intentos==3){
      lcd.clear();
      lcd.println("Sistema bloqueado");
      lcd.setCursor(0, 0);
      digitalWrite(LED_RED, HIGH);
      
      sonido();
      
      delay(5000);
      digitalWrite(LED_RED, LOW);
      intentos=0;
    }
  }while(intentos<=3);  

}