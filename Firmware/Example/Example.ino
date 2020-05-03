#include "melexis.h"
#define mlxCS 10
#define canCS 11
#define canIRQ 13

MELEXIS digitalSensor(mlxCS);
int rawSensorDiag1;
int rawSensorDiag0;
void setup() {
Serial.begin(9600);
delay(5000);
pinMode(8,OUTPUT);
bool result = false;
  for (int i = 0; i < 3; i++) {
     delay(25); // tSSRERE_mod3 delay for 25ms startup time
     if (digitalSensor.poll(false)) {
      result = false;
        }
      else {
        result = true;
      }
   }
   
Serial.println();
Serial.print("INIT MLX: ");
Serial.print(result);

for (int i = 0; i <= 30; i++) {
    int digitalSensorMsgType = digitalSensor.get_footer();
    if (digitalSensorMsgType <= 191) {
      rawSensorDiag1 = digitalSensor.get_diag_1();
      rawSensorDiag0 = digitalSensor.get_diag_0();
    }
  }
}
long prevT = 0;
int updateInterval = 500;
bool invert = false;
void loop() {
  blinkLED();
}

void blinkLED(){
long now = millis();
if(now - prevT >= updateInterval){
  Serial.println();
  Serial.print("Diag 1: ");
  Serial.print(rawSensorDiag1);
  Serial.print("  Diag0:  ");
  Serial.print(rawSensorDiag0);
  prevT = now;
  if(invert == false){
    invert = true;
    digitalWrite(8,HIGH);
  }
  else if(invert == true){
    invert = false;
    digitalWrite(8,LOW);
    }
  }  
}
