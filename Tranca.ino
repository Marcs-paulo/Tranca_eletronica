#include <Keypad.h>
#include <Servo.h>
#include <Ultrasonic.h>

const byte ROW_NUM = 4;
const byte COL_NUM = 4; 

const char TECLAS_MATRIZ[ROW_NUM][COL_NUM] = {
  {'1', '2', '3', '*'},
  {'4', '5', '6', '0'},
  {'7', '8', '9', '#'},
  {'A', 'B', 'C', 'D'}
};

const byte pin_rows[ROW_NUM] = {9, 8, 7, 6}; 
const byte pin_column[COL_NUM] = {5, 4, 3, 2}; 

Keypad keypad = Keypad(makeKeymap(TECLAS_MATRIZ), pin_rows, pin_column, ROW_NUM, COL_NUM);

Servo servo;
String password = "";
String correctPassword = "123";


const int trigPin = 12;
const int echoPin = 11;

Ultrasonic ultrasonic(trigPin, echoPin);

bool doorClosed = false; 


const int greenLedPin = 13;


const int maxAttempts = 3;
int attempts = 0;

void setup() {
  servo.attach(10); 
  pinMode(greenLedPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
 char key = keypad.getKey();
 
 if (key) {
   password += key;
   Serial.println(password);


   if (password == correctPassword) {
     Serial.println("Senha correta!");
     digitalWrite(greenLedPin, HIGH); 
     servo.write(180); 
     password = "";    
     doorClosed = false; 
     attempts = 0;
     delay(5000);
     digitalWrite(greenLedPin, LOW); 
   } else if (password.length() >= correctPassword.length() && password != correctPassword) {
     attempts++;
     if (attempts >= maxAttempts) {
       Serial.println("Número máximo de tentativas atingido. Aguarde antes de tentar novamente.");
       delay(60000); 
       attempts = 0; 
     } else {
       Serial.println("Senha incorreta!");
       for(int i=0; i<5; i++){
         digitalWrite(greenLedPin, HIGH); 
         delay(500); 
         digitalWrite(greenLedPin, LOW); 
         delay(500); 
       }
     }
     password = "";  
   }
 }

 long distance = ultrasonic.read();
 
 if (!doorClosed && distance < 15) { 
   delay(5000);
   servo.write(90); 
   doorClosed = true;
}
}

