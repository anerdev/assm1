/* 
Arduino Sky Safari Mission #1

Software sketch version 1.0
Software information: https://github.com/anerdev/assm1/wiki/Software

made by: anerdev
https://github.com/anerdev/assm1
http://www.anerdev.net/assm1

LICENSE
- The "assm1.ino" is under "The MIT License (MIT)";
- The libraries are under the proprietary license.

*/

//General declaration
#include "Wire.h"
#include "OneWire.h"
#include "SoftwareSerial.h"
int counter; 

//Declaration NTX2
#include "string.h"
#include "util/crc16.h"
#define ntx2 2
char rtty[100];

//Declaration DS18B20
#include "DallasTemperature.h"
#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
float temperature;
int temperatureint;

//Declaration DHT22
#include "DHT22.h"
#define DHT22_PIN 4
DHT22 sensoreDHT22(DHT22_PIN);
float humidity;
int humidityint;

//Declaration Led
#define led 5

//Declaration Siren
#define siren 6

//Declaration SD
#include "SD.h"
#define sdpin 9
File sdfile;

//Declaration MPL115A2
#include "MPL115A2.h"
float pressure;
int pressureint;

//Declaration GSM/GPS
#include "SIM900.h"
#include "inetGSM.h"
#include "sms.h"
#include "gps.h"
InetGSM inet;
SMSGSM sms;
GPSGSM gps;
int altint;
char sendsms[50];
char lon[15];
char lat[15];
char alt[15];
char time[20];
char vel[15];
char stat;
boolean started=false;
char YY[5]; // Year
char MM[3]; // Month
char DD[3]; // Day
char hh[3]; // Hour
char mm[3]; // Minute
char ss[3]; // Second

void setup() {

  //Setting PIN
  pinMode(ntx2, OUTPUT); //NTX2
  pinMode(sdpin, OUTPUT); //SD Shield
  pinMode(led, OUTPUT); //Led 
    digitalWrite(led, LOW);
  pinMode(siren, OUTPUT); //siren
    digitalWrite(siren, HIGH); // When is HIGH the piezo acoustic siren is poweroff, when is LOW the piezo acoustic siren is poweron

  //Start NTX2
  timeloop(30,1);

  //Test of NTX2 Transmission
  rtty_txstring("ASSM1 HW CHECK\n");

  //Led
  digitalWrite(led, HIGH);
  rtty_txstring("LED: ON (watch) ");
  digitalWrite(led, LOW);
  timeloop(3,0);

  //Siren
  digitalWrite(siren, LOW);
  rtty_txstring("SIREN: ON (listen) ");
  digitalWrite(siren, HIGH);
  timeloop(3,0);

  //MPL115A2
  MPL115A2.begin(); // Poweron MPL115A2
  MPL115A2.ReadSensor(); // Request the data
  MPL115A2.shutdown(); // Poweoff MPL115A2
  pressure = MPL115A2.GetPressure(); // Save the value of the pressure in the float "pressure"
  pressureint = MPL115A2.GetPressure(); // Save the value of the pressure in the integer "pressureint"
  if (pressureint == 69)  { // If the value is "69" the MPL115A2 is disconnect or doesn't work
    rtty_txstring("MPL115A2: ERROR\n");
  }
  else {
    rtty_txstring("MPL115A2: OK\n");
  };

  //DS18B20
  DS18B20.begin(); // Poweron DS18B20
  DS18B20.requestTemperatures(); // Request the data
  temperature = DS18B20.getTempCByIndex(0); // Save the value of the temperature in the float "temperature"
  temperatureint = DS18B20.getTempCByIndex(0); // Save the value of the temperature in the integer "temperatureint"
  if (temperatureint == -127) { // If the value is "-127" the MPL115A2 is disconnect or doesn't work
    rtty_txstring("DS18B20: ERROR\n");
  }
  else {
    rtty_txstring("DS18B20: OK\n");
  };

  //DHT22
  DHT22_ERROR_t DHT22errorCode; // Initialize the control variable
  delay(2000);
  DHT22errorCode = sensoreDHT22.readData(); // Save the errorCode variable
  switch(DHT22errorCode) {   
    case DHT_ERROR_NONE: // Is all ok
      rtty_txstring("DHT22: OK\n");
      break;
    case DHT_ERROR_NOT_PRESENT: // Sensor disconnected
      rtty_txstring("DHT22: ERROR\n");
      break;
   };
    
  //SD Shield
  if (!SD.begin(4)) { // Initialize the SD communication
    rtty_txstring("SD: ERROR\n");
  }
  else {
    rtty_txstring("SD: OK\n");
  };
  
  // GSM/GPS Shield
  
  // GSM Module
  rtty_txstring("GSM (wait): ");
  if (gsm.begin(2400)) { // Initialize the Seral1 communication
    gsm.forceON(); // Force poweron the GSM module
    started=true;
    rtty_txstring("OK\n");
    sms.SendSMS("33333333", "GSM: OK\n"); // Send sms for test - Put you mobile number without the international prefix
  }
  else {
    rtty_txstring("ERROR\n");
  };

  // GPS Module    
  rtty_txstring("GPS: ");
  if(started) {
    if(gps.attachGPS()) { // Initialize the GPS communication
      rtty_txstring("OK "); 
    }
    else {
      rtty_txstring("ERROR");
    };

    timeloop(20,2);

    stat=gps.getStat();
    if(stat==1) {
      rtty_txstring("NOT FIXED\n"); // GPS doesn't work
    }
    else if(stat==0) {
      rtty_txstring("OFF\n"); // GPS have low power source
    }
    else if(stat==2) {
      rtty_txstring("2D\n"); // GPS receive only lat and lon
    }
    else if(stat==3) {
      rtty_txstring("3D\n"); // GPS receuve lat, lon and alt
    };

    timeloop(5,0);

    gps.getPar(lon,lat,alt,time,vel); // Get values from GPS
    
    sscanf(time,"%04s%02s%02s%02s%02s%02s",&YY,&MM,&DD,&hh,&mm,&ss); // Split the time char and save the data in the char variables - more info: https://gist.github.com/anerdev/b5bf703143ea1d0342e7
    
    // Open the data.csv file
    sdfile = SD.open("data.csv", FILE_WRITE); // Open the data.csv file
    
    if (sdfile) {
      
      sdfile.println();
      
      // Write the time and date of record
      sdfile.print("Start;");
      sdfile.print(hh);
      sdfile.print(":");
      sdfile.print(mm);
      sdfile.print(":");
      sdfile.print(ss);
      sdfile.print(";");
      sdfile.print(DD);
      sdfile.print("/");
      sdfile.print(MM);
      sdfile.print("/");
      sdfile.print(YY);
      sdfile.print(";");
      
      // Write the location of the record
      sdfile.print("From;");
      sdfile.print(lat);
      sdfile.print(";");
      sdfile.print(lon);
      sdfile.print(";");
      sdfile.print(alt);
      sdfile.print(";");

      sdfile.println();
      
      // Write the title of the columns
      sdfile.print("Counter;");
      sdfile.print("Time;");
      sdfile.print("Latitude;");
      sdfile.print("Longitude;");
      sdfile.print("Temperature;");
      sdfile.print("Humidity;");
      sdfile.print("Pressure;");
      
      sdfile.println();
      
      sdfile.close();
     
      snprintf(rtty, 100, "Start: %s:%s:%s - %s/%s/%s\n", hh, mm, ss, DD, MM, YY);  rtty_txstring (rtty);
      snprintf(rtty, 100, "From: %s - %s - %s\n\n", lat, lon, alt);  rtty_txstring (rtty); 
    }
    else { //if SD failed open
      rtty_txstring("SD file: ERROR\n\n");
    };

  }; // End if(sterted)

}; // END Setup

void loop() {

  DS18B20.requestTemperatures(); // Read temperature from DS18B20
  temperature = DS18B20.getTempCByIndex(0); // Save the temperature value into "temperature" float variable
  temperatureint = DS18B20.getTempCByIndex(0); // Save the temperature value into "temperatureint" integer variable

  DHT22_ERROR_t DHT22errorCode; // Read humidity from DHT22
  DHT22errorCode = sensoreDHT22.readData(); // Read the error code of the humidity sensor from DHT22
  switch(DHT22errorCode)
  {
      case DHT_ERROR_NONE: 
      humidity = sensoreDHT22.getHumidity(); // Save the humidity value into "humidity" float variable
      humidityint = sensoreDHT22.getHumidity(); // Save the humidity value into "humidityint" integer variable
      break;
  }

  MPL115A2.ReadSensor(); // Read pressure from MPL115A2
  MPL115A2.shutdown(); // Poweroff the sensor
  pressure = MPL115A2.GetPressure(); // Save the pressure value into "pressure" float variable
  pressureint = MPL115A2.GetPressure(); // Save the pressure value into "pressure" integer variable

  if(started) {
    if(gps.attachGPS()) { // Start GPS Module

      timeloop(15,1);
      stat=gps.getStat();
      gps.getPar(lon,lat,alt,time,vel);
      sscanf(time,"%04s%02s%02s%02s%02s%02s",&YY,&MM,&DD,&hh,&mm,&ss); // Split char "time" to hourse (hh), minute (mm), seconds (ss)

      altint = atoi(alt); // Convert char "alt" to int "altint"
      
      if (altint < 5000) { // If the altitude is < of 5000 meters, send sms
          snprintf(sendsms, 50, "%s - %s - %d", lat, lon, altint);
          sms.SendSMS("3339921212", sendsms);
          digitalWrite(siren, LOW); // Activate the siren allarm
      }
      else {
        digitalWrite(siren, HIGH); // If the altitude is > of 5000 meters, deactivae the siren allarm
      };

      sdfile = SD.open("data.csv", FILE_WRITE); // Save the variable on data.csv into the SD card

      if (sdfile) {

        sdfile.print(counter);      sdfile.print(";");
        sdfile.print(hh);           sdfile.print(":");
        sdfile.print(mm);           sdfile.print(":");
        sdfile.print(ss);           sdfile.print(";");
        sdfile.print(lat);          sdfile.print(";");
        sdfile.print(lon);          sdfile.print(";");
        sdfile.print(alt);          sdfile.print(";");
        sdfile.print(temperature);  sdfile.print(";");
        sdfile.print(humidity);     sdfile.print(";");
        sdfile.print(pressure);     sdfile.println(";");

        sdfile.close();
        
        // Send the radio string, like this: $$ASSM1,1,15:14:46,4444.444444,1111.111111,88,21,30,1000
        snprintf(rtty, 100, "$$ASSM1,%d,%s:%s:%s,%s,%s,%d,%s,%d,%d,%d", counter, hh, mm, ss, lat, lon, altint, vel, temperatureint, humidityint, pressureint);
        unsigned int CHECKSUM = gps_CRC16_checksum(rtty);
        char checksum_str[7];
        sprintf(checksum_str, "*%04X\n", CHECKSUM);
        strcat(rtty,checksum_str);
        rtty_txstring (rtty);

      } 
        else { //if SD failed open
          // Send the radio string, like this: $$ASSM1,1,15:14:46,4444.444444,1111.111111,88,21,30,1000,ERROR
          snprintf(rtty, 100, "$$ASSM1,%d,%s:%s:%s,%s,%s,%d,%d,%d,%d,SD:ERROR", counter, hh, mm, ss, lat, lon, altint, temperatureint, humidityint, pressureint);
          unsigned int CHECKSUM = gps_CRC16_checksum(rtty);
          char checksum_str[7];
          sprintf(checksum_str, "*%04X\n", CHECKSUM);
          strcat(rtty,checksum_str);
          rtty_txstring (rtty);
      }; // END Save the variable on data.csv into the SD card

    }; // END if(gps.attachGPS())
  }; // END if(started)

  counter++; // Increase of 1 the counter value

}; // END void loop()

// Start GPS
void serialswread() {
  gsm.SimpleRead();
}
// END GPS

// Start Time loop
/*
timeloop(x,y);
x = time in second;
y = 0 for "new line", 1 for "tab", 2 for space.
*/
void timeloop(int timeloopvalue, int timeloopcr) {
  int timeloopcount;
  
  for (timeloopcount = 0; timeloopcount < timeloopvalue; timeloopcount++) {
    rtty_txstring("|");
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
  };
    
  switch (timeloopcr) {
    case 0:
      rtty_txstring("\n");
      break;
    case 1:
      rtty_txstring("\t");
      break;
    case 2:
      rtty_txstring(" ");
      break;
   };
};
// END Time loop

//NTX2 Instruction set
// 50 baud, 7 bits ASCII 2 stop bits.
void rtty_txstring (char * string) {
    char c;
    c = *string++;
    
    while ( c != '\0') {
        rtty_txbyte (c);
        c = *string++;
    };
};

void rtty_txbyte (char c) {    
    int i;
    rtty_txbit (0);

    for (i=0;i<7;i++) {
        if (c & 1) rtty_txbit(1);
        else rtty_txbit(0);
        c = c >> 1;
    };
    
    rtty_txbit (1);
    rtty_txbit (1);
};

void rtty_txbit (int bit) {
    if (bit) {
        digitalWrite(ntx2, HIGH);
    }
      else {
        digitalWrite(ntx2, LOW);
    };
    
    delayMicroseconds(10000);
    delayMicroseconds(10150);
    
}

uint16_t gps_CRC16_checksum (char *string) {
    size_t i;
    uint16_t crc;
    uint8_t c;
    
    crc = 0xFFFF;
    
    for (i = 2; i < strlen(string); i++) {
        c = string[i];
        crc = _crc_xmodem_update (crc, c);
    };
    
    return crc;
}
//END NTX2 Instruction set
