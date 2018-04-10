# smart_home_system

## Descriere proiect 
Un mic sistem automatizat pentru usurarea activitatilor casnice si securizarea locuintei.

Pentru partea de securizare si monitoarizare a temperaturii am folosit: 
- doua placute NodeMCU cu modul WiFi, una este server web, iar cealalta reprezinta un client. 
    - La prima placuta, care este un server web am adaugat: 
        - un senzor de temperatura si umiditate DHT11
        - un LCD 16x2 cu comunicare I2C
        - un buzzer 
        - un servomotor
    - Iar la placuta client am folosit:
        - un modul RFID 
        - un senzor ultrasonic de distanta HC-SR04
> Am vrut initial sa am o monitorizare web a temperaturii si a umiditatii cu o afisare a acestora pe LCD, dar pe parcurs am adaugat la placuta server buzzerul care avertizeaza cand cineva este in apropiere si servomotorul care tine locul unei incuietori electrice (senzorul HC-SR04 este conectat la placuta client care comunica cu serverul, accesand pagina care porneste "alarma"/activeaza buzzerul cand cineva se apropie si se efectueaza un joc pe LCD, acesta se stinge si se aprinde, iar servomotorul este activat cand cineva apropie un card care are acces)

Pentru partea automatizata de udat plantele am folosit:
- o placuta Arduino Nano si una NodeMCU cu modul WiFi
    - placuta Nano este conectata la:
        - o pompa
        - un senzor de umiditate a solului
        - un releu 
    - cealalta are doar senzorul de umiditate a solului
> Am procedat in acest mod, din cauza ca nu am reusit sa pun tot proiectul pe NodeMCU (releul si wifi-ul consumau foarte mult) si am incarcat codul care porneste pompa cand umiditatea este scazuta pe Nano, iar interfata web am facut-o pe NodeMCU


### Apasa pe imagini pentru a vedea videclipurile

>Sistem automat de udarea a plantelor cu monitorizare online
>[![N|Solid](https://i.imgur.com/Vhk8M5m.jpg)](https://www.youtube.com/watch?v=zv4vC909660)

>Sistem de securitate si informare sub forma unui termometru cu interfata web
>[![N|Solid](https://i.imgur.com/2ZBWQZN.jpg)](https://www.youtube.com/watch?v=UYCYDYaNyco)

#### Structura folderelor
> Fiecare folder contine codul sursa impreuna cu o diagrama a proiectului si o imagine a interfetei web       
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