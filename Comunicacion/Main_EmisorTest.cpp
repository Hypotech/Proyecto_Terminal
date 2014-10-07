#include "Emisor/EmisorDeImag.hpp"
#define IP_SERVIDOR "192.168.1.116"
#include "RaspiCamCV.h"
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "cv.hpp"
#include <wiringPi.h>

#define ABRIR 100

//################### Funciones Auxiliares ##########################//
int ConfigwiringPi();  //Configura el uso de GPIO
void AbrirPuerta();    //Manda senyal a chip para abrir puerta
void DesactivarChip(); //Desactiva chip
//##################################################################//

int main(int argc, char *argv[])
{
    EmisorDeImagenes Raspberry(IP_SERVIDOR); //Para la emision de imagenes hacia al srevidor
    RaspiCamCvCapture* ptrVideoStruct1 = raspiCamCvCreateCameraCapture(0); // structura que apunta al flujo de video camara

    if(ConfigwiringPi() == -1){
	std::cout << "Fallo wiringPi" << std::endl;
	return -1;
    } 

    IplImage* ptrFrame; //apuntador a un frame de video
    int i=0;

    // int Retardo = 0; //conteo de retardo
    while(true) {
	ptrFrame = raspiCamCvQueryFrame ( ptrVideoStruct1 );
	cv::Mat FrameCopia (ptrFrame);
	cv::Mat FrameEnviar;

	cvtColor(FrameCopia,FrameEnviar,CV_BGR2GRAY);

	if( Raspberry.enviar(FrameEnviar) <= 0){
	    std::cout << "\t\tConexion Perdida" << std::endl;
	    std::cout << "Saliento de la aplicacion" << std::endl;
	    break;
	}

	if(Raspberry.GetRespuesta() == ABRIR ){
	    std::cout << "**Abrir puerta**" << std::endl;
	    AbrirPuerta();
	    delay(500);
	    DesactivarChip(); //Desactiva el chip para evitar fatigar el motor
	}
    }

    return 0;
}

int ConfigwiringPi() //Especifica el convenio de numeracion de los pines y los declara c/salida
{

#define PIN8 15 //Pin 8 fisico
#define PIN10 16 //Pin 10 fisico
#define ENABLECHIP 1 //pin 12 fisico

    if(wiringPiSetup() == -1){
	return -1;
    }    

    pinMode(PIN8,OUTPUT);
    pinMode(PIN10,OUTPUT);
    pinMode(ENABLECHIP,OUTPUT);

    return 0;
}

void AbrirPuerta() //Indica al chip que gire indefinidamente el motor para abrir la puerta
{
    digitalWrite(PIN8,1);
    digitalWrite(PIN10,0);
    digitalWrite(ENABLECHIP,1);
}

void DesactivarChip() 
{
    digitalWrite(ENABLECHIP,0);
    digitalWrite(PIN8,0);
    digitalWrite(PIN10,1);
}
