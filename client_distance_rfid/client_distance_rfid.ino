//includerea bibliotecilor
#include <ESP8266WiFi.h> //biblioteca WiFi pentru ESP8266
#include <ESP8266HTTPClient.h> //biblioteca care permite sa se faca cereri HTTP
#include <SPI.h> //biblioteca pentru comunicarea intre placuta si componente i/o
#include <MFRC522.h> //biblioteca pentru citirea sau scrierea cardurilor RFID

//variabilele pentru configurarea conexiunii WiFi
#define ssid "my WiFi name"
#define password "my WiFi password" 

//configurare pinilor pentru interfatarea componentelor
//pinii pentru senzorul de distanta
#define trigPin D4
#define echoPin D3 

//pinii RFID-ului
#define SS_PIN D2
#define RST_PIN D1

//pinul senzorului PIR
#define PIR_PIN D8

HTTPClient http;   //declararea obiectului pentru a apela funcționalitatea bibliotecii
MFRC522 mfrc522(SS_PIN, RST_PIN);   //declararea obiectului cititor rfid

int statuss = 0;
int out = 0;
long duration;
int distance;
 
void setup () {
  Serial.begin(9600);

  //conectarea la WiFi
  WiFi.begin(ssid, password);

  IPAddress ip(192,168,1,210);
  IPAddress gateway(192,168,1,1);
  IPAddress subnet(225,225,225,0);
  WiFi.config(ip, gateway, subnet); //configurarea conexiunii
 
  while (WiFi.status() != WL_CONNECTED) { //verificarea statusul conexiunii
    delay(1000);
    Serial.print("Connecting..");
  }
  
  SPI.begin();      // initializarea comunicarii
  mfrc522.PCD_Init();   // initialzarea MFRC522
  
  pinMode(trigPin, OUTPUT); //setarea pinilor senzorului de distanta
  pinMode(echoPin, INPUT);
  pinMode(PIR_PIN, INPUT); //setarea pinului PIR ca output

}
 
void loop() {
  //trimiterea unei sunet de frecventa inalta pentru aproximarea distantei
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2; //calcularea distantei in functie de durata in care sunetul a fost trimis si receptat inmultit cu viteza sunetului/2
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(100);
  
  if (WiFi.status() == WL_CONNECTED && distance < 50 && digitalRead(PIR_PIN)) { //verifica statusul conexiunii, distanta intre persoana si senzor, verifica daca este un om
    http.begin("192.168.1.200",80,"/vinemonstrul"); //apelarea metodei "begin()" pentru a specifica URL-ul catre care se va face cererea HTTP
    http.GET(); // apelarea metodei "GET()" pentru trimiterea cererii care returneaza codul HTTP
    http.end(); // apelarea metodei "end()" pentru eliberarea resuselor
    delay(4000);
  }

  // verifica daca sunt carduri nou
  if ( ! mfrc522.PICC_IsNewCardPresent()){
    return;
  }
  // selecteaza cardul
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  //afisarea UID in serial
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  //transforma IDul hex intr-un string ce se poate compara cu ce este in memorie
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  //verifica daca dispozitivul este conectat la wifi si daca UID ul citit este egal cu cele 2 pentru a permite accesul sau nu
  if (WiFi.status() == WL_CONNECTED && content.substring(1) == "62 F1 9C 64" || WiFi.status() == WL_CONNECTED && content.substring(1) == "F3 F2 EE BC"){
    Serial.println(" Access Granted ");
    http.begin("192.168.1.200",80,"/open"); 
    http.GET();
    http.end();
    statuss = 1;
    delay(4000);
  } else{
    Serial.println(" Access Denied ");
    delay(3000);
  }
}
