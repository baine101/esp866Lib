/**
Author : Alex Waine 
Website : alexwaine.co.uk
**/

#include <Arduino.h>
#include <ESP8266.h>
#include <string.h>

#include <SoftwareSerial.h>

SoftwareSerial espSerialLib(6, 5); // RX, TX

ESP8266::ESP8266(){
  // Connects ESP8266 Wifi IC
   uint32_t baud = 9600 ;
   espSerialLib.begin(baud);

}

void ESP8266::feedbackLoop(){
  
  //impliment serial view debugging 
  while(espSerialLib.available() > 0) 
    {
      char a = espSerialLib.read();
      if(a == '\0')
        continue;
      if(a != '\r' && a != '\n' && (a < 32))
        continue;
      Serial.print(a);
    }

    
  //impliment serial view debugging
    while(Serial.available() > 0)
    {
      char a = Serial.read();
      Serial.write(a);
      espSerialLib.write(a);
    }

}


//** Dynamic Functions ***//


boolean ESP8266::ifOkFound(String str){
   
  if(espSerialLib.find("OK"))
  {
    String string = str + " completed";
    return true;
  }
  else
  {
    String string = str + " failed";
    return false;
  }
}

boolean ESP8266::ifErrorFound(char str){
  
  if(espSerialLib.find("ERROR"))
  {
    String string = str + "ERROR";
    return true;
  }
  else
  {
    String string = str + "NO ERROR FOUND BUT STILL DOSENT MEAN THAT THEIR ISNT ONE";
    return false;
  }
}

boolean ESP8266::ifACFound(char str){
  
  if(espSerialLib.find("ALREADY CONNECTED"))
  {
    String string = str + " completed";
    return true;
  }
  else
  {
    String string = str + " failed";
    return false;
  }
}

boolean ESP8266::ifFound(char str){
  
  if(espSerialLib.find(str))
  {
    String string = str + " completed";
    return true;
  }
  else
  {
    String string = str + " failed";
    return false;
  }
}

void ESP8266::sendSerialCmd(String cmd){
  // Append command with "AT+"
  String newCmd = "AT+" + cmd;
  // Send command to esp  & monitor
  espSerialLib.println(newCmd);
  Serial.println(newCmd);
}


//*** Command Functions ***//


boolean ESP8266::atTest(){
    String cmd = "AT";
    espSerialLib.println(cmd);
    delay(25);
  return ifOkFound(cmd);

}

boolean ESP8266::setWifiMode(){
  
  String cmd = "CWMODE=3";
  sendSerialCmd(cmd);
  
  String str = "AT+CWMODE=3";
  bool wifi = ifOkFound(str);
  return wifi;

}

boolean ESP8266::connectWifi(String wifiName ,String wifiPass){

  // String cmd is populated by the NFC process
  String cmd="CWJAP=\""; 
 
 // String SSID = "TNCAP8F4AB5";
 // String PASS = "Blink182"; 
  
  cmd+=wifiName;
  cmd+="\",\"";
  cmd+=wifiPass;
  cmd+="\"";
  
  // Send the command
  sendSerialCmd(cmd);
  //delay(100);
  bool conn = ifOkFound(cmd);

  return conn;
}

// openTcpConn(){
boolean ESP8266::openTcpConn(String protocol,String ip,String port){
  /* For Future 
  //     AT+CIPSTART="TCP","192.168.1.64",80
  */
  
  bool result = false;

    String cmd="CIPSTART=\""; 
  
  cmd+=protocol;
  cmd+="\",\"";
  cmd+=ip;
  cmd+="\",";
  cmd+=port;
  cmd+="";
  
  // AT+CIPSTART="type","ip",port;
 // String cmd = "CIPSTART=\"TCP\",\"192.168.1.64\",80";
  sendSerialCmd(cmd);
 
    delay(100);
    
     
    if (ifOkFound("OK")){
       result = true;
    }
     
 /*  if (ifACFound("ALREADY CONNECTED")){
       Serial.println("ALREADY CONNECTED");
       result = true;
   }
   result = true;
    */ 

    
    return result;

}



String ESP8266::startTcpRequest(String httpMethod,String endpoint,String ip,String port){  

  //  delay(50);

    String request = httpMethod; 
    request+=" ";
    request+=endpoint;
    request+=" HTTP/";
    request+="1.0";
    request+=" Host: ";
    request+=ip;
    request+=":";
    request+=port;
    request+=" Connection: close";

  int reqLength = request.length();
    Serial.println(reqLength);
    
     // AT+CIPSEND=<LENGTH OF SSTRING>
    String cmd = "CIPSEND=69";
    //cmd += reqLength; 
    sendSerialCmd(cmd);  
  //  delay(150);
    
    // Send GET Request String & Close The Connection 
     Serial.println(request);
     espSerialLib.println(request);
     espSerialLib.println("");
     espSerialLib.println("");
 
  
   // Get & Return The HTTP Response                      
  String httpResponse = ""; 
    
  while(espSerialLib.available()){
     httpResponse = espSerialLib.readStringUntil("CLOSED");
     //Serial.println(httpResponse);
     return httpResponse;
  }
   
}
