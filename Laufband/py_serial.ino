
#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define POWER_PIN    5   // Pin tied to relays to supply motor
#define BACKWARD_PIN 6   // Pin tied to relays to switch polarities
#define MAX_DISTANCE 300 // Maximum distance. Maximum sensor distance is rated at 400-500cm.
#define TOLERANCE    5   // Tolerance between wanted and actual position in mm
#define MEDIAN       5   // Number of measurements to take mean
#define SONICSPEED   343.21 // Sonic speed


short getDistance(NewPing sonar) {
  double t = sonar.ping_median(MEDIAN);    // m/s   for T=20°C, c is p independent!  delta d = 5mm for delta T = 5°C , and 2mm for 2°C resp. Sytematic error (e.g. some electr. delays has still to be checked)
  return (short) SONICSPEED*t*1.0e-3/2;
}

short readSerial() {
  byte Nbytes = Serial.available();
  char input[ Nbytes ];
  Serial.readBytes(input, Nbytes);
  return atoi(input);
}

void write_pos(short wanted_pos, short actual_pos) {
    Serial.print(wanted_pos);
    Serial.print(" mm - ");
    Serial.print(actual_pos);
    Serial.print("mm\n");
}

void driveBack(short actual_pos, short wanted_pos, NewPing sonar) {
        digitalWrite(BACKWARD_PIN,HIGH);
        delay(50);
        digitalWrite(POWER_PIN,HIGH);
        
        while (actual_pos > wanted_pos ) {
          actual_pos = getDistance(sonar);
          write_pos(wanted_pos, actual_pos);
          if (actual_pos < TOLERANCE) break;    // break if it comes to end
        }
        digitalWrite(POWER_PIN,LOW);
        delay(50);
        digitalWrite(BACKWARD_PIN,LOW);
}

void driveForward(short actual_pos, short wanted_pos, NewPing sonar) {
        digitalWrite(POWER_PIN,HIGH);
        
        while (actual_pos <= wanted_pos ) {
          actual_pos = getDistance(sonar);
          write_pos(wanted_pos, actual_pos);
          if (actual_pos > (MAX_DISTANCE - TOLERANCE)) break;  // break if it comes to end
        }
        digitalWrite(POWER_PIN,LOW);
}

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
short actual_pos;
short wanted_pos;
short diff_pos;

void setup() 
{
  pinMode(POWER_PIN,OUTPUT);
  pinMode(BACKWARD_PIN,OUTPUT);  
  digitalWrite(POWER_PIN,LOW);
  digitalWrite(BACKWARD_PIN,LOW);  
  Serial.begin(9600);
}

void loop() 
{
  if(Serial.available() > 0)
  { 
    actual_pos = getDistance(sonar);
    wanted_pos = readSerial();
    if (wanted_pos > MAX_DISTANCE) {
      Serial.print("Actual Pos: ");
      Serial.print(actual_pos);
      Serial.print("mm\n");      
    }
    else {
      write_pos(wanted_pos, actual_pos);
      diff_pos = actual_pos - wanted_pos;
      
      while (abs(diff_pos) > TOLERANCE) {
        
        if (actual_pos > wanted_pos) {
          Serial.print("to high\nSTART - go backward\n---------\n");
          driveBack(actual_pos, wanted_pos, sonar);
          digitalWrite(BACKWARD_PIN,HIGH);
          Serial.print("---------\nSTOP - go backward\n");
        }
        else {
          Serial.print("to low\nSTART - go forward\n---------\n");
          driveForward(actual_pos, wanted_pos, sonar);
          Serial.print("---------\nSTOP - go forward\n");
        }
        actual_pos = getDistance(sonar);
        diff_pos = actual_pos - wanted_pos;
      } 
    }
    if (abs(diff_pos) <= TOLERANCE) {
      Serial.print("\nNothing to do\n");
    }
  }  
}
