#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <Time.h>
#include <EEPROM.h>
#include <ctime>
#include <NTPClient.h>
#include <ESP8266HTTPClient.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
int DeviceID=1002;
const char* ssid = "shree";
const char* password = "12345678";

WiFiClient client;
ESP8266WebServer server(80); 
String serverName = "http://114.143.180.194:8077/API/api/values/Get?value=";

String Argument_Name, Clients_Response;
const int analog_ip = A0;
int ledPin = 13; // D7
int ledPin2 = 14; //D5
int ledPin3 = 15; // D8
int ledPin4 = 16; // D0
int ledPin5 = 5; //D1
int ledPin6 = 4; //D2
int ledPin7 = 0; //D3
int ledPin8 = 2; //D4
String HH1;
String MM1;
String SS1;
String HH2;
String MM2;
String SS2;

String TT1;
String TT2;
String TT3;
String TT4;
String TT5;
String TT6;

int ArmDisarmStatus;
void GetStatus()
{
  EEPROM.begin(50);
  HH1=EEPROM.read(0);
  HH1+=EEPROM.read(1);
  MM1=EEPROM.read(2);
  MM1+=EEPROM.read(3);
  SS1=EEPROM.read(4);
  SS1+=EEPROM.read(5);
  EEPROM.begin(56);
  TT1=EEPROM.read(6);
  TT1+=EEPROM.read(7);
  TT1+=EEPROM.read(8);
  TT1+=EEPROM.read(9);
  TT1+=EEPROM.read(10);
  TT1+=EEPROM.read(11);
  EEPROM.begin(62);
  ArmDisarmStatus=(int)EEPROM.read(12);
}
void setup() 
{
  Serial.begin(115200); 
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, INPUT_PULLUP);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin6, OUTPUT);
  pinMode(ledPin7, OUTPUT);
  pinMode(ledPin8, OUTPUT);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);
  digitalWrite(ledPin5, LOW);
  digitalWrite(ledPin6, LOW);
  digitalWrite(ledPin7, LOW);
  digitalWrite(ledPin8, LOW);
  Serial.print("Connecting to ");
  Serial.println(ssid); 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started");
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  GetStatus();
  server.on("/", HandleClient);
}
int tm[6];
int tm1[6];
void HandleClient()
{      
  String webpage;
    webpage =  "<html>";
    webpage += "<head><title>WELCOME</title>";
    webpage += "<style>";
    webpage += "body { background-color: #E6E6FA; font-family: Arial, Helvetica, Sans-Serif; Color: blue;}";
    webpage += "</style>";
    webpage += "</head>";
    webpage += "<body>";
    String IPaddress = WiFi.localIP().toString();
    webpage += "<form action='http://"+IPaddress+"' method='POST'>";     
    webpage += "<div style='width:800px; margin:0 auto; text-align:center; align:center; border:yellow'>";    
    webpage += "<div style='background-color: #6E7BBB;'><h1><font color='white'>SHREE ELECTRICALS</font></h1></div>";
    webpage += "<div style='background-color: #6E7BBB ;'><h2><font color='white'>INTERFACE TO BOARD</font></h2></div>";
    webpage += "<div style='background-color: #6E7BBB;'><font color='white'>SCHEDULE CONTROL FOR <b>INPUT 1</b> ( HH:MM:SS ) :</font><br><input type='text' name='user_input'>&nbsp;<input type='submit' value='SET'><br></div>";
    if(ArmDisarmStatus==1)
    {
     webpage += "<h2 style='background-color: green;'><font color='white'>SYSTEM IS AMRED</font></h2>";
    }
    else
    {
     webpage += "<h2 style='background-color: red;'><font color='white'>SYSTEM IS DISAMRED</font></h2>";
    }
    webpage += "<div style='background-color: lightblue;'><h3>MANUAL OUTPUT CONTROL</h3></div>";
    
    webpage += "<div style='background-color: lightblue;'><button name='on1'> ON_1 </button>";     
    webpage += "<button name='off1'> OFF_1 </button><br>";
    webpage += "<button name='on2'> ON_2 </button>";
    webpage += "<button name='off2'> OFF_2 </button><br>";
    webpage += "<button name='on3'> ON_3 </button>";
    webpage += "<button name='off3'> OFF_3 </button><br>";
    webpage += "<button name='on4'> ON_4 </button>";
    webpage += "<button name='off4'> OFF_4 </button><br>";
    webpage += "<button name='on5'> ON_5 </button>";
    webpage += "<button name='off5'> OFF_5 </button><br>";
    webpage += "<button name='on6'> ON_6 </button>";
    webpage += "<button name='off6'> OFF_6 </button><br>";
    webpage += "<button name='on7'> ON_7 </button>";
    webpage += "<button name='off7'> OFF_7 </button><br>";
    webpage += "<button name='on8'> ON_8 </button>";
    webpage += "<button name='off8'> OFF_8 </button><br><br>";
    // webpage += "<button name='RESET'> SOFT RESET </button><br><br>";
    webpage += "<button name='ARMI1' style='background-color: red;'> ARM </button><br><br>";
    webpage += "<button name='DISARMI1' style='background-color: green;'> DISARM </button></div><br><br>";
    webpage += "</div>";
    webpage += "</form>";
    webpage += "</body>";
    webpage += "</html>";
    server.send(200, "text/html", webpage); // Send a response to the client asking for input
    if (server.args() > 0 ) { // Arguments were received
    char test[8]={};
    for ( uint8_t i = 0; i < server.args(); i++ ) 
    {
      Serial.print(server.argName(i)); // Display the argument
      Argument_Name = server.argName(i);
      if (server.argName(i) == "user_input" && server.arg(i) != 0 ) 
      {
            Serial.println(server.arg(i));
            Clients_Response = server.arg(i);
            EEPROM.begin(50);                             
            for(int cc=0 ;cc<6;cc++)
            {
              int value = server.arg(i)[cc]-'0'; //convert char to int
              EEPROM.write(cc,value);
              tm[cc]= value;
              EEPROM.commit();                           
              delay(500);
            }
            GetStatus();
            CallAPI("?ScheduleIsActivatedFor?"+HH1+","+MM1+","+SS1);
      }

      if (server.argName(i) == "user_input1" && server.arg(i) != 0 ) 
      {
            Serial.println(server.arg(i));
            Clients_Response = server.arg(i);
            EEPROM.begin(56);                             
            for(int cc_1=6 ;cc_1<11;cc_1++)
            {
              int value = server.arg(i)[cc_1]-'0'; //convert char to int
              EEPROM.write(cc_1,value);
              tm1[cc_1]= value;
              EEPROM.commit();                           
              delay(500);
            }
            GetStatus();
      }
      //1 ON OFF 
      if (server.argName(i) == "on1") 
      {
        if(ArmDisarmStatus==1)
        {
          digitalWrite(ledPin, HIGH);
          CallAPI("?INPUT_NUMBER_ONE_IS_ON");
        }
      }     
      if (server.argName(i) == "off1") 
      {
        if(ArmDisarmStatus==1)
          {
           digitalWrite(ledPin, LOW);
           CallAPI("?INPUT_NUMBER_ONE_IS_OFF");
          }
      }
      //2
      if (server.argName(i) == "on2") 
      {
        if(ArmDisarmStatus==1)
        {
          digitalWrite(ledPin2, HIGH);
          CallAPI("?INPUT_NUMBER_TWO_IS_ON");
        }
      }
      if (server.argName(i) == "off2") {
        if(ArmDisarmStatus==1)
        {
          digitalWrite(ledPin2, LOW);
          CallAPI("?INPUT_NUMBER_TWO_IS_OFF");
        }
      }
      //3
      if (server.argName(i) == "on3") 
      {
        if(ArmDisarmStatus==1)
        {
          digitalWrite(ledPin3, HIGH);
          CallAPI("?INPUT_NUMBER_THREE_IS_ON");
        }
      }
      if (server.argName(i) == "off3") 
      {
        if(ArmDisarmStatus==1)
        {
          digitalWrite(ledPin3, LOW);
          CallAPI("?INPUT_NUMBER_THREE_IS_OFF");
        }
      }
      //4
      if (server.argName(i) == "on4") 
      {
        if(ArmDisarmStatus==1)
          {
            digitalWrite(ledPin4, HIGH);
            CallAPI("?INPUT_NUMBER_FOUR_IS_ON");
          }
      }
      if (server.argName(i) == "off4") 
      {
        if(ArmDisarmStatus==1)
          {
            digitalWrite(ledPin4, LOW);
            CallAPI("?INPUT_NUMBER_FOUR_IS_OFF");
          }
      }
      //5
      if (server.argName(i) == "on5") 
      {
        if(ArmDisarmStatus==1)
          {
            digitalWrite(ledPin5, HIGH);
            CallAPI("?INPUT_NUMBER_FIVE_IS_ON");
          }
      }
      if (server.argName(i) == "off5") 
      {
        if(ArmDisarmStatus==1)
          {
            digitalWrite(ledPin5, LOW);
            CallAPI("?INPUT_NUMBER_FIVE_IS_OFF");
          }
      }
      //6
      if (server.argName(i) == "on6") 
      {
        if(ArmDisarmStatus==1)
          {
            digitalWrite(ledPin6, HIGH);
            CallAPI("?INPUT_NUMBER_SIX_IS_ON");
          }
      }
      if (server.argName(i) == "off6") 
      {
        if(ArmDisarmStatus==1)
          {         
            digitalWrite(ledPin6, LOW);
            CallAPI("?INPUT_NUMBER_SIX_IS_OFF");
          }
      }
      //7
      if (server.argName(i) == "on7") 
      {
        if(ArmDisarmStatus==1)
        {
          digitalWrite(ledPin7, HIGH);
          CallAPI("?INPUT_NUMBER_SEVEN_IS_ON");
        }
      }
      if (server.argName(i) == "off7") 
      {
        if(ArmDisarmStatus==1)
        {
          digitalWrite(ledPin7, LOW);
          CallAPI("?INPUT_NUMBER_SEVEN_IS_OFF");
        }
      }
      //8
      if (server.argName(i) == "on8") 
      {
        if(ArmDisarmStatus==1)
        {
          digitalWrite(ledPin8, HIGH);
          CallAPI("?INPUT_NUMBER_EIGHT_IS_ON");
        }
      }
      if (server.argName(i) == "off8") 
      {
        if(ArmDisarmStatus==1)
          {
            digitalWrite(ledPin8, LOW);
            CallAPI("?INPUT_NUMBER_EIGHT_IS_OFF");
          }
      }
      //SYSTEM IS ARMED
      if (server.argName(i) == "ARMI1") 
      {
              Serial.println(server.arg(i));
              Clients_Response = server.arg(i);
              EEPROM.begin(62);                           
              EEPROM.write(12,1);
              EEPROM.commit();
              delay(500);
              GetStatus();
              CallAPI("?SYSTEM_IS_ARMED");
      }
      //SYSTEM IS DISARMED
      if (server.argName(i) == "DISARMI1") 
      {
              digitalWrite(ledPin2,LOW);
              Serial.println(server.arg(i));
              Clients_Response = server.arg(i);
              EEPROM.begin(62); 
              EEPROM.write(12,0);
              EEPROM.commit();       
              delay(500); 
              GetStatus();
              CallAPI("?SYSTEM_IS_DISARMED");
      }
     /*if (server.argName(i) == "RESET") 
      {     
        Serial.println("In RESET");
        EEPROM.begin(50);
        HH1=EEPROM.read(0);
        HH1+=EEPROM.read(1);

        MM1=EEPROM.read(2);
        MM1+=EEPROM.read(3);
        SS1=EEPROM.read(4);
        SS1+=EEPROM.read(5);

        EEPROM.begin(56);
        TT1=EEPROM.read(6);
        TT1+=EEPROM.read(7);
        TT1+=EEPROM.read(8);
        TT1+=EEPROM.read(9);
        TT1+=EEPROM.read(10);
        TT1+=EEPROM.read(11);        
        EEPROM.begin(62);        
        ArmDisarmStatus=(int)EEPROM.read(12);
        Serial.println(HH1);
        Serial.println(MM1);
        Serial.println(SS1);
        Serial.println(TT1);
      }*/
    }
  }
}

int timer=0;
int IsOn=0;
void loop()
{
  if(ArmDisarmStatus==1)
  {
    if(digitalRead(ledPin2)==1)
    {
      if(IsOn==0)
      {
        IsOn=1;
        CallAPI("?DoorIsOpened?1001");
      }      
      digitalWrite(ledPin3,HIGH);
    }
    else 
    {
       IsOn=0;
       digitalWrite(ledPin3,LOW);
    }
  }
  timer++;
  timeClient.update();
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int currentSecond = timeClient.getSeconds();
  if(currentHour==HH1.toInt() && currentMinute==MM1.toInt() && currentSecond==SS1.toInt())
  {
      digitalWrite(ledPin, HIGH);
      timer=0;
  }
  if(timer==5)
  {
      digitalWrite(ledPin,LOW);
  }  
  server.handleClient();
  delay(3000);
}

void CallAPI(String Call)
{
    HTTPClient http;
    http.setReuse(true);
    String serverPath = serverName + Call;//Device id is 1001
    WiFiClient secureClient;
    Serial.println(serverPath);
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode>0)
    {
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else
    {
      Serial.println(httpResponseCode);
    }
    http.end();
}
