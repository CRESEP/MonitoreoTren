#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 11


// set up a new serial port
SoftwareSerial Sigfox =  SoftwareSerial(rxPin, txPin);

//Set to 0 if you don't need to see the messages in the console
#define DEBUG 1

//Message buffer
uint8_t msg[12];

int pushButton = 7;
int led = 13;
int estado = 0;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(pushButton, INPUT);
  pinMode(led, OUTPUT);
  //
  pinMode(LED_BUILTIN, OUTPUT);

  if(DEBUG){
    Serial.begin(9600);
  }

  // open Wisol communication
   // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Sigfox.begin(9600);
  delay(100);
  getID();
  delay(100);
  getPAC();
}

// the loop function runs over and over again forever
void loop() {
  int buttonState = digitalRead(pushButton); 
  if(buttonState == 1){
    estado = 10; 
  }
  
  for(;estado>1;estado--){
    digitalWrite(led,HIGH);
    msg[0]=0xFE;
    //msg[1]=0xFF;
    //msg[2]=0xEE;

    sendMessage(msg, 1);

    // In the ETSI zone, due to the reglementation, an object cannot emit more than 1% of the time hourly
    // So, 1 hour = 3600 sec
    // 1% of 3600 sec = 36 sec
    // A Sigfox message takes 6 seconds to emit
    // 36 sec / 6 sec = 6 messages per hours -> 1 every 10 minutes
    delay(6000);  
  }

  digitalWrite(led,LOW);
}

void blink(){
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);    
}

//Get Sigfox ID
String getID(){
  String id = "";
  char output;

  Sigfox.print("AT$I=10\r");
  while (!Sigfox.available()){
     blink();
  }

  while(Sigfox.available()){
    output = Sigfox.read();
    id += output;
    delay(10);
  }

  if(DEBUG){
    Serial.println("Sigfox Device ID: ");
    Serial.println(id);
  }

  return id;
}


//Get PAC number
String getPAC(){
  String pac = "";
  char output;

  Sigfox.print("AT$I=11\r");
  while (!Sigfox.available()){
     blink();
  }

  while(Sigfox.available()){
    output = Sigfox.read();
    pac += output;
    delay(10);
  }

  if(DEBUG){
    Serial.println("PAC number: ");
    Serial.println(pac);
  }

  return pac;
}


//Send Sigfox Message
void sendMessage(uint8_t msg[], int size1)
{
  Serial.println("Inside sendMessage");

  String status = "";
  String hexChar = "";
  String sigfoxCommand = "";
  char output;

  sigfoxCommand += "AT$SF=";

  for (int i = 0; i < size1; i++)
  {
    hexChar = String(msg[i], HEX);

    //padding
    if (hexChar.length() == 1)
    {
      hexChar = "0" + hexChar;
    }

    sigfoxCommand += hexChar;
  }

  Serial.println("Sending...");
  Serial.println(sigfoxCommand);
  Sigfox.println(sigfoxCommand);

  while (!Sigfox.available())
  {
    Serial.println("Waiting for response");
    delay(1000);
  }

  while (Sigfox.available())
  {
    output = (char)Sigfox.read();
    status += output;
    delay(10);
  }

  Serial.println();
  Serial.print("Status \t");
  Serial.println(status);
}
