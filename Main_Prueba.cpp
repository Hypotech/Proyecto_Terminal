#include <iostream>
#include "Reconocimiento_Facial/ReconocerPersona.hpp"
#include "Deteccion_Facial/RostrosDector.hpp"
#include "Camara/Camara.hpp"
#include"cv.hpp"
#include <iostream>
#define ESCALA_TEXT 2

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    Camara CamLaptop;
    DectorRostros DectorCamLaptop;
    ReconocerdordePersona ReconocedorCamLaptop(atoi(argv[3]),atof(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));
    //ReconocerdordePersona ReconocedorCamLaptop(atoi(argv[3]),atof(argv[4]) );
    Mat UnFrame;
    vector<Rect> MarcosRostros;
    Scalar colorAzul(255,0,0); //representa el color azul en BGR

    while( CamLaptop.GetFrame(UnFrame) == true)
	{
	    if ( DectorCamLaptop.EscanearImagen(UnFrame, MarcosRostros) == false )
		{
		    cout<< "No se pudo cargar el clasificador\nSaliendo de la aplicacion..."<<endl;
		    return -2;
		}
	   
	    for (vector<Rect>::const_iterator Rostro = MarcosRostros.begin() ; Rostro != MarcosRostros.end() ; Rostro++){

		Mat ImagenRostro;

		rectangle(UnFrame, *Rostro, colorAzul); //( Mat& input , Rect , const Scalar& color, int thickness, int line type, int shift ), se dibuja un rectangulo en la imagen original

		cvtColor(UnFrame(*Rostro), ImagenRostro,CV_BGR2GRAY);
		Mat RostroNormalizado(atoi(argv[2]), atoi(argv[1]) ,ImagenRostro.type() );

		resize(ImagenRostro, RostroNormalizado, RostroNormalizado.size(), 0, 0, INTER_LINEAR);
		int IDpersona = ReconocedorCamLaptop.consulatarBD(RostroNormalizado);

		Point P(Rostro->x, Rostro->y);

		if(IDpersona == -1){
		    putText(UnFrame,"Rostro desconocido" ,P ,FONT_HERSHEY_PLAIN, ESCALA_TEXT, colorAzul);
		    cout<< "Rostro desconocido"<<endl;
		}
		else if(IDpersona == 1){
		    putText(UnFrame, "Christian",P ,FONT_HERSHEY_PLAIN, ESCALA_TEXT, colorAzul);
		    cout<< "Rostro encontrado: Persona con ID " << IDpersona << endl;
		}
		else{
		    putText(UnFrame,format("Subjeto ID %d", IDpersona),P ,FONT_HERSHEY_PLAIN, ESCALA_TEXT, colorAzul);
		    cout<< "Rostro encontrado: Persona con ID " << IDpersona << endl;
		}
	    }  

	    imshow("Prueba",UnFrame);
	
	    if(waitKey(30) == 27) break;  
	}
    return 0;
}
