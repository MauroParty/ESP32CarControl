//CONTROL DE CARRO VIA WEB CON ESP32
//VERSION 1.5
//UNEFA - DEPARTAMENTO DE INGENIERIA
//INTRODUCCION A LA ROBOTICA
//HECHO POR: MAURICIO LAFONTANT

//LIBRERIAS DE ARDUINO Y SISTEMA DE ARCHIVOS SPIFFS
#include <Arduino.h>
#include "SPIFFS.h"
#include <data.h>
//LIBRERIAS PARA WIFI SERVER Y WEBSOCKET
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ESPAsyncWebServer.h>
//LIBRERIA PARA BUZZER Y CARRO
#include <Tone32.h>                      //LIBRERIA PERSONALIZADA
#include <carMotor.h>                    //LIBRERIA PERSONALIZADA

//DEFINICIONES DE PINES
#define Min1 14
#define Min2 12
#define PWMP 27
#define lPin 32
#define rPin 33
#define lLED 25
#define rLED 26
#define bPin 13

//DEFINCIONES DE CARACTERISTICAS DEL MOTOR Y BUZZER
#define motorFrequency 30000
#define PWMPChannel        1
#define bzzrChannel        2
#define resolution         8

//SERVIDOR DEFINIDO EN PUERTO [80]
WebServer server(80);
//SERVIDOR WEBSOCKET DEFINIDO EN PUERTO [81]
WebSocketsServer webSocket = WebSocketsServer(81);
//CARRO [LIBRERIA PERSONALIZADA]
carMotor car(Min1,Min2,lPin,rPin,PWMP,lLED,rLED,resolution,PWMPChannel,motorFrequency);

//ESTADOS DE DIRECCION
bool leftState = false;
bool righState = false;
bool conectionStatus = false;
/*-------------------------------------------------------------------------------------------------------*/
/*                                          CONTENT TYPE FUNCTION                                        */
/*-------------------------------------------------------------------------------------------------------*/
//OBTENER EL TIPO DE CONTENIDO DEL SISTEMA DE ARCHIVOS
String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
//NOTA: ESTO SOLO FUNCIONA SI SE USA LA LIBRERIA SPIFFS PARA CREAR EL SISTEMA DE ARCHIVOS

/*-------------------------------------------------------------------------------------------------------*/
/*                                          CAR CONTROL FUNCTION                                         */
/*-------------------------------------------------------------------------------------------------------*/
void carControl(uint8_t* payload){
  switch(payload[0]){
    case 'F':{
      String stringLoad = String((const char*)payload);
      stringLoad.remove(0,1);
      uint8_t direction = stringLoad.toInt();
      Serial.print("Adelante con ");
      Serial.println(direction);
      car.fwdDirection(direction);
      break;
    }
    case 'B':{
      String stringLoad = String((const char*)payload);
      stringLoad.remove(0,1);
      uint8_t direction = stringLoad.toInt();
      Serial.print("Atras con ");
      Serial.println(direction);
      car.bckDirection(direction);
      break;
    }
    case 'R':{
      righState = !righState;
      if(righState) car.rgtDirection();
      else car.noDirection();
      break;
    }
    case 'L':{
      leftState = !leftState;
      if(leftState) car.lftDirection();
      else car.noDirection();
      break;
    }
    case 'S':{
      buzzerLoud(2,bPin,bzzrChannel);
      break;
    }
    case 'D':{
      car.stpDirection();
      return;
    }
  }
  if(leftState && righState){
    leftState = righState = false;
    car.noDirection();
  }
}

/*-------------------------------------------------------------------------------------------------------*/
/*                                      WEBSOCKET EVENT FUNCTION                                         */
/*-------------------------------------------------------------------------------------------------------*/
//AQUI ES DONDE SE RECIBE TODA LA INFORMACION OBTENIDA DEL CLIENTE
void webSocketEvent(uint8_t num,WStype_t type,uint8_t*payload,size_t length){
  Serial.printf("webSocketEvent(%d,%d,...\r\n",num,type);
  switch(type){
    case WStype_CONNECTED:{              //WEBSOCKET CONECTADO
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Conectado desde %d.%d.%d.%d url:%s\r\n",num,ip[0],ip[1],ip[2],ip[3],payload);
      conectionStatus = true;
      break;
    }
    case WStype_DISCONNECTED:{           //WEBSOCKET DESCONECTADO
      Serial.printf("[%u] Desconectado.\r\n",num);
      conectionStatus = false;
      break;
    }
    case WStype_TEXT:{                   //RECEPCION DE CADENA PARA EL CONTROL DEL CARRITO
      carControl(payload);
      break;
    }
  }
}

/*-------------------------------------------------------------------------------------------------------*/
/*                                       SPIFFS PROTOCOLE FUNCTION                                       */
/*-------------------------------------------------------------------------------------------------------*/
//VERIFICACION DE DATOS HTML
//AGREGADO EXTENSION PARA CUMPLIR CON EL PROTOCOLO HTTP
bool handleFileRead(String path){

  Serial.println(path);

  #ifdef DEBUG
    Serial.println("handleFileRead: " + path);
  #endif

  if(path.endsWith("/")) path += "car.html";
  if(SPIFFS.exists(path)){
    File file = SPIFFS.open(path,"r");   //LECTURA DE HTML
    size_t sent = server.streamFile(file,getContentType(path));
    file.close();
    return true;
  }
  return false;
}

/*-------------------------------------------------------------------------------------------------------*/
/*                                          SETUP FUNCTION                                               */
/*-------------------------------------------------------------------------------------------------------*/
void setup(){
    Serial.begin(115200);               //INICIO DE PUERTO SERIAL
    starting();                         //INICIALIZACION EN PUERTO SERIAL

    WiFi.softAP(ssid,password);         //INICIO DE CONEXION WIFI. NOTA: EN CASO DE NO QUERERE CONTRASENA, NO PONERLA.  WiFi.softAP(ssid);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("Direccion IP de punto de acceso: ");
    Serial.println(IP);

    if(!SPIFFS.begin(true)){
        Serial.println("SPPIFFS no ha sido detectado");
        return;
    }

    SPIFFS.begin();                     //INICIO DE SISTEMA DE ARCHIVOS

    webSocket.begin();                  //INICIO DE WEBSOCKET 
    webSocket.onEvent(webSocketEvent);

    server.onNotFound([](){
    if(!handleFileRead(server.uri())) server.send(404,"text/plain","FileNotFound");
    });

    server.begin();                     //INICIO DE SERVIDOR HTTP

    digitalWrite(lLED,HIGH);
    digitalWrite(rLED,HIGH);
    buzzerLoud(1,bPin,bzzrChannel);     //BUZZER INDICANDO FUNCION SETUP FINALIZADO
    delay(1000);
    digitalWrite(lLED,LOW);
    digitalWrite(rLED,LOW);
}

/*-------------------------------------------------------------------------------------------------------*/
/*                                          LOOP FUNCTION                                                 */
/*-------------------------------------------------------------------------------------------------------*/
void loop(){
  webSocket.loop();                      //SE ACTUALIZA EL WEBSOCKET
  server.handleClient();                 //SE OBTIENE LA CADENA DE DATOS DEL CLIENTE
  if(!conectionStatus) car.ledBlinking();//HASTA NO CONECTARSE, LOS LEDS PARPADEARAN ETERNAMENTE, AMEN.
}