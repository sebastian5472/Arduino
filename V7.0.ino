// Version 1 Ethernet y cONtrol basico de relays
// Version 2 Se modifican los botONes
// Version 3 Se incorpora el sensor de humedad
// Version 4 Se corrige la web para usar tablas html
// Version 5 Incorporo el reloj, se carga subrutina luces exteriores, se ajusta distribuciON de botONes
// Version 6 Se incorpora el sensor de movimiento y se enlaza al pasillo
// Version 7 Se agrega el sensor de monoxido

/* CONsideraciONes:
 *  Reloj SDA PIN: 20
 *        SCL PIN: 21
 *  Relay R01-R16: PIN 30 - PIN 45
 *  Humedad PIN: 24
 *  Movimiento PIN: 26
 *  MONoxido PIN: 28
 */

#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>
#include <Wire.h>
#include <RTClib.h>


#define DHTTYPE DHT22

// Enter a MAC address and IP address for your cONtroller below.
// The IP address will be dependent ON your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC };
IPAddress ip(192,168,1,33);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);


// Sensor de humedad
int DHTPin = 24; 
DHT dht(DHTPin, DHTTYPE);

// Clock
RTC_DS3231 rtc;
int horaVal = 0;

// constantes umbral horario Exterior
int horaMaxExt = 7;
int horaMinExt = 19;
int horaMaxPas = 6;
int horaMinPas = 0;
int t_apagado = 1;

//auxiliares horario web
int t_hora = 0;
int t_min = 0;

// Umbral de humedad
int humedad = 50;
int flag2 = 0;

//Sensor de movimiento
const int buttonPin = 26;
int buttonState = 0;
int flag = 0 ;
int flag3 = 0;

//Sensor de monoxido
int MQ_PIN = 28; 

// Relay state and pin
String R1State = "OFF";
String R2State = "OFF";
String R3State = "OFF";
String R4State = "OFF";
String R5State = "OFF";
String R6State = "OFF";
String R7State = "OFF";
String R8State = "OFF";
String R9State = "OFF";
String R10State = "OFF";
String R11State = "OFF";
String R12State = "OFF";
String R13State = "OFF";
String R14State = "OFF";
String R15State = "OFF";
String R16State = "OFF";

const int R1 = 30;
const int R2 = 31;
const int R3 = 32;
const int R4 = 33;
const int R5 = 34;
const int R6 = 35;
const int R7 = 36;
const int R8 = 37;
const int R9 = 38;
const int R10 = 39;
const int R11 = 40;
const int R12 = 41;
const int R13 = 42;
const int R14 = 43;
const int R15 = 44;
const int R16 = 45;

// Client variables 
char linebuf[80];
int charcount=0;

String Cadenas = "";


void setup() { 
  // Sensor de movimiento
  pinMode(buttonPin, INPUT);

  //Sensorde monoxido
   pinMode(MQ_PIN, INPUT);
   
  // Relay module prepared 
  pinMode(R1, OUTPUT);
  digitalWrite(R1, HIGH);

  pinMode(R2, OUTPUT);
  digitalWrite(R2, HIGH);

  pinMode(R3, OUTPUT);
  digitalWrite(R3, HIGH);

  pinMode(R4, OUTPUT);
  digitalWrite(R4, HIGH);

  pinMode(R5, OUTPUT);
  digitalWrite(R5, HIGH);
  
  pinMode(R6, OUTPUT);
  digitalWrite(R6, HIGH);

  pinMode(R7, OUTPUT);
  digitalWrite(R7, HIGH);

  pinMode(R8, OUTPUT);
  digitalWrite(R8, HIGH);

  pinMode(R9, OUTPUT);
  digitalWrite(R9, HIGH);

  pinMode(R10, OUTPUT);
  digitalWrite(R10, HIGH);
  
  pinMode(R11, OUTPUT);
  digitalWrite(R11, HIGH);

  pinMode(R12, OUTPUT);
  digitalWrite(R12, HIGH);

  pinMode(R13, OUTPUT);
  digitalWrite(R13, HIGH);

  pinMode(R14, OUTPUT);
  digitalWrite(R14, HIGH);

  pinMode(R15, OUTPUT);
  digitalWrite(R15, HIGH);

  pinMode(R16, OUTPUT);
  digitalWrite(R16, HIGH);
  
  // Open serial communicatiON at a baud rate of 9600
  Serial.begin(9600);
  
  // start the Ethernet cONnectiON and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

 // inicio sensor de humedad
 dht.begin();
 
 // start clock
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } 
}

// Display dashboard page with ON/OFF button for relay
// It also print Temperature in C and F
void dashboardPage(EthernetClient &client) {
  client.println("<!DOCTYPE HTML><html><head>");
  client.println("<meta name=\"viewport\" cONtent=\"width=device-width, initial-scale=1\"></head><body>");                                                             
  client.println("<h3>Domotica Hogar - Hora: "); 
  client.print(t_hora);
  client.print(':');
  client.print(t_min);
  client.print("  -   ");
  client.println("<a href=\"/\">Refresh</a></h3>");
  client.println("<table border=0>");
  client.println("<tr><td><b>ELEMENTO</b></td><td><b>STATUS</b></td><td><b>CONTROL</b></td></tr>");
  
  // Relay 14
      client.println("<tr><td>Luz Exterior Frente</td>"); if (R14State=="OFF"){ client.println("<td align=center bgcolor=gray>"); } else { client.println("<td align=center bgcolor=#1aff1a>");} client.println("<fONt color=#ffffff> "+ R14State + "</td><td align=center>");
      // If relay is OFF, it shows the button to turn the output ON          
      if(R14State == "OFF"){
        client.println("<a href=\"/R14_ON\"><button>ON</button></a></td></tr>");
      }
      // If relay is ON, it shows the button to turn the output OFF         
      else if(R14State == "ON"){
        client.println("<a href=\"/R14_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }
      
  // Relay 16
      client.println("<tr><td>Luz Exterior Lateral</td>"); if (R16State=="OFF"){ client.println("<td align=center bgcolor=gray>"); } else { client.println("<td align=center bgcolor=#1aff1a>");} client.println("<fONt color=#ffffff> "+ R16State + "</td><td align=center>");
      // If relay is OFF, it shows the button to turn the output ON          
      if(R16State == "OFF"){
        client.println("<a href=\"/R16_ON\"><button>ON</button></a></td></tr>");
      }
      // If relay is ON, it shows the button to turn the output OFF         
      else if(R16State == "ON"){
        client.println("<a href=\"/R16_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }
      
  // Relay 5
      client.println("<tr><td>Luz Living</td>"); if (R5State=="OFF"){ client.println("<td align=center bgcolor=gray>"); } else { client.println("<td align=center bgcolor=#1aff1a>");} client.println("<fONt color=#ffffff> "+ R5State + "</td><td align=center>");
      // If relay is OFF, it shows the button to turn the output ON          
      if(R5State == "OFF"){
        client.println("<a href=\"/R5_ON\"><button>ON</button></a></td></tr>");
      }
      // If relay is ON, it shows the button to turn the output OFF         
      else if(R5State == "ON"){
        client.println("<a href=\"/R5_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }
      
  // Relay 15
      client.println("<tr><td>Spot Living</td>"); if (R15State=="OFF"){ client.println("<td align=center bgcolor=gray>"); } else { client.println("<td align=center bgcolor=#1aff1a>");} client.println("<fONt color=#ffffff> "+ R15State + "</td><td align=center>");
      // If relay is OFF, it shows the button to turn the output ON          
      if(R15State == "OFF"){
        client.println("<a href=\"/R15_ON\"><button>ON</button></a></td></tr>");
      }
      // If relay is ON, it shows the button to turn the output OFF         
      else if(R15State == "ON"){
        client.println("<a href=\"/R15_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }
  
  // Relay 6
      client.println("<tr><td>Luz Comedor</td>"); if (R6State=="OFF"){ client.println("<td align=center bgcolor=gray>"); } else { client.println("<td align=center bgcolor=#1aff1a>");} client.println("<fONt color=#ffffff> "+ R6State + "</td><td align=center>");
      // If relay is OFF, it shows the button to turn the output ON          
      if(R6State == "OFF"){
        client.println("<a href=\"/R6_ON\"><button>ON</button></a></td></tr>");
      }
      // If relay is ON, it shows the button to turn the output OFF         
      else if(R6State == "ON"){
        client.println("<a href=\"/R6_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }

  // Relay 8
      client.println("<tr><td>Luz Cocina</td>"); if (R8State=="OFF"){ client.println("<td align=center bgcolor=gray>"); } else { client.println("<td align=center bgcolor=#1aff1a>");} client.println("<fONt color=#ffffff> "+ R8State + "</td><td align=center>");
      // If relay is OFF, it shows the button to turn the output ON          
      if(R8State == "OFF"){
        client.println("<a href=\"/R8_ON\"><button>ON</button></a></td></tr>");
      }
      // If relay is ON, it shows the button to turn the output OFF         
      else if(R8State == "ON"){
        client.println("<a href=\"/R8_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }
  
  // Relay 7
      client.println("<tr><td>Spot Cocina</td>"); if (R7State=="OFF"){ client.println("<td align=center bgcolor=gray>"); } else { client.println("<td align=center bgcolor=#1aff1a>");} client.println("<fONt color=#ffffff> "+ R7State + "</td><td align=center>");
      // If relay is OFF, it shows the button to turn the output ON          
      if(R7State == "OFF"){
        client.println("<a href=\"/R7_ON\"><button>ON</button></a></td></tr>");
      }
      // If relay is ON, it shows the button to turn the output OFF         
      else if(R7State == "ON"){
        client.println("<a href=\"/R7_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }
      
  // Relay 13
      client.println("<tr><td>Luz Ba&ntilde;o</td>"); if (R13State=="OFF"){ client.println("<td align=center bgcolor=gray>"); } else { client.println("<td align=center bgcolor=#1aff1a>");} client.println("<fONt color=#ffffff> "+ R13State + "</td><td align=center>");
      // If relay is OFF, it shows the button to turn the output ON          
      if(R13State == "OFF"){
        client.println("<a href=\"/R13_ON\"><button>ON</button></a></td></tr>");
      }
      // If relay is ON, it shows the button to turn the output OFF         
      else if(R13State == "ON"){
        client.println("<a href=\"/R13_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }
         
  // Relay 12
      client.println("<tr><td>Luz Pasillo</td>"); if (R12State=="OFF"){ client.println("<td align=center bgcolor=gray>"); } else { client.println("<td align=center bgcolor=#1aff1a>");} client.println("<fONt color=#ffffff> "+ R12State + "</td><td align=center>");
      // If relay is OFF, it shows the button to turn the output ON          
      if(R12State == "OFF"){
        client.println("<a href=\"/R12_ON\"><button>ON</button></a></td></tr>");
      }
      // If relay is ON, it shows the button to turn the output OFF         
      else if(R12State == "ON"){
        client.println("<a href=\"/R12_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }
      
  // Relay 11
      client.println("<tr><td>Luz Lavadero</td>"); if (R11State=="OFF"){ client.println("<td align=center bgcolor=gray>"); } else { client.println("<td align=center bgcolor=#1aff1a>");} client.println("<fONt color=#ffffff> "+ R11State + "</td><td align=center>");
      // If relay is OFF, it shows the button to turn the output ON          
      if(R11State == "OFF"){
        client.println("<a href=\"/R11_ON\"><button>ON</button></a></td></tr>");
      }
      // If relay is ON, it shows the button to turn the output OFF         
      else if(R11State == "ON"){
        client.println("<a href=\"/R11_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }
      
  // Relay 2
      client.println("<tr><td>Luz Habitacion Frente</td>"); if (R2State=="OFF"){ client.println("<td align=center bgcolor=gray>"); } else { client.println("<td align=center bgcolor=#1aff1a>");} client.println("<fONt color=#ffffff> "+ R2State + "</td><td align=center>");
      // If relay is OFF, it shows the button to turn the output ON          
      if(R2State == "OFF"){
        client.println("<a href=\"/R2_ON\"><button>ON</button></a></td></tr>");
      }
      // If relay is ON, it shows the button to turn the output OFF         
      else if(R2State == "ON"){
        client.println("<a href=\"/R2_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }
      
  // Relay 1
      client.println("<tr><td>Spot Habitacion Frente</td>"); if (R1State=="OFF"){ client.println("<td align=center bgcolor=gray>"); } else { client.println("<td align=center bgcolor=#1aff1a>");} client.println("<fONt color=#ffffff> "+ R1State + "</td><td align=center>");
      // If relay is OFF, it shows the button to turn the output ON          
      if(R1State == "OFF"){
        client.println("<a href=\"/R1_ON\"><button>ON</button></a></td></tr>");
      }
      // If relay is ON, it shows the button to turn the output OFF         
      else if(R1State == "ON"){
        client.println("<a href=\"/R1_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }

  // Relay 4
      client.println("<tr><td>Luz Habitacion Fondo</td>"); if (R4State=="OFF"){ client.println("<td align=center bgcolor=gray>"); } else { client.println("<td align=center bgcolor=#1aff1a>");} client.println("<fONt color=#ffffff> "+ R4State + "</td><td align=center>");
      // If relay is OFF, it shows the button to turn the output ON          
      if(R4State == "OFF"){
        client.println("<a href=\"/R4_ON\"><button>ON</button></a></td></tr>");
      }
      // If relay is ON, it shows the button to turn the output OFF         
      else if(R4State == "ON"){
        client.println("<a href=\"/R4_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }
 
  // Relay 3
      client.println("<tr><td>Spot Habitacion Fondo</td>"); if (R3State=="OFF"){ client.println("<td align=center bgcolor=gray>"); } else { client.println("<td align=center bgcolor=#1aff1a>");} client.println("<fONt color=#ffffff> "+ R3State + "</td><td align=center>");
      // If relay is OFF, it shows the button to turn the output ON          
      if(R3State == "OFF"){
        client.println("<a href=\"/R3_ON\"><button>ON</button></a></td></tr>");
      }
      // If relay is ON, it shows the button to turn the output OFF         
      else if(R3State == "ON"){
        client.println("<a href=\"/R3_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }
      
  // Relay 10 - Extractor
      client.println("<tr><td>Extractor</td>"); if (R10State=="OFF"){ client.println("<td align=center bgcolor=gray>"); } else { client.println("<td align=center bgcolor=#1aff1a>");} client.println("<fONt color=#ffffff> "+ R10State + "</td><td align=center>");
      // If relay is OFF, it shows the button to turn the output ON          
      if(R10State == "OFF"){
        client.println("<a href=\"/R10_ON\"><button>ON</button></a></td></tr>");
      }
      // If relay is ON, it shows the button to turn the output OFF         
      else if(R10State == "ON"){
        client.println("<a href=\"/R10_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }
      
/*  // Relay 9 - Vacio
      client.println("<tr><td>Relay 9</td><td>" + R9State + "</td><td>");
      // If relay is OFF, it shows the button to turn the output ON          
      if(R9State == "OFF"){
        client.println("<a href=\"/R9_ON\"><button>ON</button></a></td></tr>");
      }
      // If relay is ON, it shows the button to turn the output OFF         
      else if(R9State == "ON"){
        client.println("<a href=\"/R9_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }
*/
  
 // Ajuste variables
 client.println("</table><br><br><b>Umbrales</b><br><table>");   
 client.println("<tr><td>Encender Luces Exteriores</td><td><form><input type=number length=2  name=m_1 value="+ String(horaMinExt) +"><td><input type=submit value=Submit></form></td></tr>");
 client.println("<tr><td>Apagar Luces Exteriores</td><td><form><input type=number length=2  name=m_2 value="+ String(horaMaxExt) +"><td><input type=submit value=Submit></form></td></tr>");
 client.println("<tr><td>Luz de Pasillo Automatica desde:</td><td><form><input type=number length=2  name=m_3 value="+ String(horaMinPas) +"><td><input type=submit value=Submit></form></td></tr>");
 client.println("<tr><td>Luz de Pasillo Automatica hasta:</td><td><form><input type=number length=2  name=m_4 value="+ String(horaMaxPas) +"><td><input type=submit value=Submit></form></td></tr>");
 client.println("<tr><td>Delay Luz Pasillo (min):</td><td><form><input type=number length=2  name=m_5 value="+ String(t_apagado) +"><td><input type=submit value=Submit></form></td></tr>");
 client.println("<tr><td>Porcentaje de humedad</td><td><form><input type=number length=2  name=m_6 value="+ String(humedad) +"><td><input type=submit value=Submit></form></td></tr>");
 client.println("</table></body></html>"); 
}


void loop() 
{
  //Verifico hora
  DateTime now = rtc.now();   
  int horaVal = now.hour();
  t_hora = horaVal;
  t_min = now.minute();
  
  // Control de luz externa por horario
  if ((horaVal >= horaMaxExt ) and (horaVal < horaMinExt))
  {
    if (R16State == "ON")
      {
       digitalWrite(R16, HIGH);
       R16State = "OFF";
      }
    if (R14State == "ON")
      {
       digitalWrite(R14, HIGH);
       R14State = "OFF";
      }
  }
  else
  {
    if (R14State == "OFF")
      {  
       digitalWrite(R14, LOW);
       R14State = "ON";
      }
    if (R16State == "OFF")
      {
       digitalWrite(R16, LOW);
       R16State = "ON";
      }
  }
  
  // Control de luz de Pasillo por horario
  if (( horaVal >= horaMinPas) and ( horaVal <= horaMaxPas))
    {
      buttonState = digitalRead(buttonPin);  
      if (buttonState == HIGH)
      {
         if ( R12State == "OFF") 
         {
           digitalWrite(R12, LOW);
           R12State = "ON";
           int minutos = now.minute();
           flag3 = 1;
           flag = minutos + 5;
           if ( flag > 59)
            { 
              flag = 60 - flag;
            }
         } 
         else
         {       
          int minutos = now.minute();
          flag = minutos + t_apagado; 
          if ( flag > 59)
          { 
           flag = 60 - flag;
          }
         }                 
      }
      else
      {
        if ( R12State == "ON")
         {
          int minutos = now.minute();
          if (flag <= minutos)
          {
              digitalWrite(R12, HIGH);
              R12State = "OFF";
              flag3 = 0;
          }
         }
      }       
    }
    else
    {
      if ( flag3==1 )
      {
        digitalWrite(R12, HIGH);
        R12State = "OFF";
        flag3 = 0;
      }
    }

  //Valor de humedad
  float humVal = dht.readHumidity();
  //control extractor
  Serial.print (humVal);
   if ( humVal > humedad )
   {
      if (R10State == "OFF")
      {
       int tmpExt = now.minute();
       flag2 = tmpExt +5;
       Serial.print ("prender extractor"); //Relay 10
       digitalWrite(R10, LOW);
       R10State = "ON";
      }
   }
   else
   {
      int tmpExt = now.minute();
      if (R10State == "ON")     
      {
       if (tmpExt > flag2)
        {
          Serial.print ("apagar extractor");
          digitalWrite(R10, HIGH);
          R10State = "OFF";
        }
      }
   }
  // Lectura de monoxido
/*  bool state= digitalRead(MQ_PIN);
  if (!state)
  {
    int sensorValue = analogRead(A0);
    // print out the value you read:
    Serial.println(sensorValue);
    Serial.println("Detectado");
  }
  else
  {
    //Serial.println("No detectado");
  } 
  */
  // listen for incoming clients
  EthernetClient client = server.available();

  if (client) {
    Serial.println("new client");
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
       char c = client.read();
       //read char by char HTTP request
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          dashboardPage(client);
          break;
        }
        if (c == '\n') {

          //Relay 1
          if (strstr(linebuf,"GET /R1_OFF") > 0){
            digitalWrite(R1, HIGH);
            R1State = "OFF";
          }
          else if (strstr(linebuf,"GET /R1_ON") > 0){
            digitalWrite(R1, LOW);
            R1State = "ON";
          }

          //Relay 2
          if (strstr(linebuf,"GET /R2_OFF") > 0){
            digitalWrite(R2, HIGH);
            R2State = "OFF";
          }
          else if (strstr(linebuf,"GET /R2_ON") > 0){
            digitalWrite(R2, LOW);
            R2State = "ON";
          }
          
          //Relay 3
          if (strstr(linebuf,"GET /R3_OFF") > 0){
            digitalWrite(R3, HIGH);
            R3State = "OFF";
          }
          else if (strstr(linebuf,"GET /R3_ON") > 0){
            digitalWrite(R3, LOW);
            R3State = "ON";
          }
          
          //Relay 4
          if (strstr(linebuf,"GET /R4_OFF") > 0){
            digitalWrite(R4, HIGH);
            R4State = "OFF";
          }
          else if (strstr(linebuf,"GET /R4_ON") > 0){
            digitalWrite(R4, LOW);
            R4State = "ON";
          }
          
          //Relay 5
          if (strstr(linebuf,"GET /R5_OFF") > 0){
            digitalWrite(R5, HIGH);
            R5State = "OFF";
          }
          else if (strstr(linebuf,"GET /R5_ON") > 0){
            digitalWrite(R5, LOW);
            R5State = "ON";
          }
          
          //Relay 6
          if (strstr(linebuf,"GET /R6_OFF") > 0){
            digitalWrite(R6, HIGH);
            R6State = "OFF";
          }
          else if (strstr(linebuf,"GET /R6_ON") > 0){
            digitalWrite(R6, LOW);
            R6State = "ON";
          }

          //Relay 7
          if (strstr(linebuf,"GET /R7_OFF") > 0){
            digitalWrite(R7, HIGH);
            R7State = "OFF";
          }
          else if (strstr(linebuf,"GET /R7_ON") > 0){
            digitalWrite(R7, LOW);
            R7State = "ON";
          }

          //Relay 8
          if (strstr(linebuf,"GET /R8_OFF") > 0){
            digitalWrite(R8, HIGH);
            R8State = "OFF";
          }
          else if (strstr(linebuf,"GET /R8_ON") > 0){
            digitalWrite(R8, LOW);
            R8State = "ON";
          }
          
          //Relay 9
          if (strstr(linebuf,"GET /R9_OFF") > 0){
            digitalWrite(R9, HIGH);
            R9State = "OFF";
          }
          else if (strstr(linebuf,"GET /R9_ON") > 0){
            digitalWrite(R9, LOW);
            R9State = "ON";
          }
          
          //Relay 10
          if (strstr(linebuf,"GET /R10_OFF") > 0){
            digitalWrite(R10, HIGH);
            R10State = "OFF";
          }
          else if (strstr(linebuf,"GET /R10_ON") > 0){
            digitalWrite(R10, LOW);
            R10State = "ON";
          }
          
          //Relay 11
          if (strstr(linebuf,"GET /R11_OFF") > 0){
            digitalWrite(R11, HIGH);
            R11State = "OFF";
          }
          else if (strstr(linebuf,"GET /R11_ON") > 0){
            digitalWrite(R11, LOW);
            R11State = "ON";
          }
          
          //Relay 12
          if (strstr(linebuf,"GET /R12_OFF") > 0){
            digitalWrite(R12, HIGH);
            R12State = "OFF";
          }
          else if (strstr(linebuf,"GET /R12_ON") > 0){
            digitalWrite(R12, LOW);
            R12State = "ON";
          }
          
          //Relay 13
          if (strstr(linebuf,"GET /R13_OFF") > 0){
            digitalWrite(R13, HIGH);
            R13State = "OFF";
          }
          else if (strstr(linebuf,"GET /R13_ON") > 0){
            digitalWrite(R13, LOW);
            R13State = "ON";
          }
          
          //Relay 14
          if (strstr(linebuf,"GET /R14_OFF") > 0){
            digitalWrite(R14, HIGH);
            R14State = "OFF";
          }
          else if (strstr(linebuf,"GET /R14_ON") > 0){
            digitalWrite(R14, LOW);
            R14State = "ON";
          }
          
          //Relay 15
          if (strstr(linebuf,"GET /R15_OFF") > 0){
            digitalWrite(R15, HIGH);
            R15State = "OFF";
          }
          else if (strstr(linebuf,"GET /R15_ON") > 0){
            digitalWrite(R15, LOW);
            R15State = "ON";
          }
          
          //Relay 16
          if (strstr(linebuf,"GET /R16_OFF") > 0){
            digitalWrite(R16, HIGH);
            R16State = "OFF";
          }
          else if (strstr(linebuf,"GET /R16_ON") > 0){
            digitalWrite(R16, LOW);
            R16State = "ON";
          }

       //Ajustes umbrales

       //horario prender luz externa
      if (strstr(linebuf,"m_1") > 0)
        {
          Cadenas = linebuf;
          int j = Cadenas.indexOf("=");
          if (j>30)
          {
            Cadenas = Cadenas.substring(j+1,j+3);
            int num = Cadenas.toInt();
            Serial.println(num);
            if ((num >=0)and (num <25))
            {
              horaMinExt=num;
            }
          }
        }
        // Apagado luz externa
        if (strstr(linebuf,"m_2") > 0)
        {
          Cadenas = linebuf;
          int j = Cadenas.indexOf("=");
          if (j>30)
          {
            Cadenas = Cadenas.substring(j+1,j+3);
            int num = Cadenas.toInt();
            Serial.println(num);
            if ((num >=0)and (num <25))
            {
              horaMaxExt=num;
            }
          }
        }
         // Encender Pasillo
        if (strstr(linebuf,"m_3") > 0)
        {
          Cadenas = linebuf;
          int j = Cadenas.indexOf("=");
          if (j>30)
          {
            Cadenas = Cadenas.substring(j+1,j+3);
            int num = Cadenas.toInt();
            Serial.println(num);
            if ((num >=0)and (num <25))
            {
              horaMinPas=num;
            }
          }
        }
         // Apagar Pasillo
        if (strstr(linebuf,"m_4") > 0)
        {
          Cadenas = linebuf;
          int j = Cadenas.indexOf("=");
          if (j>30)
          {
            Cadenas = Cadenas.substring(j+1,j+3);
            int num = Cadenas.toInt();
            Serial.println(num);
            if ((num >=0)and (num <25))
            {
              horaMaxPas=num;
            }
          }
        }
         // Delay Pasillo
        if (strstr(linebuf,"m_5") > 0)
        {
          Cadenas = linebuf;
          int j = Cadenas.indexOf("=");
          if (j>30)
          {
            Cadenas = Cadenas.substring(j+1,j+3);
            int num = Cadenas.toInt();
            Serial.println(num);
            if ((num >=0)and (num <25))
            {
              t_apagado=num;
            }
          }
        }
        // % Humedad
        if (strstr(linebuf,"m_6") > 0)
        {
          Cadenas = linebuf;
          int j = Cadenas.indexOf("=");
          if (j>30)
          {
            Cadenas = Cadenas.substring(j+1,j+3);
            int num = Cadenas.toInt();
            Serial.println(num);
            if ((num >=0)and (num <100))
            {
              humedad=num;
            }
          }
        }
          // you're starting a new line
          currentLineIsBlank = true;
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;          
        } 
        else if (c != '\r') {
          // you've gotten a character ON the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(300);
    // close the cONnectiON:
    client.stop();
    Serial.println("client disONnected");
  }
}
