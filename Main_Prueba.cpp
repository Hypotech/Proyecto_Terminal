#include <iostream>
#include "Reconocimiento_Facial/ReconocerPersona.hpp"
#include "Deteccion_Facial/RostrosDector.hpp"
#include "Deteccion_Facial/Camara.hpp"
#include"cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    Camara CamLaptop;
    DectorRostros DectorCamLaptop;
    ReconocerdordePersona ReconocedorCamLaptop;
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
		Mat RostroNormalizado( 112,92,ImagenRostro.type() );

		resize(ImagenRostro, RostroNormalizado, RostroNormalizado.size(), 0, 0, INTER_LINEAR);
		cout<< "Rostro Normalizado... finalizado"<< endl;
		int IDpersona = ReconocedorCamLaptop.consulatarBD(RostroNormalizado);

		if(IDpersona == -1)
		    cout<< "Rostro desconocido"<<endl;
		else
		    cout<< "Rostro encontrado: Persona con ID" << IDpersona << endl;
	    }  

	    imshow("Prueba",UnFrame);
	
	    //	   waitKey(0);
	    if(waitKey(30) == 27) break;  
	}
    return 0;
}
