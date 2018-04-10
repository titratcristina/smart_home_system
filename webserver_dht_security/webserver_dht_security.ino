//includerea bibliotecilor
#include <ESP8266WiFi.h> //biblioteca WiFi pentru ESP8266
#include <ESP8266WebServer.h> //biblioteca pentru crearea serverului web 
#include <EasyDDNS.h> //biblioteca EasyDDNS este folosita pentru a mentine hostnameul actualizat cu IPul extern dinamic
#include <LiquidCrystal_I2C.h> //biblioteca LCD ului care comunica prin I2C
#include "DHTesp.h" //biblioteca senzorului de temperatura
#include <Servo.h> //biblioteca servomotorului

//configurarea variabilelor pentru conexiunea wifi
#define ssid "my network name"
#define password "my network password"

//declararea obiectelor globale pentru a le putea accesa ulterior
LiquidCrystal_I2C lcd(0x3F, 16, 2);
DHTesp dht;
ESP8266WebServer server(80);
Servo myservo;

// configurarea pinilor si declararea variabilelor
uint8_t degree[8] = {0x8,0xf4,0x8,0x43,0x4,0x4,0x43,0x0}; //caracterul pentru grade celsius
float u,t;
int servoPin = D8;
int dhtPin = D4;
int buzzerPin = D5;
int buzzerVCC = D6;
int verificare = 1;
int pos = 1400;

void setup() {
  Serial.begin(9600);
  
  WiFi.mode(WIFI_OFF); //oprirea wifiului pana cand se face configurarea si conectarea
    
  dht.setup(dhtPin); //configurarea pinului pentru senzorul de temperatura si umiditare

  //configurarea LCD-ului
  lcd.begin(16,2); 
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, degree); //crearea caracterului pentru grade

  //configurarea pinilor buzzerului
  pinMode(buzzerPin, OUTPUT);
  pinMode(buzzerVCC, OUTPUT);
  digitalWrite(buzzerVCC, LOW);

  //configurarea motorasului
  myservo.writeMicroseconds(pos);
  myservo.attach(servoPin);
  myservo.write(pos);
  
  Serial.print("Connecting to ");
  Serial.print(ssid);

  //configurarea wifiului
  WiFi.begin(ssid, password); //conectarea la wifi
  IPAddress ip(192,168,1,200);
  IPAddress gateway(192,168,1,1);
  IPAddress subnet(225,225,225,0);
  WiFi.config(ip, gateway, subnet);

  while(WiFi.status() != WL_CONNECTED){ //verificarea statusului conexiunii si asteptarea conectarii
      delay(500);
      Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin(); //pornirea serverului 
  
  EasyDDNS.service("noip"); //declararea serviciului folosit
  EasyDDNS.client("myhostname.ddns.net","my acount name","my acount password"); //Datele pentru login pe serviciul ddns si selectarea hostnameului pentru actualizare

  //cand are loc o cere HTTP catre una din aceste parti se va executa functia specifica 
  server.on("/temp", getTemp); //crearea unui API unde este afisata valoarea temperaturii
  server.on("/umid", getUmid); //API pentru valoarea umiditatii
  server.on("/vinemonstrul", alerta); //linkul accesat de client care porneste buzzerul pentru a anunta ca o persoana este in apropiere
  server.on("/open", openDoor); //linkul prin care se "descuie usa", momentan porneste si opreste motorului
  server.on("/", indexHtml); //pagina initila unde este trimis html
  server.on("/stinge", noapte); //link pentru oprirea luminii LCD ului
  server.on("/aprinde", zi); //link pentru aprinderea luminii LCD ului
}

void loop() {
  //pentru a gestiona cererile primite de HTTP, trebuie apelata metoda handleClient pe obiectul serverului
  server.handleClient();
  
  EasyDDNS.update(10000); //verifica la fiecare 10 secunde dacă e necesara actualizarea hostnameului
  delay(1500);

  //preluarea valorilor de la DHT si stocarea lor in variabile
  u = dht.getHumidity();
  t = dht.getTemperature();

  Serial.println(u, 1);
  Serial.println(t, 1);

  //afisarea pe LCD a valorilor
  lcd.setCursor(0,0);
  lcd.print("temp:      ");
  lcd.setCursor(6,0);
  lcd.print(t, 1);
  lcd.setCursor(10,0);
  lcd.print((char)0);
  lcd.setCursor(0,1);
  lcd.print("umid:     %");
  lcd.setCursor(6,1);
  lcd.print(u, 1);
}

void getTemp() { //utilizarea metodei send pentru a trimite valoarea temperaturii / valoarea preluata de clientii API
    server.send(200, "text/html", String(t));
  }

void getUmid() { //utilizarea metodei send pentru a trimite valoarea umiditatii
    server.send(200, "text/html", String(u)); 
  }

void alerta() {
  //pornirea buzzerului
  digitalWrite(buzzerVCC, HIGH);
  tone(buzzerPin, 1000, 500); 
  if (verificare == 1) { //oprirea si stingerea LCDului pentru avertizare
   lcd.noBacklight();
   delay(100);
   lcd.backlight();
   delay(100);
   lcd.noBacklight();
   delay(100);
   lcd.backlight();
   delay(100);
   lcd.noBacklight();
   delay(100);
   lcd.backlight(); 
  }else{
    lcd.backlight();
    delay(100);
    lcd.noBacklight();
    delay(100);
    lcd.backlight();
    delay(100);
    lcd.noBacklight();
    delay(100);
    lcd.backlight();
    delay(100);
    lcd.noBacklight();
  }
  delay(500);
  digitalWrite(buzzerVCC, LOW);//oprirea buzzerului
}

void openDoor() {
    pos = 2000;
    myservo.writeMicroseconds(pos); //pornirea motorasului in sensul acelor de ceasornic
    Serial.println("usa s-a deschis");
    delay(100); 
    pos = 1000;
    myservo.writeMicroseconds(pos); //pornirea motorasului in sensul invers acelor de ceasornic
    Serial.println("usa s-a inchis");
    delay(100);
    pos = 1400;
    myservo.writeMicroseconds(pos); //oprirea motorasului (folosesc un motor cu rotatie continua care trebui oprit)
}

void noapte() { //oprirea luminii LCD ului
  lcd.noBacklight();
  verificare = 0;
}

void zi() { //pornirea luminii LCD ului
  lcd.backlight();
  verificare = 1;
}

void indexHtml() { //trimiterea continutului/html-ului care trebuie afisat de browser
    server.send(200, "text/html",
    "<!doctype html>\
     <html>\
        <head>\
            <meta charset='utf-8'/>\
            <title>Temp si umid</title>\
            <meta name='viewport' content='width=device-width, initial-scale=1'>\
            <script src='https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.0/jquery.min.js' integrity='sha256-RTQy8VOmNlT6b2PIRur37p6JEBZUE7o8wPgMvu18MC4=' crossorigin='anonymous'></script>\
            <link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>\
            <script src=\"https://code.jquery.com/jquery-3.2.1.min.js\"></script>\
            <style>\
               .gauge {\
                  width: 300px;\
                  height: 300px;\
                  display: inline-block;}\
                body {\
                  padding:15px;}\
                button{\
                  margin:5px;}\
            </style>\
        </head>\
        <body>\
            <div class='container text-center'>\
                <div class='row'>\
                    <div class='col-md-6'>\
                        <h2>TEMPERATURA</h2>\
                        <div class='alert ' role='alert' id='temp_s'></div>\
                        <div id='gage1' class='gauge'></div>\
                    </div>\
                    <div class='col-md-6'>\
                        <h2>UMIDITATE</h2>\
                        <div class='alert ' role='alert' id='umid_s'></div>\
                        <div id='gage2' class='gauge'></div>\
                    </div>\
               </div>\
               <div class='row'>\
                  <div class='col-12'>\
                    <button onclick='update()' class='btn btn-primary btn-sm'>CLICK ME 4 UPDATE</button>\
                    <button onclick=\"$.get('/stinge')\" class='btn btn-dark btn-sm'>Night mode on</button>\
                    <button onclick=\"$.get('/aprinde')\" class='btn btn-secondary btn-sm'>Night mode off</button>\
                    <button onclick=\"$.get('/open')\" class='btn btn-success btn-sm'>Open the door</button>\
                  </div>\
               </div>\
           </div>\
          <script src='https://cdnjs.cloudflare.com/ajax/libs/raphael/2.2.7/raphael.min.js' integrity='sha256-67By+NpOtm9ka1R6xpUefeGOY8kWWHHRAKlvaTJ7ONI=' crossorigin='anonymous'></script>\
          <script src='https://cdnjs.cloudflare.com/ajax/libs/justgage/1.2.9/justgage.min.js' integrity='sha256-nL2oTVCXGFtdu+pxt8QkObEXDFh0CVRkwLPIy04s8z0=' crossorigin='anonymous'></script>\
          <script src='https://rawgit.com/titratcristina/IoT_thermometer/7d114ea429b28b98668c5ab2f240cca5bb05fdd7/script.js'></script>\
     </body>\
  </html>");}
