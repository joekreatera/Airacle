#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>


#define SERVER_MODE false
SoftwareSerial s(D6, D5); // (Rx, Tx)

//Nombre de la red
const char* nombre = "NETWORK11_2.4";

//Password
const char* password = "***********";

//Puerto (dejar en 80 por defecto)
//WiFiServer server(80);

ESP8266WebServer server(80);

const char* host  = "airacle.videogamesacademy.com";
const int port = 80;

int mode_function_data  = 0;
int code = 0;

int cnt = 0;
String data = "";
int receiveMode = 0;

unsigned long report_previous_millis = 0;
unsigned long receive_previous_millis = 0;
String sensor_data = "-1:-1:-1:-1";  

void setup() {
  Serial.begin(115200);
  delay(50); //Hay que dejar un margen de tiempo

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT); 

  // Conectar a la red WiFi
  WiFi.hostname("airacle.local");
  WiFi.begin(nombre, password);

  while (WiFi.status() != WL_CONNECTED)
    delay(100); //aqui tambien hay que dejar un margen de tiempo
  Serial.println("Conectado a la red WiFi");

  // for now, this won't be used!
  if (SERVER_MODE)
    setServer();

  // Escribir la direccion IP
  Serial.print("IP: ");
  Serial.print(WiFi.localIP());
  Serial.println("");

  s.begin(19200);

}

void setServer() {
  // Iniciar el servidor

  server.on("/", handle_OnConnect);
  server.on("/red", handle_OnConnectRed);
  server.on("/green", handle_OnConnectGreen);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("Se ha iniciado el servidor");

}

String SendHTML(String msg) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #1abc9c;}\n";
  ptr += ".button-on:active {background-color: #16a085;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP8266 Web Server</h1>\n";
  ptr += "<h3>Using Access Point(AP) Mode</h3>\n";


  ptr += "<p>\n";
  ptr += msg;

  ptr += "</p>\n";

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

void handle_OnConnect() {
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", "Server ready!");
  code = 0;
  s.write(code);
  s.write(code);
}

void handle_OnConnectRed() {
  Serial.println("GPI  O7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", "received ready!");
  code = 1;
  s.write(code);
  s.write(code);
}
void handle_OnConnectGreen() {
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", "received ready!");
  code = 2;
  s.write(code);
  s.write(code);
}

void handle_NotFound() {
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", "Order not found.... yet .... sorry");
}


String receiveModeFunction() {
  
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return "";
  }

  String url = "/getMode.php?device=ABC";
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return "";
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  String line = "";
  while (client.available()) {
    line = client.readStringUntil('\r');
    //Serial.print(line);
  }

  int li = line.lastIndexOf(":");
  String datum = line.substring(li+1,line.length());
  
  //Serial.println(datum);
 
  return datum;
}

String sendDataToHost(String s_data) {

  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return "";
  }

  //Serial.println("s d : " + s_data);
  
  String url = "/server.php?device=ABC&data="+String(s_data);
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return "";
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  String line = "";
  while (client.available()) {
    line = client.readStringUntil('\r');
    //Serial.print(line);
  }

  int li = line.lastIndexOf(":");
  String datum = line.substring(li+1,line.length());
  
  //Serial.println(datum);
 
  return datum;
}

String receiveDataFromArduino() {
  // change this method to seconds based
  //Serial.println("Trying from arduino");
  
  if ( cnt++ >= 25 ) {

    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);


    s.write(mode_function_data);
    //s.write(4);
    receiveMode = 1;
    data = "";
    cnt = 0;
    
  } else {
    //Serial.println( String(receiveMode) + " " +  String(cnt) );
  } 

  if ( receiveMode) {

    while (s.available() ) {
      char c = s.read();
      data += c;

      if ( c == 'F' ) {
        receiveMode = 0;
        break;
      }
    }
  } else {

    if ( !data.equals("") ) {
      String toret= "" + data.substring(1,data.length()-1); 
      //Serial.println(data);
      data = "";
      
      return toret;
    }
    data = "";
    
    
  }

 return "";
}

void loop() {
  // Comprobar si se ha conectado algun cliente
  //WiFiClient client = server.available();
  if (SERVER_MODE)
    server.handleClient();

  String d = receiveDataFromArduino();
  
  if(!d.equals("") ){
    
    sensor_data = "" + d.substring(1,d.length());
  }


  if ( millis() - receive_previous_millis > 2000 ) {
    digitalWrite(LED_BUILTIN, HIGH);
    
    String mode_f = receiveModeFunction();
    mode_function_data = mode_f.toInt();
    receive_previous_millis = millis();

    if ( millis() - report_previous_millis > 30000 ) {
    //Serial.println("sensor data " + sensor_data + "--------");
      sendDataToHost(sensor_data); //strings have a method to transform it to ints :D
      report_previous_millis = millis();
    }

    digitalWrite(LED_BUILTIN, LOW);
  }

  if (!SERVER_MODE)
    delay(50);

}
