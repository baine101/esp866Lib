#ifndef ESP8266_h
#define ESP8266_h

#include <Arduino.h>
#include <string.h>


class ESP8266 {
  public:
    String cmd="";
    ESP8266();
    void feedbackLoop();
    void sendSerialCmd(String cmd);
    boolean atTest();
    boolean setWifiMode();
    boolean connectWifi(String wifiName ,String wifiPass);
    boolean openTcpConn(String protocol,String ip,String port);
    String startTcpRequest(String httpMethod,String endpoint,String ip,String port);
    
  private:
    boolean ifOkFound(String str);
    boolean ifErrorFound(char str);
    boolean ifACFound(char str);
    boolean ifFound(char str);
  
};

#endif
