*Arduino code:

#include <Keypad.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);

#include<LiquidCrystal.h>
LiquidCrystal lcd(2,3,4,5,6,7);


#define Password_Length 5 

int buzzer = 9;
int relay = 8;

char Data[Password_Length]; 
char Master[Password_Length] = "1234"; 
byte data_count = 0, master_count = 0;
bool Pass_is_good;

char customKey;

const byte ROWS = 3; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
26
  {'1', '3', '2', '*'},
  {'4', '6', '5', '0'},
  {'7', '9', '8', '#'}
 // {'*', '0', '#', 'A'}
};

byte rowPins[ROWS] = {14, 15, 16}; 
byte colPins[COLS] = {17, 18, 19 ,20}; //8, 9, 7, 6  //5, 4, 3 ,2

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
void clearData();

byte flag1=0;

String incomingData;   // for storing incoming serial data
String message = "";   // A String for storing the message



void SendSMS();
void send_message(String message);

void buz()
{
  for(int k =0;k<2;k++)
  {
  digitalWrite(buzzer, HIGH);
27
  delay(1000);
  digitalWrite(buzzer, LOW);
  delay(200);
  }
 }
 

 
void setup()
{

  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  digitalWrite(buzzer, LOW);
  
    lcd.begin(16, 2);
    lcd.clear();
    lcd.print("PASSWORD BASED ");
    lcd.setCursor(0,1);
    lcd.print("CIRCUIT BREAKER"); // 
    delay(2000);


   digitalWrite(relay, LOW); //ON
     
    Serial.begin(9600);
28 
mySerial.begin(9600);
    delay(1000);
    SendSMS();
    lcd.clear();
}





void loop()
{
  lcd.setCursor(0,0);
  lcd.print("Enter Password:");
  customKey = customKeypad.getKey();
  if (customKey){   // && flag1==1
    Data[data_count] = customKey; 
    lcd.setCursor(data_count,1); 
    lcd.print(Data[data_count]); 
    data_count++; 
    }

  if(data_count == Password_Length-1){
    lcd.clear();

    if(!strcmp(Data, Master)){
      flag1=~flag1;
29
      lcd.print("Password Correct");
      delay(1000);
      if(flag1==0x00)
      {
       digitalWrite(relay, LOW); 
       lcd.clear();
        lcd.print("CIRCUIT BREAKER");
        lcd.setCursor(0,1);
        lcd.print("    ON     "); 
       delay(2000);
       send_message("CIRCUIT BREAKER ON ");
        }
      if(flag1==0xff)
      {
       digitalWrite(relay, HIGH); 
       lcd.clear();
        lcd.print("CIRCUIT BREAKER");
        lcd.setCursor(0,1);
        lcd.print("    OFF     "); 
       delay(2000);
       send_message("CIRCUIT BREAKER OFF");
        }    
      clearData();
      lcd.clear();
      }

      
30
    else{
      flag1=0;
      lcd.print("Password Incorrect");
      buz();
      delay(1000);
      //send_message("Password Incorrect");
      //delay(1000);
      clearData();
      lcd.clear();
      }
 
  }


}
// LOOP END//


void clearData(){
  while(data_count !=0){
    Data[data_count--] = 0; 
  }
  return;
}

 
void SendSMS()
31
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MODEM INIT....");
    
  mySerial.println("AT");    //To send SMS in Text Mode
  delay(1000);
  mySerial.println("AT+CMGF=1");    //To send SMS in Text Mode
  delay(1000);
  mySerial.println("AT+CNMI=2,2,0,0,0");    //To send SMS in Text Mode
  delay(1000);
  mySerial.println("AT+CMGS=\"8074218334\"\r"); //Change to destination phone number 9494252144
  delay(1000);
  mySerial.println("GSM Modem! INIT...");//the content of the message
  delay(200);
  mySerial.println((char)26); //the stopping character Ctrl+Z
  mySerial.write(0x1A);
  delay(5000); 
        lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MSG SENT");
      delay(1000);    
}


void send_message(String message)
{
32
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MSG SENDING.....");
  mySerial.println("AT");    //To send SMS in Text Mode
  delay(1000);
  mySerial.println("AT+CMGF=1");    //To send SMS in Text Mode
  delay(1000);
  mySerial.println("AT+CNMI=2,2,0,0,0");    //To send SMS in Text Mode
  delay(1000);
  mySerial.println("AT+CMGF=1");    //Set the GSM Module in Text Mode
  delay(1000);  
  mySerial.println("AT+CMGS=\"8074218334\""); // Replace it with your mobile number
  delay(1000);
  mySerial.println(message);   // The SMS text you want to send
  delay(1000);
  mySerial.println((char)26); //the stopping character Ctrl+Z
  mySerial.write(0x1A);
  delay(3000); 
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MSG SENT");
      delay(1000);
}





