/*
program ini digunakan untuk project Midi Controller menggunakan capasitive touch pada EspectroBase
dengan komunikasi via WIFI denagn software rtpMIDI.


Created by 29 April 2017
by Hisyam Kamil

referensi project: 
https://github.com/lathoub/Arduino-AppleMidi-Library
https://github.com/adafruit/Adafruit_MPR121

*/

#include "Adafruit_MPR121.h"
#include "Wire.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include "AppleMidi.h"
#include <ESPectro.h>

ESPectro board(ESPectro_V3);
APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI);
Adafruit_MPR121 cap = Adafruit_MPR121();

bool isConnected = false;
char ssid[] = "DyWare-AP2"; //  your network SSID (name)
char pass[] = "957DaegCen";    // your network password (use for WPA, or use as key for WEP)
uint16_t lasttouched = 0;
uint16_t currtouched = 0;
int pitch[] = {36, 38, 42, 46, 44, 55};
int neo[] = {0x0000ff, 0x00ff00, 0xff0000};




void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.print("Getting IP address...");

  WiFi.begin(ssid, pass);
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");


  Serial.println();
  Serial.print("IP address is ");
  Serial.println(WiFi.localIP());
  Serial.println("OK, sekarang buka software rtpMIDI dan pastikan session status : enabled");

AppleMIDI.begin("test");

  AppleMIDI.OnConnected(OnAppleMidiConnected);
  AppleMIDI.OnDisconnected(OnAppleMidiDisconnected);

  AppleMIDI.OnReceiveNoteOn(OnAppleMidiNoteOn);
  AppleMIDI.OnReceiveNoteOff(OnAppleMidiNoteOff);
  
if (!cap.begin(0x5A)) //0x5A alamat i2c MPR121
{
    Serial.println("MPR121 not found, check wiring?");
    while (1);
     }
  Serial.println("MPR121 found!");
}

void loop() {
  // put your main code here, to run repeatedly:

AppleMIDI.run();
currtouched = cap.touched();




 for (uint8_t i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    // note on check
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
       
      AppleMIDI.noteOn(pitch[i], 100, 1);
      
      }
  if ( !(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
  
      AppleMIDI.noteOff(pitch[i], 100, 1);
  board.turnOffNeopixel(i);
  }
  }
  lasttouched = currtouched;
 } 



void OnAppleMidiConnected(uint32_t ssrc, char* name) {
  isConnected  = true;
  Serial.print("Connected to session ");
  Serial.println(name);
}


void OnAppleMidiDisconnected(uint32_t ssrc) {
  isConnected  = false;
  Serial.println("Disconnected");
}

void OnAppleMidiNoteOn(byte channel, byte note, byte velocity) {
  Serial.print("Incoming NoteOn from channel:");
  Serial.print(channel);
  Serial.print(" note:");
  Serial.print(note);
  Serial.print(" velocity:");
  Serial.print(velocity);
  Serial.println();
}


void OnAppleMidiNoteOff(byte channel, byte note, byte velocity) {
  Serial.print("Incoming NoteOff from channel:");
  Serial.print(channel);
  Serial.print(" note:");
  Serial.print(note);
  Serial.print(" velocity:");
  Serial.print(velocity);
  Serial.println();
}
