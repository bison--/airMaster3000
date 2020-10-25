# airMaster3000
a ready to use CO2 sensor I build for [JujiBla](https://twitter.com/JujiBla) to use in classrooms

![final build sensor assembled](https://pbs.twimg.com/media/Ekm3o0IX0AAJO8t?format=jpg)

This is my first ever finished and end-user ready microcontroller project ever, so bear with me about the actual build quality ^^  
To not spam the repo I might write a blogpost about the build process with more pictures (link will be HERE then).  
Short tweet about the process: https://twitter.com/bison_42/status/1317786502701645824

# Software

I used the arduino IDE and these libraries (read more about that here: https://www.arduino.cc/en/Guide/Libraries)

* LiquidCrystal 1.0.7 (build in) https://www.arduino.cc/en/Reference/LiquidCrystal
* Adafruit_CCS811 1.0.3 https://github.com/adafruit/Adafruit_CCS811

## Parts

I used (not exactly all those parts but equal): 

Name | link
---- | ----
Joy-IT CCS811 SENSOR (Luftqualitätsmesser mit I2C) | https://joy-it.net/de/products/SEN-CCS811V1 <br> https://www.conrad.de/de/p/joy-it-sen-ccs811v1-sensor-modul-1884871.html
Joy-IT 2,6" LCD Display, 16x2 (with I2C) | https://joy-it.net/de/products/SBC-LCD16x2 <br> https://www.conrad.de/de/p/joy-it-sbc-lcd16x2-display-modul-6-6-cm-2-6-zoll-16-x-2-pixel-passend-fuer-raspberry-pi-arduino-banana-pi-cubieboar-1503825.html
AZDelivery Nano V3.0 CH340 Chip, Ready Soldered Parent | https://www.amazon.de/AZDelivery-Nano-CH340-Soldered-Parent/dp/B0755S95F3/
diymore Nano I/O Extension Sensor Shield Module for Arduino UNO R3 Nano V3.0 | https://www.amazon.de/gp/product/B07D5522B6
Donau Elektronik KGB15 Euro Box Small, Black, 95 x 135 x 45 cm | https://www.amazon.de/gp/product/B005DRPCBW
TRU COMPONENTS 1PU Toggle switch (Kippschalter) 250 V/AC 1.5 A 1 x Ein/Ein rastend 1 St. | https://www.conrad.de/de/p/tru-components-1pu-kippschalter-250-v-ac-1-5-a-1-x-ein-ein-rastend-1-st-1564808.html
UNITEC 40621 Lustre Terminal 12 x 1.5-2.5 mm Pack of 10 | https://www.amazon.de/-/en/UNITEC-40621-Lustre-Terminal-1-5-2-5/dp/B007CWCQ74/
AVERY Zweckform L6009-20 Nameplate Foil Labels (45.7 x 21.2 mm on DIN A4 | https://www.amazon.de/gp/product/B0002S4DIU
random RGB-Led | ...
USB 2.0 CON.CABLE.TYPEA-MINI B (5PIN)1M | ...
Div. Jumperwire | ...
Div Wires | ...
solder | ...
hot glue | ...

## IMPROVEMENTS / PROBLEMS

While building this I discovered the library has special functions to set temperature, humidity and air preasue.  
To increase accuracy you really should add those 3 external sensors too. The integrated temperature sensor is not really useable for calibration, since it seems to use ITS temperature and not the surrounding air temperature (what's kinda obvious, because it has to warm up). Those sensors are available with an I2C interface, so you can add them directly to the existing bus.  
I tested it against my bought CO2 sensor and for environments like Cafés you really need a CO2 ONLY sensor. In fact the CCS811 goes "haywire" when more is in the air than CO2, like making a coffee or it gets close to a source like a surface desinfect. But it seems fine for environments like classrooms.

**IMPORTANT**: You REALLY should read the CCS811 datasheet! The sensor has to run for a few days to get readings that are not all over the place (they call it: burn-in).

## QA

* Q: Why Joy-IT?
* A: Because CO2 sensors are a Corona product and I wanted a REAL sensor and not a fake one. It is a trustworthy supplier and their manuals and datasheets are awesome.

## Schematics & Pictures

Thanks to [PlantProgrammer](https://plantprogrammer.com) for making the schematics 💖 .
![schematics](https://github.com/bison--/airMaster3000/blob/main/CO2_Sensor_LCD_bb.png)
![final build sensor inner parts](https://pbs.twimg.com/media/Ekm3mngXEAICSXH?format=jpg)
