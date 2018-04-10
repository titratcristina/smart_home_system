//includerea bibliotecilor
#include <ESP8266WiFi.h> //biblioteca WiFi pentru ESP8266
#include <ESP8266WebServer.h> //biblioteca pentru crearea serverului web
#include <EasyDDNS.h>  //biblioteca EasyDDNS este folosita pentru a mentine hostnameul actualizat cu IPul extern dinamic

//configurarea variabilelor pentru conexiunea wifi
#define ssid "my network name"
#define password "my network password"

ESP8266WebServer server(80); //declararea obiectului global pentru a-l putea accesa ulterior

//configurarea pinilor 
int sense_Pin = A0;
int value = 0;

void setup() {  
  WiFi.mode(WIFI_OFF);
  
  Serial.begin(9600);

  Serial.print("Connecting to ");
  Serial.print(ssid);

  //configurarea wifiului
  WiFi.begin(ssid, password); //conectarea la wifi
  IPAddress ip(192,168,1,205);
  IPAddress gateway(192,168,1,1);
  IPAddress subnet(225,225,225,0);
  WiFi.config(ip, gateway, subnet);

  while(WiFi.status() != WL_CONNECTED){ //asteptarea realizarii conexiunii
      delay(500);
      Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin(); //pornirea serverului
  
  EasyDDNS.service("noip"); //declararea serviciului folosit
  EasyDDNS.client("myhostname.ddns.net","my acount name","my acount password"); //datele pentru login pe serviciul ddns si selectarea hostnameului pentru actualizare 

  //cand are loc o cere HTTP catre una din aceste parti se va executa functia specifica 
  server.on("/value", getValue); //API ul unde este valoarea umiditatii din sol
  server.on("/", indexHtml); //pagina principala trimisa de server
}

void loop() {
  server.handleClient(); //gestionarea cererilor HTTP
  
  EasyDDNS.update(10000); //verifica daca ip-ul extern s-a schimbat
  delay(1500);

  //afisarea in serial a umiditatii din sol
  Serial.print("MOISTURE LEVEL : ");
  value = analogRead(sense_Pin);
  value = value/10;
  Serial.println(value);
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
