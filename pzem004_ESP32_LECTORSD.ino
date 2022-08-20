#include <PZEM004Tv30.h>


/*
  SD card test for esp32
 
  This example shows how use the utility libraries
 
  The circuit:
    SD card attached to SPI bus as follows:
        SS    = 5;
        MOSI  = 23;
        MISO  = 19;//
        SCK   = 18;
 
 
*/



//NEWS
#include "FS.h"
#include "SD.h"
#include "SPI.h"



const int chipSelect = SS;//10;



PZEM004Tv30 pzem(Serial2, 16, 17);


// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_DS3231 rtc;

 
#include "TimeLib.h"


tmElements_t my_time;  // time elements structure
time_t unix_timestamp; // a timestamp

double milliseconds=0;
float unix_timestamp10;
float pp=0;
int64_t  timestamp13=0;
String Nombrearchivo;
String ano;
String dia;
String mes;
String horas;
String minutos;
String segundos;
String Fecha;
String Hora;
String Imprimir;


void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.println("LISTDIR.....");
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);


Serial.println("WRITEFILE.....");
  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.println(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
//insertar una linea en blanco
 if(file.println("")){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){

  Serial.println("APPENDFILE.....");
  
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.println(message)){
      Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}



void setup() {
   // Serial.begin(115200);
      Serial.begin(9600);
      
     //Serial.begin(2000000);
    // Uncomment in order to reset the internal energy counter
     pzem.resetEnergy();


     if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

Serial.print("Inicializando tarjeta SD...");
  delay(2000);

  if(!SD.begin(SS)){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

delay(2000);
 Serial.println("TIPO DE TARJETA...");
  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");  //ESTA
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }


delay(2000);
 Serial.println("TAMAÑO TARJETA...");
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);  //1910MB



Serial.println("LISTADO ANTES.....");
  listDir(SD, "/", 0);
delay(2000);

Serial.println("WRITEFILE.....");
  delay(2000);


    
 DateTime now = rtc.now();
 ano = now.year();
 mes = now.month();
 dia = now.day();
 horas = now.hour();
 minutos = now.minute();
 segundos = now.second();
 Fecha = ano + "/" + mes + "/" + dia;
 Hora = horas + ":" + minutos + ":" + segundos;
 /////////////////////////////////////////////
 Nombrearchivo ="/"+ ano + mes + dia + horas+ minutos+ segundos +".txt";

delay (2000);
Serial.print("fichero nuevo:");
Serial.println(Nombrearchivo);

/////////////////////////////////////////////
String    Imprimir2 = "TS;V;I;W;F;CF";


char Buf[20];
Imprimir2.toCharArray(Buf, 50);
char Buf2[20];
Nombrearchivo.toCharArray(Buf2, 50);
  
writeFile(SD, Buf2, Buf); 

Serial.println("LISTADO DESPUES.....");
listDir(SD, "/", 0);
delay(2000);
}





void loop() {
        
   // Serial.print("Custom Address:");
   // Serial.println(pzem.readAddress(), HEX);

    // Read the data from the sensor
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();
   
      
    DateTime now = rtc.now();
   /* each sec update the display */
  my_time.Year = now.year()- 1970; //time since 19700
  my_time.Month = now.month() - 1; // months sice January
  my_time.Day=now.day();
  my_time.Hour = now.hour(); // hourse
  my_time.Minute = now.minute(); // minutes
  my_time.Second = now.second(); // seconds
  
   unix_timestamp =  makeTime(my_time);
//  Serial.print("Seconds since 1900: ");
 // Serial.print(unix_timestamp);
 // Serial.print(";");

    if (milliseconds == 999)  // roll over to zero
    {    milliseconds = 0;
    }
    else
    {   ++milliseconds;
    }

String kk;

kk=unix_timestamp;

float pp=kk.toInt();
  
timestamp13=1000*pp + milliseconds;
  


Serial.print(timestamp13);
Serial.print(";");
String p1=";"; 
Serial.println ((String)voltage+p1+(String)power+p1+(String)energy+p1+(String)frequency+p1+(String)pf);
  
 /////////////////////////////////////////////
 Imprimir = timestamp13 + ";" + (String)voltage + ";" + (String)power +";"+ (String)energy + ";" + (String)frequency +";"+ (String)pf;



char Buf[50];
Imprimir.toCharArray(Buf, 50);
char Buf2[20];
Nombrearchivo.toCharArray(Buf2, 50);
appendFile(SD, Buf2, Buf);

 
}
