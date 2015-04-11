# Arduino Sky Safari Mission #1 (ASSM1) #
## High Altitude Balloon. ##

For all information about this project, go to: [www.anerdev.net/project/assm/](http://www.anerdev.net/assm1)

In the folder "assm1" you can found the "assm1.ino" Arduino's sketch file.

This code is based on this hardware configuration:
- [Arduino Mega 2560](http://arduino.cc/en/Main/ArduinoBoardMega2560);
- [Futura Elettronica SD Shield](https://www.futurashop.it/index.php?route=product/product&filter_name=arduino+sd&page=2&product_id=4723);
- [Futura Elettronica GSM/GPS Shield V2 with SIM908](https://www.futurashop.it/index.php?route=product/product&filter_name=gsm+shield&product_id=334);
- [NTX2 Radio Transmitter](http://www.radiometrix.com/content/ntx2).
- [DS18B20 temperature sensor](http://www.adafruit.com/product/374);
- [DHT22 humidity sensor](http://www.adafruit.com/products/385);
- [MPL115A2 atmospheric pressure sensor](http://www.adafruit.com/products/992);

With this software version:
- Arduino IDE = [version 1.0](http://arduino.cc/en/Main/OldSoftwareReleases);
- OneWire = [version undefined](http://playground.arduino.cc/Learning/OneWire);
- SdFatLib = [version 20130629](https://code.google.com/p/sdfatlib/);
- GSM/GPS GSMLib Library = [version 305](http://www.gsmlib.org/download.html);
- NTX2 = [version undefined by UKHas Community](http://ukhas.org.uk/guides:linkingarduinotontx2);
- DS18B20 = [version 3.7.2 by Miles Burton](http://milesburton.com/Dallas_Temperature_Control_Library);
- MPL115A2 = [version uundefined by Misenso](https://github.com/misenso/MPL115A2-Arduino-Library);
- DHT22 = [version 0.5 by Nethoncho](https://github.com/nethoncho/Arduino-DHT22);

## IMPORTANT ! ##

The libraries that you can found in the folder "libraries" are already configured for work with this project;
you must import these libraries into your Arduino library folder ([for more info click here](http://arduino.cc/en/Guide/Libraries)).
Only the "GSMSHIELD" library is amended; the other libraries are there for a "fast download" of all. 
More information on: https://github.com/anerDev/ASSM1/wiki/Libraries-setting

## LICENSE ##
- The "assm1.ino" is under "MIT 2.0";
- The libraries are under the proprietary license.
