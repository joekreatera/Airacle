
#include "ledController.h"
#include "Timer.h"
#include "SensorController.h"
#include "MotorController.h"
#include "CommunicationController.h"

#define SENSORS_EVENT 0
#define LED_ACTUATOR_EVENT 1
#define COMMS_EVENT 2
#define CLEAN_ON_EVENT 3

#define F_NONE 0
#define F_1 1
#define F_2 2
#define F_3 3
#define F_4 4
#define F_5 5
#define F_1_BIS 6
#define F_4_BIS 7

#define ACCEPTANCE_COLOR 0x002244

#define IDLE_BREATHE_COLOR 0x000033
#define IDLE_BREATHE_COLOR2 0x110033


int function = 0;
int function_phase = 0;
int actual_work_function = 0; // just an auxiliary int to stop being looped on no loop functions
void setup() {


  Serial.begin(115200);

  setupLED();
  setupTimer();
  setupComms();
  setupSensors();
  setupActuators();


  setEvent(SENSORS_EVENT, 1000);
  setEvent(COMMS_EVENT, 1000);
  setEvent(LED_ACTUATOR_EVENT, 50);

  function = F_NONE;
  actual_work_function = function;
}




int doFunctionOneWithPhase(int ph) {
  switch (ph) {
    case 0:
      setEvent(CLEAN_ON_EVENT, 1000);
      return ph + 1;
      break;
    case 1:
      if ( timerEvent(CLEAN_ON_EVENT) ) {
        setEvent(CLEAN_ON_EVENT, 6000);
        return ph + 1;
      } else {
        showOneColorLED(ACCEPTANCE_COLOR);
      }
      return ph;
      break;
    case 2:
      if ( timerEvent(CLEAN_ON_EVENT) ) { // finished cleaning, this would depend on time and sensors!!!! maybe this would change
        setEvent(CLEAN_ON_EVENT, 4000);
        controlActuators( 1 , 0 ,false);
        return ph + 1;
      } else {
        controlActuators( 1 , 255 ,false);
        showBreathingLED(0x0000FF, 0x000000);
      }
      return ph;
      break;
    case 3:
      if ( timerEvent(CLEAN_ON_EVENT) ) {
        setEvent(CLEAN_ON_EVENT, 4000);
        return ph + 1;
      } else {
        showOneColorLED(0x00FF00);
      }
      return ph;
      break;
    default:
      return -1;
      break;

  }
}

int doFunctionTwoWithPhase(int ph) {
  switch (ph) {
    case 0:
      setEvent(CLEAN_ON_EVENT, 1000);
      return ph + 1;
      break;
    case 1:
      if ( timerEvent(CLEAN_ON_EVENT) ) {
        setEvent(CLEAN_ON_EVENT, 10000);
        return ph + 1;
      } else {
        showOneColorLED(ACCEPTANCE_COLOR); // command acceptance
      }
      return ph;
      break;
    case 2: // command start
      if ( timerEvent(CLEAN_ON_EVENT) ) { // finished cleaning, this would depend on time and sensors!!!! maybe this would change
        setEvent(CLEAN_ON_EVENT, 2000);
        controlActuators( 1 , 0 ,false);
        return ph + 1;
      } else {
        controlActuators( 1 , 255 ,false);
        showBreathingLED(0x000055, 0x000000);
      }
      return ph;
      break;
    case 3:
      if ( timerEvent(CLEAN_ON_EVENT) ) { // out from mode
        setEvent(CLEAN_ON_EVENT, 10000);  // <---- set here the 4 hr for mode 2
        return ph + 1;
      } else {
        showOneColorLED(0x00FF00);
      }
      // this one should never end, shouuld go to a stay forever and loop every 4 hours. Missing state
      return ph;
      break;

    case 4:
      if ( timerEvent(CLEAN_ON_EVENT) ) { // loop here
        return 0;
      } else {
        showBreathingLED(IDLE_BREATHE_COLOR, IDLE_BREATHE_COLOR2); // breath until next loop
      }
      return ph;
      break;
      
    default:
      return -1;
      break;

  }
}


int doFunctionThreeWithPhase(int ph) {
  switch (ph) {
    case 0:
      setEvent(CLEAN_ON_EVENT, 1000);
      return ph + 1;
      break;
    case 1:
      if ( timerEvent(CLEAN_ON_EVENT) ) {
        setEvent(CLEAN_ON_EVENT, 12000);
        return ph + 1;
      } else {
        showOneColorLED(ACCEPTANCE_COLOR); // command acceptance
      }
      return ph;
      break;
    case 2: // command start
      if ( timerEvent(CLEAN_ON_EVENT) ) { // finished cleaning, this would depend on time and sensors!!!! maybe this would change
        setEvent(CLEAN_ON_EVENT, 2000);
        controlActuators( 1 , 0 ,false);
        return ph + 1;
      } else {
        controlActuators( 1 , 255 ,false);
        showQuickBreathingLED(0x0FFCC00, 0x000000);
      }
      return ph;
      break;
    case 3:
      if ( timerEvent(CLEAN_ON_EVENT) ) { // out from mode
        setEvent(CLEAN_ON_EVENT, 10000);  // <---- set here the 4 hr for mode 2
        return ph + 1;
      } else {
        showOneColorLED(0x00FF00);
      }
      // this one should never end, shouuld go to a stay forever and loop every 2 hours. Missing state
      return ph;
      break;

    case 4:
      if ( timerEvent(CLEAN_ON_EVENT) ) { // loop here
        return 0;
      } else {
        showBreathingLED(IDLE_BREATHE_COLOR, IDLE_BREATHE_COLOR2); // breath until next loop
      }
      return ph;
      break;
      
    default:
      return -1;
      break;

  }
}


int doFunctionFourWithPhase(int ph) {
  switch (ph) {
    case 0:
      setEvent(CLEAN_ON_EVENT, 1000);
      return ph + 1;
      break;
    case 1:
      if ( timerEvent(CLEAN_ON_EVENT) ) {
        setEvent(CLEAN_ON_EVENT, 12000);
        return ph + 1;
      } else {
        showOneColorLED(ACCEPTANCE_COLOR); // command acceptance
      }
      return ph;
      break;
    case 2: // command start
      if ( timerEvent(CLEAN_ON_EVENT) ) { // finished cleaning, this would depend on time and sensors!!!! maybe this would change
        setEvent(CLEAN_ON_EVENT, 2000);
        controlActuators( 1 , 0 ,false);
        return ph + 1;
      } else {
        showChaseLED(0x0FF1100, 0x000000);
        controlActuators( 1 , 255, true);
      }
      return ph;
      break;
    case 3:
      if ( timerEvent(CLEAN_ON_EVENT) ) { // out from mode
        setEvent(CLEAN_ON_EVENT, 4000);
        return ph + 1;
      } else {
        showOneColorLED(0x00FF00);
      }
      return ph;
      break;
    default:
      return -1;
      break;

  }
}



int doFunctionFiveWithPhase(int ph) {
  switch (ph) {
    case 0:
      setEvent(CLEAN_ON_EVENT, 1000);
      return ph + 1;
      break;
    case 1:
      if ( timerEvent(CLEAN_ON_EVENT) ) {
        //controlActuators(1,0,true);
        setEvent(CLEAN_ON_EVENT, 14000);
        return ph + 1;
      } else {
        showOneColorLED(ACCEPTANCE_COLOR); // command acceptance
      }
      return ph;
      break;
    case 2: // command start
      if ( timerEvent(CLEAN_ON_EVENT) ) { // finished cleaning, this would depend on time and sensors!!!! maybe this would change
        controlActuators(1,0,false);
        setEvent(CLEAN_ON_EVENT, 2000);
        return ph + 1;
      } else {
        controlActuators( 1 , 0 ,true);
        showBreathingLED(0x0FF00FF, 0x000000);
      }
      return ph;
      break;
    case 3:
      if ( timerEvent(CLEAN_ON_EVENT) ) { // out from mode
        setEvent(CLEAN_ON_EVENT, 10000); // <---- change this to loop for the amount of time needed
        return ph + 1;
      } else {
        showOneColorLED(0x00FF00);
      }
      // this one should never end, shouuld go to a stay forever and loop every 20 minutesurs. Missing state
      return ph;
      break;
     case 4:
      if ( timerEvent(CLEAN_ON_EVENT) ) { // loop here
        return 0;
      } else {
        showBreathingLED(IDLE_BREATHE_COLOR, IDLE_BREATHE_COLOR2); // breath until next loop
      }
      return ph;
      break;
      
    default:
      return -1;
      break;

  }
}

void setFunctions(int param){
  function = param;
  actual_work_function = param;  
}

void loop() {

  

  if ( timerEvent(LED_ACTUATOR_EVENT)) {

    // this will depend on function_phase
    //controlActuators(1,0,false); // dir,speed, humidifier onOff

    if ( function == F_1) {
      function_phase = doFunctionOneWithPhase(function_phase);
    }
    if ( function == F_2) {
      function_phase = doFunctionTwoWithPhase(function_phase);
    }
    if ( function == F_3) {
      function_phase = doFunctionThreeWithPhase(function_phase);
    }

    if ( function == F_4) {
      function_phase = doFunctionFourWithPhase(function_phase);
    }

    if ( function == F_5) {
      function_phase = doFunctionFiveWithPhase(function_phase);
    }
    if ( function == F_NONE ) {
      
      showBreathingLED(IDLE_BREATHE_COLOR, IDLE_BREATHE_COLOR2);
    }
    // end function!!
    if (function_phase == -1 ) {
      // function has finished
      // go to idle state

      // functions loop, so, return to function_phase 0
      //function = F_NONE;
      if(actual_work_function == F_1 || actual_work_function == F_1_BIS ){
        function = F_NONE; // dont loop
      }
      if(actual_work_function == F_4 || actual_work_function == F_4_BIS ){
        function = F_NONE; // dont loop, but actual_work_function stays there 
      }
      function_phase = 0;
    }

  }



  if ( timerEvent(SENSORS_EVENT) ) {


    readSensors();


  }

  if ( timerEvent(COMMS_EVENT) ) {

    // set this on according to rules and thats it
    
    
    sendAndReceiveComms();

    int next_function = getActualFunction();
    if (actual_work_function != next_function ) {
      Serial.print("Function just changed!!!:");
       Serial.print("from : ");
      Serial.print(function, DEC);
      Serial.print(" to : ");
      Serial.println(next_function, DEC);
      // user made a change in config.

      // any event on going should be reset!
      clearEvent(CLEAN_ON_EVENT);
      function_phase = 0;
      switch(next_function){
        case 0: setFunctions(F_NONE);
                break;
        case 1: setFunctions(F_1);
                break;
        case 2: setFunctions(F_2);
                break;
        case 3: setFunctions(F_3);
                break;
        case 4: setFunctions(F_4);
                break;
        case 5: setFunctions(F_5);
                break;
        case 6: setFunctions(F_1);
                actual_work_function = F_1_BIS;
                break;
        case 7: setFunctions(F_4);
                actual_work_function = F_4_BIS; // this is just to be able to redo the function. So it will toggle
                break;
        default:
                setFunctions(F_NONE);
                break;        
      }
    }
  }

  updateTimer(); // for events
  delay(50);
}
