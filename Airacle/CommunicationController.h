#include <SoftwareSerial.h>


// softwarte serial for MCU connection
SoftwareSerial s(5, 6);
#define COMM_MCU  1  // 1 do comm, 0 do not


String last_full_status = "-1:-1:-1:-1";

// more than a server , it will ask the outer world and send messages

void setupComms() {
  s.begin(19200);
}


// eventually it will become selected function 
int f = 0; // variable for controlling LED status from server

void commMcu() {
  int data = f;

  //Serial.println( s.available() );

  int sent = 0;
  while (s.available())
  {
    //s.write(data);
    int c = s.read();
    // for function 1 and 4 protocol:
    /*
   They have two numbers to activate (1 and 6) and (4 and 7)
    */
    if ( c >= 0 && c < 8 ) {
      Serial.println("Mode from MCU");
      Serial.println(c, DEC);
      f = c;
    }
    if ( sent == 0 ) {
      String data = "";
      data += String(gas) + ":" + String(temp) + ":" + String(humidity) + ":" + String(dust) + "F";
      s.println(data);
      sent = 1;
    }
  }
}

int getActualFunction(){
  return f;
}

void forceFunctionTo(int ff){
  f = ff;
}

void sendAndReceiveComms() {

  if ( COMM_MCU == 1) {
    commMcu();
  }
}
