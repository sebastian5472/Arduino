
/* Consideraciones:
 *  Reloj SDA PIN: 20
 *        SCL PIN: 21
 *  Relay R01-R16: PIN 30 - PIN 45
 *  Humedad PIN: 24
 *  Movimiento PIN: 26
 *  Monoxido PIN: 28
 */

// Includes
	#include <SPI.h>
	#include <Ethernet.h>
	#include <DHT.h>
	#include <Wire.h>
	#include <RTClib.h>

// Variables Globales

	// Definiciones Sensor temperatura
	#define DHTTYPE DHT22
	int DHTPin = 24; 
	DHT dht(DHTPin, DHTTYPE);
	
	// Definiciones LAN
	byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC };
	IPAddress ip(192,168,1,33);
	EthernetServer server(80);

	// Definiciones Reloj
	RTC_DS3231 rtc;
	int horaVal = 0;

	// Definiciones Sensor Movimiento
	const int buttonPin = 26;
	int buttonState = 0;
	
	// Sensor de monoxido
	int MQ_PIN = 28; 
	
	// Relay
	int maxrelay = 16;
	int RelayPin[]={ 34, 44, 35, 37, 36, 31, 30, 33, 32, 42, 39, 40, 41, 43, 45 };
	string RelayName[]={ "Living", "Spot Living", "Comedor", "Cocina", "Spot Cocina", "Habitacion Frente", "Spot Habitacion Frente", "Luz Habitacion Fondo", "Spot Habitacion Fondo", "Baño", "Extractor", "Lavadero", "Pasillo", "Exterior Frente", "Exterior Lateral" }; 
	string RelayState[]={ "OFF", "OFF", "OFF", "OFF", "OFF", "OFF", "OFF", "OFF", "OFF", "OFF", "OFF", "OFF", "OFF", "OFF", "OFF", "OFF" };
	int RelayTimeOn[]={ 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 };
	int RelayTimeOff[]={ 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 };
	
	//Variables extras
	int horaVal = 0;
	int t_hora  = 0;
	int t_min = 0;
	int humedad = 50;
	float humVal = 0.0;
	
	//Flags
	int flag1 = 0;
	int flag2 = 0;
	int flag3 = 0;
	int flag4 = 0;

	// Client variables 
	char linebuf[80];
	int charcount=0;
	String Cadenas = "";

// Seccion Setup
void setup() 
{ 
	// Asignacion de Relay
	for( i = 0; i < maxrelay ; i++ )
	{
		pinMode(RelayPin[i], OUTPUT);
		digitalWrite(RelayPin[i], HIGH);
	}
   
	// Apertura de puerto serial
	Serial.begin(9600);
  
	// Conexion LAN:
	Ethernet.begin(mac, ip);
	server.begin();

	// Inicio sensor de humedad
	dht.begin();
	
 	// Sensor de movimiento
	pinMode(buttonPin, INPUT);

	// Sensorde monoxido
	pinMode(MQ_PIN, INPUT);
	
	// start clock
	if (! rtc.begin()) 
	{
		Serial.println("Couldn't find RTC");
		while (1);
	}
	if (rtc.lostPower()) 
	{
		Serial.println("RTC lost power, lets set the time!");
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	} 
	
	//Ajust inicial de hora
	DateTime now = rtc.now();   
	int horaVal = now.hour();
	t_hora = horaVal;
	t_min = now.minute();
}

//Despliega la pagina
void dashboardPage(EthernetClient &client) 
{
	client.println("<!DOCTYPE HTML><html><head>");
	client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body>");                                                             
	client.println("<h3>Domotica Hogar - Hora: "); 
	client.print(t_hora);
	client.print(':');
	client.print(t_min);
	client.print("  -   ");
	client.println("<a href=\"/\">Refresh</a></h3>");
	client.println("<table border=0>");
	client.println("<tr><td><b>ELEMENTOS</b></td><td><b>STATUS</b></td><td><b>CONTROL</b></td></tr>");

	// Bucle de web
	for ( i = 0; i < maxrelay ; i++ )
	{
		client.println("<tr><td>"+ RelayName[i] +"</td>"); 
		if (RelayState[i]=="OFF")
		{ 
			client.println("<td align=center bgcolor=gray>"); 
		} 
		else 
		{ 
			client.println("<td align=center bgcolor=#1aff1a>");
		} 
		client.println("<fONt color=#ffffff> "+ RelayState[i] + "</td><td align=center>");
		
      if(RelayState[i] == "OFF")
	  {
        client.println("<a href=\"/R"+ RelayPin[i] +"_ON\"><button>ON</button></a></td></tr>");
      }      
      else if(RelayState[i] == "ON")
	  {
        client.println("<a href=\"/R"+ RelayPin[i] +"_OFF\"><button>OFF</button></a></td></tr>");                                                                    
      }    
	}

  // Ajuste variables
	client.println("</table><br><br><b>Umbrales</b><br><table><tr><td>Elemento</td><td>Prendido</td><td>Apagado</td></tr>");   
	for ( i = 0; i < maxrelay ; i++ )
	{
		client.println("<tr><td>"+ RelayName[i] +"</td><td><form><input type=number length=2  name=V"+ RelayPin[i] +"_ON value="+ String(RelayTimeOn[i]) +"><td><input type=submit value=Submit></form></td>");
		client.println("<td><form><input type=number length=2  name=V"+ RelayPin[i] +"_OFF value="+ String(RelayTimeOff[i]) +"><td><input type=submit value=Submit></form></td></tr>");
	}
	client.println("<tr><td>Porcentaje de humedad</td><td><form><input type=number length=2  name=V_1 value="+ String(humedad) +"><td><input type=submit value=Submit></form></td></tr>");
	client.println("</table></body></html>"); 
}


void loop() 
{
	// Verifico hora
	DateTime now = rtc.now();   
	horaVal = now.hour();
	t_hora = horaVal;
	t_min = now.minute();
  
	// Control de luz por horario
	for ( i = 0; i < maxrelay ; i++ )
	{
		if (i == 12) // Relay de pasillo
		{
			if ((horaVal >= RelayTimeOn[i]) and (horaVal >= RelayTimeOff[i]))
			{
			    buttonState = digitalRead(buttonPin);  
				if (buttonState == HIGH)
				{
					digitalWrite(RelayPin[i], LOW);
					RelayState[i] = "ON";
					flag2 = now.minute();
					flag1=flag2;
				}
				else
				{
					flag2 = now.minute();				
					
					if (flag2 > flag1)
					{
						digitalWrite(RelayPin[i], HIGH);
						RelayState[i] = "OFF";
					} 
					else if ((flag1==59) and (flag2==0)) // Ajuste por minuto 59
					{
						digitalWrite(RelayPin[i], HIGH);
						RelayState[i] = "OFF";
					}
				}
			}
		} 
		else
		{
			if (horaVal >= RelayTimeOn[i])
			{
				digitalWrite(RelayPin[i], LOW);
				RelayState[i] = "ON";
			}
			
			if (horaVal >= RelayTimeOff[i])
			{
				digitalWrite(RelayPin[i], HIGH);
				RelayState[i] = "OFF";
			}
		}
	}
	  
	// Control de Extractor
	float humVal = dht.readHumidity();
	if ( humVal > humedad )
	{
		if (RelayState[10] == "OFF")
		{
			digitalWrite(RelayPin[10], LOW);
			RelayState[10] = "ON";
			flag3 = now.minute();
			flag4=flag3;
		}
	}
	else	
	{
		if (RelayState[10] == "ON")
		{
			int flag3 = now.minute();
			flag4=flag4+5;
			if ( flag3 > flag4 )
			{
				digitalWrite(RelayPin[10], HIGH);
				RelayState[10] = "OFF";
			}
		}
	}		

	//Control Monoxido de carbono
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

	if (client) 
	{
		memset(linebuf,0,sizeof(linebuf));
		charcount=0;
		
		// an http request ends with a blank line
		boolean currentLineIsBlank = true;
		while (client.connected()) 
		{
			if (client.available()) 
			{
				char c = client.read();
				//read char by char HTTP request
				linebuf[charcount]=c;
				
				if ( charcount < sizeof(linebuf) - 1 ) charcount++; // if you've gotten to the end of the line (received a newline character) and the line is blank, the http request has ended, so you can send a reply
				if ( c == '\n' && currentLineIsBlank ) 
				{
					dashboardPage(client);
					break;
				}
				if (c == '\n') 
				{
					for ( i = 0; i < maxrelay ; i++ )
					{
						if (strstr(linebuf,"GET /R"+ RelayPin[i] +"_OFF") > 0)
						{
							digitalWrite(RelayPin[i], HIGH);
							RelayState[i] = "OFF";
						}
						else if (strstr(linebuf,"GET /R"+ RelayPin[i] +"_ON") > 0)
						{
						digitalWrite(RelayPin[i], LOW);
						RelayState[i] = "ON";
						}
					}

					// Ajustes horario de prendido
					for ( i = 0; i < maxrelay ; i++ )
					{
					    if (strstr(linebuf,"V"+ RelayPin[i] +"_ON") > 0)
						{
							Cadenas = linebuf;
							int j = Cadenas.indexOf("=");
							if (j>30)
							{
								Cadenas = Cadenas.substring(j+1,j+3);
								int num = Cadenas.toInt();
        						if ((num >=0)and (num <25))
								{
									RelayTimeOn[i]=num;
								}
							}
						}
					}
					
					// Ajustes horario de apagado
					for ( i = 0; i < maxrelay ; i++ )
					{
					    if (strstr(linebuf,"V"+ RelayPin[i] +"_OFF") > 0)
						{
							Cadenas = linebuf;
							int j = Cadenas.indexOf("=");
							if (j>30)
							{
								Cadenas = Cadenas.substring(j+1,j+3);
								int num = Cadenas.toInt();
        						if ((num >=0)and (num <25))
								{
									RelayTimeOff[i]=num;
								}
							}
						}
					}

					// % Humedad
					if (strstr(linebuf,"V_1") > 0)
					{
						Cadenas = linebuf;
						int j = Cadenas.indexOf("=");
						if (j>30)
						{
							Cadenas = Cadenas.substring(j+1,j+3);
							int num = Cadenas.toInt();
							if ((num >=0)and (num <100))
							{
								humedad=num;
							}
						}
					}
          
					// Inicio nueva linea
					currentLineIsBlank = true;
					memset(linebuf,0,sizeof(linebuf));
					charcount=0;          
				} 
				else if (c != '\r') 
				{
					currentLineIsBlank = false; // you've gotten a character ON the current line
				}
			}
		}
		delay(300);		// give the web browser time to receive the data	
		client.stop(); // cierro la conexion
	}
}
