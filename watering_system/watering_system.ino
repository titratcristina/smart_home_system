//configurarea pinilor 
const int pumpPin = 4; //pinul D4 pentru pompa
const int solPin = A3; //pinul A3 pentru senzorul de umiditate a solului
int value = 0;

void setup() {
  pinMode(pumpPin, OUTPUT); //seteaza pinul pompei ca fiind unul output
  Serial.begin(9600);
}

void loop(){
  Serial.print("MOISTURE LEVEL : ");
  value = analogRead(solPin);  //preluarea valorii de la senzor
  value = value/10;            //impartirea la 10 pentru a avea valori sub 100
  Serial.println(value);       // afisarea in serial
  
  if(value < 50){   // valorile sub 50 indica un sol umed, iar cele mai mari ca 50 unul uscat
    digitalWrite(pumpPin, HIGH); //mentine pompa oprita
    Serial.println("pompa asteapa ca solul sa fie uscat");
  }else{
    digitalWrite(pumpPin, LOW); //porneste pompa
    Serial.println("pompa hidrateaza plantuta");
  }
  delay(1000); //astepta o secunda
}
