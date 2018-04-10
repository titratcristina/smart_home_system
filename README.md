# smart_home_system

## Descriere proiect 
Un mic sistem automatizat pentru ușurarea activităților casnice și securizarea locuinței.

Pentru partea de securizare și monitoarizare a temperaturii am folosit: 
- două plăcuțe NodeMCU cu modul WiFi, una este server web, iar cealaltă reprezintă un client. 
    - La prima plăcuță, care este un server web am adăugat: 
        - un senzor de temperatură și umiditate DHT11
        - un LCD 16x2 care se conectează prin I2C
        - un buzzer 
        - un servomotor
    - Iar la plăcuța client am folosit: 
        - un modul RFID 
        - un senzor ultrasonic de distanță HC-SR04
> Am vrut inițial să am o monitorizare web a temperaturii și a umidității cu o afișare a acestora pe LCD, dar pe parcurs am adăugat la plăcuța server buzzerul care avertizează când cineva este în apropiere și servomotorul care ține locul unei încuietori electrice (senzorul HC-SR04 este conectat la plăcuța client care comunica cu serverul, accesând pagina care pornește "alarmă"/activează buzzerul când cineva se apropie și se efectuează un joc pe LCD, acesta se stinge și se aprinde, iar servomotorul este activat când cineva apropie un card care are acces)

Pentru partea automatizată de udat plantele am folosit:
- o plăcuță Arduino Nano si una NodeMCU cu modul WiFi
    - plăcuța Nano este conectată la:
        - o pompa
        - un senzor de umiditate a solului
        - un releu 
    - cealaltă are doar senzorul de umiditate a solului
> Am procedat în acest mod, din cauză că nu am reușit să pun tot proiectul pe NodeMCU (releul si wifi-ul consumau foarte mult) și am încărcat codul care pornește pompa când umiditatea este scăzută pe Nano, iar interfața web am facut-o pe NodeMCU


### Apasă pe imagini pentru a vedea videclipurile

>Sistem automat de udarea a plantelor cu monitorizare online
>[![N|Solid](https://i.imgur.com/Vhk8M5m.jpg)](https://www.youtube.com/watch?v=zv4vC909660)

>Sistem de securitate și informare sub forma unui termometru cu interfață web
>[![N|Solid](https://i.imgur.com/2ZBWQZN.jpg)](https://www.youtube.com/watch?v=UYCYDYaNyco)

#### Structura folderelor
> Fiecare folder conține codul sursă împreună cu o diagramă a proiectului și o imagine a interfeței web 
+ client_distance_rfid
    * client_distance_rfid.ino
    * nodemcu_distance_rfid.jpg
      ![](https://i.imgur.com/dD732ap.jpg)
+ watering_system
    * watering_system.ino
    * water_pump.jpg
      ![](https://i.imgur.com/ZqSDQRj.jpg)
+ webserver_dht_security
    * webserver_dht_security.ino
    * thermometer.jpg
      ![](https://i.imgur.com/v5MkN85.jpg)
    * webpage.jpg
      ![](https://i.imgur.com/FS52cH7.jpg)
+ webserver_soil
    * webserver_soil.ino
    * soil_moisture.jpg
      ![](https://i.imgur.com/3t4hJoC.jpg)
    * soil.png
      ![](https://i.imgur.com/gIIUktW.png)