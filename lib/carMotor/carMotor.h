/*
    carControl.h - Libreria para el control del carro.
    Libreria personalizada especificamente para proyecto de robotica.
    Creado por Mauricio Lafontant, Mayo 22, 2022.
    Derechos reservados hasta la exposicion del proyecto.
*/

#ifndef carMotor_h
#define carMotor_h

#include "Arduino.h"

class carMotor{
    private:
        int _Min1,_Min2,_lPin,_rPin;
        int _PWMP, _lLED,_rLED;
        int _resolution,_PWMChannel;
        int _motorFrequency;
    public:
        carMotor(int Min1,int Min2,int lPin,int rPin,int PWMP,int lLED,int rLED,int resolution,int PWMChannel,int motorFrequency);
        void fwdDirection(int);
        void bckDirection(int);
        void noDirection();
        void lftDirection();
        void rgtDirection();
        void stpDirection();
        void notDirection();
        void ledBlinking();
};

#endif