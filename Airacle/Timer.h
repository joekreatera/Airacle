#define EVENT_ID 0
#define EVENT_SECS 1
#define EVENT_TIME_PASSED 2

#define MAX_EVENTS 10

int events[MAX_EVENTS][3]; // max events , no need to do more right now as it is a MVP
int max_event = 0;
unsigned long time_millis = 0;


void setupTimer(){
  for(int i = 0; i < MAX_EVENTS ; i++ ){
    events[i][EVENT_ID] = -1;   
  }
}

int setEvent(int event, int m_sec){
  if(event < MAX_EVENTS ) {
    events[event][EVENT_ID] = event;
    events[event][EVENT_SECS] = m_sec;
    events[event][EVENT_TIME_PASSED] = 0;
  }
}
void clearEvent(int event){
  events[event][EVENT_ID] = -1;
    events[event][EVENT_SECS] = 0;
    events[event][EVENT_TIME_PASSED] = 0;
}

bool timerEvent(int event){
  if( event < MAX_EVENTS ){
  
    if( events[event][EVENT_ID] > -1 ){
      if( events[event][EVENT_TIME_PASSED] >=  events[event][EVENT_SECS] ){
          events[event][EVENT_TIME_PASSED] = 0;
         
          return true;
      } 
    }
  }

 
  return false;
}

void updateTimer(){
  int delta;
  if( time_millis == 0 ){
    delta = 0;
  }else{
    delta = (long)millis() - (long)time_millis;
  }
  time_millis = millis();
    
  for(int i = 0; i < MAX_EVENTS ; i++ ){
    if(events[i][EVENT_ID] > -1 )
       events[i][EVENT_TIME_PASSED] += delta;   
  }
}
