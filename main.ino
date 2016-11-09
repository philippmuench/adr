#include <DHT.h>
#include <SD.h>

// Time module
#include <RTClib.h>
#include <Wire.h>    // I2C-Bibliothek einbinden
RTC_DS1307 RTC;      // RTC Modul
dht DHT;
dht DHT2;

#define DHT11_PIN 4 
#define DHT21_PIN 5
#define DHT22_PIN 6

// second module
#define DHT2_11_PIN 5 
#define DHT2_21_PIN 5
#define DHT2_22_PIN 6

int chk; // first temp module
int chk2; // second temp module
int value; // Store value from photoresistor (0-1023)

const int pResistor = A0; // Photoresistor at Arduino analog pin A0
const int ledPin=9;       // Led pin at Arduino pin 9

File myFile;
 
void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
   if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // Initialisiere I2C  
  Wire.begin();
  
  // Initialisiere RTC
  RTC.begin();
 
  Serial.begin(9600);
  Serial.println("Set Datum und Zeit");
   RTC.adjust(DateTime(__DATE__, __TIME__));
  
  // Prüfen ob RTC läuft  
  if (! RTC.isrunning()) {
    
    // Aktuelles Datum und Zeit setzen, falls die Uhr noch nicht läuft
    RTC.adjust(DateTime(__DATE__, __TIME__));
    
    Serial.println("Echtzeituhr wurde gestartet und auf Systemzeit gesetzt.");
  }
  else Serial.println("Echtzeituhr laeuft bereits.");
 pinMode(ledPin, OUTPUT);  // Set lepPin - 9 pin as an output
 pinMode(pResistor, INPUT);// Set pResistor - A0 pin as an input (optional)
}

void loop()
{
  myFile = SD.open("sensor.txt", FILE_WRITE);
  chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
    Serial.print("Status: OK,\t"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Status: Checksum error,\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Status: Time out error,\t"); 
    break;
    default: 
    Serial.print("Status: unbekannter Fehler,\t"); 
    break;
  }

  chk2 = DHT2.read11(DHT2_11_PIN);
  switch (chk2)
  {
    case DHTLIB_OK:  
    Serial.print("Status 2: OK,\t"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Status 2: Checksum error,\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Status 2: Time out error,\t"); 
    break;
    default: 
    Serial.print("Status 2: unbekannter Fehler,\t"); 
    break;
  }

    DateTime now=RTC.now(); // aktuelle Zeit abrufen
    value = analogRead(pResistor);
 
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing...");
    myFile.print(now.year(),DEC);
    myFile.print(";");
    myFile.print(now.month(),DEC);
    myFile.print(";");
    myFile.print(now.day(),DEC);
    myFile.print(";");
    myFile.print(now.hour(),DEC);
    myFile.print(";");
    myFile.print(now.minute(),DEC);
    myFile.print(";");
    myFile.print(now.second(),DEC);
    myFile.print(";");
    myFile.print(DHT.humidity, 1);
    myFile.print(";");
    myFile.print(DHT.temperature, 1);
    myFile.print(";");
    myFile.print(DHT2.humidity, 1);
    myFile.print(";");
    myFile.print(DHT2.temperature, 1);
    myFile.print(";");
    myFile.println(value);
    digitalWrite(ledPin, HIGH); //Turn led on
    delay(10);
    digitalWrite(ledPin, LOW); //Turn led on
  // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening file");
  }
  
  delay(5000);
}

String get_day_of_week(uint8_t dow){ 
  String dows="  ";
  switch(dow){
   case 0: dows="So"; break;
   case 1: dows="Mo"; break;
   case 2: dows="Di"; break;
   case 3: dows="Mi"; break;
   case 4: dows="Do"; break;
   case 5: dows="Fr"; break;
   case 6: dows="Sa"; break;
  }
  
  return dows;
}


