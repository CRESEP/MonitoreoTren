
int inicio = 0;

// the setup routine runs once when you press reset
void setup() {
  // initialize serial communication at 9600 bits per second
  Serial.begin(9600);
 
  inicio = analogRead(A0);
  pinMode(13,OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int valor = analogRead(A0);
  
  if(valor<=25 || valor==inicio)
  {
    delay(1000);
    valor = analogRead(A0);
    if(valor<=25 || valor==inicio)
    {
      Serial.print(valor);
      Serial.print("\n");
      if(valor<=25)
      {
      digitalWrite(13,HIGH);
      }
      else
      {
        digitalWrite(13,LOW);
      }
    }
  }
 
   delay(1);
}
