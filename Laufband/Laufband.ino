//www.elegoo.com
//2016.06.13

#include <NewPing.h>

double getDistance(NewPing sonar) {
  double t = sonar.ping_median(3);    // m/s   for T=20°C, c is p independent!  delta d = 5mm for delta T = 5°C , and 2mm for 2°C resp. Sytematic error (e.g. some electr. delays has still to be checked)
  return 343.21*t*1.0e-3/2;
}

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
float d;

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
}

void loop() {
  delay(30);                         // Wait 500ms between pings (about 2 pings/sec). 29ms should be the shortest delay between pings.
  d = getDistance(sonar);
  Serial.print(d); 
  Serial.println("mm");
}
