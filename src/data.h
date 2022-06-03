/*
    data.h - Libreria para la data del proyecto.
    Libreria personalizada especificamente para proyecto de robotica.
    Creado por Mauricio Lafontant, Mayo 22, 2022.
    Derechos reservados hasta la exposicion del proyecto.
*/

const char* ssid = "Chuleta_Karting";   //NOMBRE DEL PUNTO DE ACCESO, NO DEBE TENER ESPACIOS
const char* password = "chuletita";     //CONTRASENA DEL PUNTO DE ACCESO, NO MENOS DE 8 CARACTERES, NO ESPACIOS


void starting(){
  Serial.println(" ");
  Serial.println("CONTROL DE CARRO VIA WEB CON ESP32");
  Serial.println("VERSION 1.5");
  Serial.println("UNEFA - DEPARTAMENTO DE INGENIERIA");
  Serial.println("INTRODUCCION A LA ROBOTICA");
  Serial.println("HECHO POR: MAURICIO LAFONTANT & ANDRES GONZALEZ");
  Serial.println(" ");
}
