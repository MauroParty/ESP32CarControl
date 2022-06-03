/*
    carControl.cpp - Libreria para el control del carro.
    Libreria personalizada especificamente para proyecto de robotica.
    Creado por Mauricio Lafontant, Mayo 22, 2022.
    Derechos reservados hasta la exposicion del proyecto.
*/

#include "Arduino.h"
#include "carMotor.h"

carMotor::carMotor(int Min1,int Min2,int lPin,int rPin,int PWMP,int lLED,int rLED,int resolution,int PWMChannel,int motorFrequency){
    _Min1 = Min1; _Min2 = Min2;
    _lPin = lPin; _rPin = rPin;
    _lLED = lLED; _rLED = rLED;
    _PWMP = PWMP;
    _resolution = resolution;
    _PWMChannel = PWMChannel;
    _motorFrequency = motorFrequency;

    pinMode(_PWMP,OUTPUT);
    ledcAttachPin(_PWMP,_PWMChannel);
    ledcSetup(_PWMChannel,_motorFrequency,_resolution);

    pinMode(_Min1,OUTPUT); digitalWrite(_Min1,LOW);
    pinMode(_Min2,OUTPUT); digitalWrite(_Min2,LOW);
    pinMode(_lPin,OUTPUT); digitalWrite(_lPin,LOW);
    pinMode(_rPin,OUTPUT); digitalWrite(_rPin,LOW);
    pinMode(_lLED,OUTPUT); digitalWrite(_Min1,LOW);
    pinMode(_rLED,OUTPUT); digitalWrite(_rLED,LOW);
}

void carMotor::fwdDirection(int revolution){
    if((revolution<=0)||(revolution>=255)){
        return;
    }else{
        digitalWrite(_Min1, LOW);
        digitalWrite(_Min2,HIGH);

        ledcWrite(_PWMChannel,revolution);

    }
    
}

void carMotor::bckDirection(int revolution){
    if((revolution<=0)||(revolution>=255)){
        return;
    }else{
        digitalWrite(_Min1,HIGH);
        digitalWrite(_Min2, LOW);

        digitalWrite(_lLED,HIGH);
        digitalWrite(_rLED,HIGH);

        ledcWrite(_PWMChannel,revolution);
    }
}

void carMotor::lftDirection(){
    digitalWrite(_lPin,HIGH); digitalWrite(_lLED,HIGH);
    digitalWrite(_rPin, LOW); digitalWrite(_rLED, LOW);
}

void carMotor::rgtDirection(){
    digitalWrite(_lPin, LOW); digitalWrite(_lLED, LOW);
    digitalWrite(_rPin,HIGH); digitalWrite(_rLED,HIGH);
}

void carMotor::noDirection(){
    digitalWrite(_lPin, LOW); digitalWrite(_lLED, LOW);
    digitalWrite(_rPin, LOW); digitalWrite(_rLED, LOW);
}

void carMotor::stpDirection(){
    digitalWrite(_Min1,LOW); digitalWrite(_lLED,LOW);
    digitalWrite(_Min2,LOW); digitalWrite(_rLED,LOW);
    digitalWrite(_lPin,LOW); digitalWrite(_rPin,LOW);
    ledcWrite(_PWMChannel,0);
}

void carMotor::ledBlinking(){
    digitalWrite(_lLED,HIGH); digitalWrite(_rLED,HIGH);
    delay(200);
    digitalWrite(_lLED, LOW); digitalWrite(_rLED, LOW);
    delay(200);
}
