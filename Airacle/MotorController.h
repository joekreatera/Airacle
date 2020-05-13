/*
   Motor controller
*/
#define enA 8
#define in1 9
#define in2 10
#define humOut A4
/*
  End motor controller
*/

bool humidifierOn = false;

void setupActuators(){
      // Motor controller
    pinMode(enA, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    // humidifier
    //pinMode(humOut, OUTPUT);
  }
void controlMotor(int direction, int speed) {

  if ( direction == 2) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }

  analogWrite(enA, speed);
}

void setHumidifierOutput(int p, bool on){
  if( on ){
    analogWrite(p,255);
  }  else{
    analogWrite(p,0);
  }
}

bool toggleHumidifier(){
  humidifierOn = !humidifierOn;

  if(humidifierOn){
    setHumidifierOutput(humOut, true);
  }else{
    setHumidifierOutput(humOut, false);
   }
  
  return humidifierOn;
}

void setHumidifier(bool on){
  humidifierOn = on;

  if(humidifierOn){
    setHumidifierOutput(humOut, true);
  }else{
    setHumidifierOutput(humOut, false);
   }
}

void controlActuators(int direction, int speed, bool onOff){
     controlMotor(direction,speed);
     setHumidifier(onOff);
}
