# smart_home_system

## Descriere proiect 
Un mic sistem automatizat pentru ușurarea activităților casnice și securizarea locuinței.

Pentru partea de securizare și monitoarizare a temperaturii am folosit: 
- două plăcuțe NodeMCU cu modul WiFi, una este server web, iar cealaltă reprezintă un client. 
    - La prima plăcuță, care este un server web am adăugat: 
        - un senzor de temperatură și umiditate DHT11
        - un LCD 16x2 care se conectează prin I2C
        - un buzzer 
        - o încuietoare electromagnetică 12V
    - Iar la plăcuța client am folosit: 
        - un modul RFID 
        - un senzor PIR
        - un senzor ultrasonic de distanță HC-SR04
> Am vrut inițial să am o monitorizare web a temperaturii și a umidității cu o afișare a acestora pe LCD, dar pe parcurs am adăugat la plăcuța server buzzerul care avertizează când cineva este în apropiere și servomotorul care ține locul unei încuietori electrice (senzorul HC-SR04 este conectat la plăcuța client care comunica cu serverul, accesând pagina care pornește "alarmă"/activează buzzerul când cineva se apropie și se efectuează un joc pe LCD, acesta se stinge și se aprinde, iar servomotorul este activat când cineva apropie un card care are acces)


Pentru partea automatizată de udat plantele am folosit:
- O plăcuță NodeMCU cu modul WiFi conectată la:
    - o pompa
    - un senzor de umiditate a solului
    - un releu 
> Declanșarea operațiunii de udare a plantelor se face automat la scăderea umidității din sol, iar verificarea nivelului de umiditate se poate face pe web, pe pagina principală a proiectului unde se află și indicațiile de temperatură și umiditate, dar și individual accesând pagina web de pe IP-ul plăcuței ce formează acest modul.

#### Structura folderelor
> Fiecare folder conține codul sursă împreună cu o diagramă a proiectului și o imagine a interfeței web 
+ client_distance_rfid
    * client_distance_rfid.ino
    * client_rfid_diagram.png
    * client_distance_rfid.jpg
+ watering_system
    * watering_system.ino
    * watering_system.jpg
    * watering_system_diagram.png
+ webserver_dht_security
    * thermometer_security.ino
    * thermometer_security.jpg
    * thermometer.jpg
    * webpage.jpg

### Apasă pe imagini pentru a vedea videclipurile

>Sistem automat de udarea a plantelor cu monitorizare online
>[![N|Solid](https://i.imgur.com/Vhk8M5m.jpg)](https://www.youtube.com/watch?v=zv4vC909660)

>Sistem de securitate și informare sub forma unui termometru cu interfață web
>[![N|Solid](https://i.imgur.com/2ZBWQZN.jpg)](https://www.youtube.com/watch?v=UYCYDYaNyco)