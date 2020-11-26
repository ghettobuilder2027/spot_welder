// Spot welder by Ghetto Builder


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>


// Wifi credentials
const char* ssid     = "your router";
const char* password = "your password";

// time for the 3 phases of the spot welding ( in millisecondes)
int chauffage = 11;
int pause = 11 ;
int impulsion = 11 ;

// Address of the timings saved in eeprom
int addr_c = 0;
int addr_p = 10;
int addr_i = 20;

// Create an instance of the server
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // where the spot welds
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  chauffage = read_eeprom(0);
  pause = read_eeprom(10);
  impulsion = read_eeprom(20);

  Serial.println(chauffage);
  Serial.println(pause);
  Serial.println(impulsion);
 

  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  server.on("/args", handleGenericArgs); //Associate the handler function to the path
  server.on("/spargs", handleSpecificArg);   //Associate the handler function to the path
  server.on("/chauffage", a1);
  server.on("/pause", a2);
  server.on("/impulsion", a3);
  server.on("/spot",spot);
  
  
  // Start the server
  server.begin();
  Serial.println("Server started");
  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();    //Handling of incoming requests
  Serial.println(chauffage);
  Serial.println(pause);
  Serial.println(impulsion);
  delay (30);
  
 }

// unused
void handleGenericArgs() { //Handler

  String message = "Number of args received:";
  message += server.args();            //Get number of parameters
  message += "\n";                            //Add a new line
  for (int i = 0; i < server.args(); i++) {
    message += "Arg nº" + (String)i + " –> ";   //Include the current iteration value
    message += server.argName(i) + ": ";     //Get the name of the parameter
    message += server.arg(i) + "\n";              //Get the value of the parameter
  } 
  server.send(200, "text/plain", message);       //Response to the HTTP request
}

void handleSpecificArg() {
   if ( server.arg("b1") != "") {
    chauffage =server.arg("b1").toInt(); 
    write_eeprom(chauffage,0);
    server.send(200, "text/plain",server.arg("b1") );
  }
  else if ( server.arg("b2") != "") {
    pause =server.arg("b2").toInt();
    write_eeprom(pause,10);
    server.send(200, "text/plain",server.arg("b2") );
  }
  else if ( server.arg("b3") != "") {
    impulsion =server.arg("b3").toInt(); 
    write_eeprom(impulsion,20);
    server.send(200, "text/plain",server.arg("b3") );
  }
  
  
}
void a1(){
  server.send(200, "text/plain", String(chauffage));
}
void a2(){
  server.send(200, "text/plain", String(pause));
}
void a3(){
  server.send(200, "text/plain", String(impulsion));
}

void spot () {
  digitalWrite(2, HIGH);
  delay(chauffage);
  digitalWrite(2, LOW);
  delay(pause);
  digitalWrite(2, HIGH);
  delay(impulsion);
  digitalWrite(2, LOW);
  server.send(200, "text/plain", "Ca soude");
}

void write_eeprom(int a, int addr) {
  EEPROM.begin(512);
  EEPROM.put(addr, a);
  addr += sizeof(int);
  EEPROM.put(addr, a);
  EEPROM.commit();
}

int read_eeprom(int addr) {
  int a;
  EEPROM.begin(512);
  EEPROM.get(addr, a);
  return a;
}
