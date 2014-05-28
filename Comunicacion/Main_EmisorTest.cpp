#include "Emisor/EmisorDeImag.hpp"
#define IP_SERVIDOR "192.168.1.116"
#include "RaspiCamCV.h"
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "Deteccion_Facial/RostrosDector.hpp"
#include "cv.hpp"

int main(int argc, char *argv[])
{
    EmisorDeImagenes Raspberry(IP_SERVIDOR);
    RaspiCamCvCapture* ptrVideoStruct1 = raspiCamCvCreateCameraCapture(0); // structura que apunta al flujo de video camara
    DectorRostros DectorRaspiCam;
    vector<Rect> MarcosRostros;

    IplImage* ptrFrame; //apuntador a un frame del video
    int i=0;

    while(true) {
	ptrFrame = raspiCamCvQueryFrame ( ptrVideoStruct1 );
	cv::Mat FrameCopia (ptrFrame);
	cv::Mat FrameEnviar;

	cvtColor(FrameCopia,FrameEnviar,CV_BGR2GRAY);
	// DectorRaspiCam.EscanearImagen(FrameCopia, MarcosRostros);

	// if( !MarcosRostros.empty() )
	//     std::cout << "Rostro Detectado" << std::endl;
	// else{
	//     std::cout << "Rostro no detectado" << std::endl;
	// }

	// for (vector<Rect>::const_iterator Rostro = MarcosRostros.begin() ; Rostro != MarcosRostros.end() ; Rostro++){
	//     FrameCopia(*Rostro);

	//     cv::Mat FrameEnviar (150,150, FrameCopia.type());
	//     resize(FrameCopia, FrameEnviar, FrameEnviar.size(), 0, 0, INTER_LINEAR);
	    
	    Raspberry.enviar(FrameEnviar);
	    // Raspberry.enviar(FrameEnviar);

	//     std::cout << "Imagen Enviada con exito" << std::endl;
	//     // cv::waitKey(50);
	// }
    }

    return 0;
}

