  /*
 Name     :     E-bandage
 Created  :	    November 15, 2019 
 Sensor   :   
 Authors  :	    Clinton Oduor, Jackline Tum & Immaculate Maiyo
*/

#include <SigFox.h>
#include <ArduinoLowPower.h>
#include <Arduino.h>

const int oxysensor = 2;
const int tempsensor = 4;
const int phsensor = 5;
const int lactosesensor = 6;
const int levelsensor =3;


int oxygen;
//oxygen concentration

int lactane
//lactane concentration

int ph;
//pH level

int level;
//drug level

int temp;
//temperature concentration for swell analysing

void setup() 
{
   // begin transmission
   Serial.begin(9600);
   pinMode(oxysensor, INPUT);
   pinMode(tempsensor, INPUT); 
   pinMode(phsensor, INPUT);
   pinMode(lactosesensor, INPUT); 
   pinMode(levelsensor, INPUT);

}

void loop() 
{
  // all parameters being monitored
  char parameters; 
  while (Serial.available() > 0) 
  {
    
    // read data byte by byte and store it
    parameters = parameters + (char)Serial.read(); 
  }
  sendDataToSigFox(parameters)
}

void getDeviceId()
{ 
  Serial.begin(9600);

  while(!Serial) {};

  if (!SigFox.begin())
  {
    Serial.println("Shield error or not present!");
  }
     //Run This funtion to get the ID and PAC after which yoi can safely comment
   // getDeviceId();

    String version = SigFox.SigVersion();
    String ID = SigFox.ID();
    String PAC = SigFox.PAC();

    // Display module informations
    Serial.println("MKRFox1200 Sigfox first configuration");
    Serial.println("SigFox FW version " + version);
    Serial.println("ID  = " + ID);
    Serial.println("PAC = " + PAC);

    Serial.println("");

    Serial.println("Register your board on https://backend.sigfox.com/activate with provided ID and PAC");

    delay(100);

    // Send the module to the deepest sleep
    SigFox.end();

  return;  
}

void sendDataToSigFox(char animalName)
{
  /*
  ATTENTION - the structure we are going to send MUST
  be declared "packed" otherwise we'll get padding mismatch
  on the sent data - see http://www.catb.org/esr/structure-packing/#_structure_alignment_and_padding
  for more details
  */

  typedef struct __attribute__ ((packed)) sigfox_message {
  char name;
  uint8_t lastMessageStatus;
  } SigfoxMessage;

  // stub for message which will be sent
  SigfoxMessage msg;

  if (!SigFox.begin()) {
      Serial.println("Shield error or not present!");
      return;
  }

  //start the module
  SigFox.begin();

  // Wait at least 30ms after first configuration (100ms before)
  delay(100);

  msg.name=animalName;        

  SigFox.status();
  delay(1);

  SigFox.beginPacket();
  SigFox.write((uint8_t*)&msg,12);

  msg.lastMessageStatus=SigFox.endPacket();

  Serial.println("status: "+ String(msg.lastMessageStatus));  
  Serial.println(String(msg.name)); 
}

