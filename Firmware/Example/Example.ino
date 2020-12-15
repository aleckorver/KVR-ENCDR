#include "melexis.h"
#include "CanNodeSimple.h"
#include "CanConvert.h"

#define mlxCS 10
#define canCS 11
#define canIRQ 13
#define canID 90
#define led1 8
#define led2 9
uint8_t data[8];
const float alphaToDegConst = 45.50834;
MELEXIS digitalSensor(mlxCS);
CanNodeSimple node(canID,canCS,canIRQ);
//CANCONVERT convertCan;
int rawSensorDiag1;
int rawSensorDiag0;

//*******************SETUP********************
void setup() {
Serial.begin(115200);
delay(5000);
pinMode(led1,OUTPUT);
pinMode(led2,OUTPUT);
pinMode(canCS,OUTPUT);
node.Setup(true);
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

//*******************MAIN LOOP*****************
long prevT = 0;
int updateInterval = 100;
bool invert = false;
void loop() {
  blinkLED();
}
//*******************MAIN LOOP*****************

void blinkLED(){
long now = millis();
if(now - prevT >= updateInterval){
  prevT = now;
  getSensorVal();
  sendMsgs(rawSensorDiag1);
  if(invert == false){
    invert = true;
    digitalWrite(led1,HIGH);
  }
  else if(invert == true){
    invert = false;
    digitalWrite(led1,LOW);
    }
  }  
}

void getSensorVal(){
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
for (int i = 0; i <= 30; i++) {
    int digitalSensorMsgType = digitalSensor.get_footer();
    if (digitalSensorMsgType <= 191) {
      rawSensorDiag1 = digitalSensor.get_diag_1();
      rawSensorDiag0 = digitalSensor.get_diag_0();
    }  
  }
  Serial.println();
  Serial.print("Diag 1: ");
  Serial.print(rawSensorDiag1);
  Serial.print("  DEG:  ");
  Serial.print(rawSensorDiag1/alphaToDegConst);
  calcRotationalSpeed(rawSensorDiag1/alphaToDegConst);
  getSensorError();
}

void getSensorError(){
  uint8_t sensorError2 = digitalSensor.get_diag_details2(); //AND 12 or 0xC;
  uint8_t fieldStrengthLow = sensorError2 & 4;
  uint8_t fieldStrengthHigh = sensorError2 & 8; 
  Serial.print("  ERROR: ");
  Serial.print(sensorError2);
  if(fieldStrengthLow > 0){
    Serial.print("FIELD STRENGTH TOO LOW");
    digitalWrite(led2,HIGH);
  }
  else if(fieldStrengthHigh >0){
    Serial.print("FIELD STRENGTH TOO HIGH");
    digitalWrite(led2,LOW);
  }
  else{
    digitalWrite(led2,LOW);
  }
}

float _prevAngle = 0;
long prevTime = 0;
long updateSpeedInterval = 50;

//TODO DEAL WITH ROLLOVER?
int calcRotationalSpeed(float _angle){
  long _now = millis();
  if(_now - prevTime >= updateSpeedInterval){
    float angleDelta = abs(_prevAngle - _angle);
    if(angleDelta >= 300){
      angleDelta = 360 - abs((_prevAngle - _angle));
    }
    long timeDelta = _now - prevTime;
    prevTime = _now;
    float RPM = (((angleDelta/timeDelta))); //Rotations/Min
    Serial.println();
    Serial.print("RPM: ");
    Serial.print(((angleDelta/timeDelta)*60000)/360);
    _prevAngle = _angle;
  }
}
void sendMsgs(int buff){
  //node.SendBuffer(uint32_t id, int len, uint8_t *data)
  int _len = 8;
  uint8_t top4bits = buff & 0xF00;
  uint8_t calcLSB = buff & 0xFF; // get LSB byte
  uint8_t calcMSB = buff >> 8;
  //convertCan.SerializeInt32Be(int32_t v, int bit_offset, uint8_t *data)
  Serial.println();
  Serial.print(" DATA: ");
  Serial.print(top4bits);
  Serial.print("  ");
  Serial.print(calcLSB);
  data[0] = calcLSB;
  data[1] = calcMSB;
  //Serial.println();
  //Serial.print("DATA0: ");
  //Serial.print(data[0]);
  //Serial.print("  DATA1: ");
  //Serial.print(data[1]);
  //uint8_t _buff = "hello";//(uint8_t)rawSensorDiag1;
  uint32_t rxID = 80;
  node.SendBuffer(rxID, _len, data);
  node.CanReceive();
}

//int WriteTxThumbwheelFrame(const TxThumbwheelFrame *m, uint8_t *data) {
//  memset(data, 0, 8);
//  SerializeBool(m->error_mlx, 0, data);
//  SerializeBool(m->error_mlx_range, 1, data);
//  SerializeBool(m->error_mlx_error_bit0, 2, data);
//  SerializeBool(m->error_mlx_error_bit1, 3, data);
//  SerializeBool(m->error_no_rx_ack, 4, data);
//  SerializeBool(m->error_rx_interference, 5, data);
//  SerializeInt16(m->flux_x, 16, data);
//  SerializeInt16(m->flux_y, 32, data);
//  SerializeInt16(m->flux_z, 48, data);
//  return 8;
//}
