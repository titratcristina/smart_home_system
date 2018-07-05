//includerea bibliotecilor
#include <ESP8266WiFi.h> //biblioteca WiFi pentru ESP8266
#include <ESP8266WebServer.h> //biblioteca pentru crearea serverului web

//configurarea variabilelor pentru conexiunea wifi
#define ssid "sSiD"
#define password "pAsSwOrD"

ESP8266WebServer server(80); //declararea obiectului global pentru a-l putea accesa ulterior

//configurarea pinilor 
const int pumpPin = D4; //pinul D4 pentru pompa
const int solPin = A0; //pinul A3 pentru senzorul de umiditate a solului
int value = 0;

void setup() {
  pinMode(pumpPin, OUTPUT); //seteaza pinul pompei ca fiind unul output
  
  WiFi.mode(WIFI_OFF);
  
  Serial.begin(9600);

  Serial.print("Connecting to ");
  Serial.print(ssid);

  //configurarea wifiului
  WiFi.begin(ssid, password); //conectarea la wifi
  IPAddress ip(192,168,43,205);
  IPAddress gateway(192,168,43,1);
  IPAddress subnet(225,225,225,0);
  WiFi.config(ip, gateway, subnet);

  while(WiFi.status() != WL_CONNECTED){ //asteptarea realizarii conexiunii
      delay(500);
      Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin(); //pornirea serverului
  
  //cand are loc o cere HTTP catre una din aceste parti se va executa functia specifica 
  server.on("/value", getValue); //API ul unde este valoarea umiditatii din sol
  server.on("/", indexHtml); //pagina principala trimisa de server
}

void loop(){
  server.handleClient(); //gestionarea cererilor HTTP
  
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

void getValue() { ///utilizarea metodei send pentru a trimite valoarea emisa de senzor
  server.send(200, "text/html", String(value));  
}

void indexHtml() { //trimiterea html-ului care trebuie afisat de browser
    server.send(200, "text/html",
    "<!doctype html>\
     <html>\
        <head>\
            <meta charset='utf-8'/>\
            <title>Soil Moisture</title>\
            <meta name='viewport' content='width=device-width, initial-scale=1'>\
            <script src='https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.0/jquery.min.js' integrity='sha256-RTQy8VOmNlT6b2PIRur37p6JEBZUE7o8wPgMvu18MC4=' crossorigin='anonymous'></script>\
            <link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>\
            <script src=\"https://code.jquery.com/jquery-3.2.1.min.js\"></script>\
            <style>\
               .gauge {\
                  width: 300px;\
                  height: 300px;\
                  display: inline-block;}\
               body{padding: 10px;}\
               button{margin:5px;}\
            </style>\
        </head>\
        <body>\
            <div class='container align-items-center justify-content-center'>\
                <div class='row justify-content-md-center'>\
                    <div class='col-12 col-md-auto'>\
                        <h2 class='text-center'>Soil Moisture</h2>\
                        <div class='alert ' role='alert' id='val'></div>\
                        <div id='gage1' class='gauge'></div>\
                    </div>\
               </div>\
               <div class='row justify-content-md-center'>\
                  <div class='col-12 col-md-auto'>\
                    <button onclick='update()' type='button' class='btn btn-primary btn-sm'>CLICK ME 4 UPDATE</button>\
                  </div>\
               </div>\
           </div>\
          <script src='https://cdnjs.cloudflare.com/ajax/libs/raphael/2.2.7/raphael.min.js' integrity='sha256-67By+NpOtm9ka1R6xpUefeGOY8kWWHHRAKlvaTJ7ONI=' crossorigin='anonymous'></script>\
          <script src='https://cdnjs.cloudflare.com/ajax/libs/justgage/1.2.9/justgage.min.js' integrity='sha256-nL2oTVCXGFtdu+pxt8QkObEXDFh0CVRkwLPIy04s8z0=' crossorigin='anonymous'></script>\
          <script src='https://rawgit.com/titratcristina/IoT/master/script.js'></script>\
     </body>\
  </html>");
}
