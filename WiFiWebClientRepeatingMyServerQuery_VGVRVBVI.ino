/*
  Repeating WiFi Web Client

 This sketch connects to a a web server and makes a request
 using a WiFi equipped Arduino board.

 created 23 April 2012
 modified 31 May 2012
 by Tom Igoe
 modified 13 Jan 2014
 by Federico Vanzati

 This code is in the public domain.

  Find the full UNO R4 WiFi Network documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples#wi-fi-web-client-repeating
 */

#include "WiFiS3.h"


#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;

// Initialize the WiFi client library
WiFiClient client;

// server address:
//char server[] = "www.google.com";
IPAddress server(108,62,123,70);

//Variables to send to the server
//Voltaje Red Eléctrica (d5)
//Electric Grid Voltage (d5)
float vg = 0;
//Voltaje Relay Inversor (d6)
//Inverter Relay Voltage (d6)
float vr = 0;
//Voltaje Batería (a1)
//Battery Voltage (a1)
float vb = 0;
//Voltaje Encender Inversor (d7)
//Power On Inverter Voltage (d7)
float vi = 0;
//Entradas analógicas para monitorear voltajes 
//Analog inputs to monitor voltages
float a0 = 0;
float a1 = 0;
float a2 = 0;
float a3 = 0;
//Identificador del dispositivo
//Device identifier
char sensor_id[] = "WIL-01";

//1L * 1000L para un tiempo de espera de 1 segundo para volver a enviar datos
//60L * 1000L para un tiempo de espera de 60 segundos para volver a enviar datos
//1L * 1000L for a 1-second wait time before resending data
//60L * 1000L for a 60-second wait time before resending data
unsigned long lastConnectionTime = 0;            // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 1L * 1000L; // delay between updates, in milliseconds

/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    //delay(10000);
  }
  // you're connected now, so print out the status:
  printWifiStatus();

}

/* just wrap the received data up to 80 columns in the serial print*/
/* -------------------------------------------------------------------------- */
void read_request() {
/* -------------------------------------------------------------------------- */  
  uint32_t received_data_num = 0;

  while (client.available()) {
    /* actual data reception */
    char c = client.read();
    /* print data to serial port */
    Serial.print(c);
    /* wrap data to 80 columns*/
    received_data_num++;
    if(received_data_num % 80 == 0) { 
      
    }
    
  }  
}

/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */  
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  read_request();
  
  // if sixty seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }

}

// this method makes a HTTP connection to the server:
/* -------------------------------------------------------------------------- */
void httpRequest() {
/* -------------------------------------------------------------------------- */  
  // close any connection before send a new request.
  // This will free the socket on the NINA module
  client.stop();

  //Lectura de Voltajes por las entradas analógicas
  //Reading Voltages of Analog inputs
  a0 = analogRead(0);
  vg = (a0*20.0)/1024.0;
  a1 = analogRead(1);
  vr = (a1*20.0)/1024.0;
  a2 = analogRead(2);
  vb = (a2*20.0)/1024.0;
  a3 = analogRead(3);
  vi = (a3*20.0)/1024.0;

  // if there's a successful connection:
  //Conectar servidor por puerto 80 HTTP
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP GET request:
    //Método GET, /VGVRVBVI/ es el directorio,
    //formaction_vgvrvbvi.php es el formulario para ingresar variables
    //vg, vr, vb, vi son las variables enviadas al servidor 
    //GET method, /VGVRVBVI/ is the directory,
    //formation_vgvrvbvi.php is the form for entering variables
    //vg, vr, vb, vi are the variables sent to the server
    client.print("GET /VGVRVBVI/formaction_vgvrvbvi.php?vg=");
    client.print(vg);
    client.print("&vr=");
    client.print(vr);
    client.print("&vb=");
    client.print(vb);
    client.print("&vi=");
    client.print(vi);
    client.print("&sensor_id=");
    client.print(sensor_id);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}

/* -------------------------------------------------------------------------- */
void printWifiStatus() {
/* -------------------------------------------------------------------------- */  
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
