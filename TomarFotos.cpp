//#include "Camara/Camara.hpp"
#include "Deteccion_Facial/RostrosDector.hpp"
#include "Comunicacion/Receptor/ReceptorDeImag.hpp" //Para recibir imagenes de la raspberry
#include <stdio.h>
#define IP_SERVIDOR "192.168.1.116"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

int main(int argc, char *argv[])
{
    //Camara CamLaptop;
    ReceptorDeImagen RaspiCam(IP_SERVIDOR);
    DectorRostros DectorCamLaptop;
    Mat UnFrame;
    Scalar colorAzul (255,0,0);
    int x = 1;
    char path[50];
    vector<Rect> MarcosRostros;

    vector<int> compresion_param;
    compresion_param.push_back(CV_IMWRITE_PXM_BINARY);
    compresion_param.push_back(1);

    if( RaspiCam.EsperarConexion() < 0) //esperamos a que la raspberry se conecte
	{
	    printf( "**Error al intentar esperar la conexion\nSaliendo de la aplicacion...\n");
	    return -2;
	}

    while(RaspiCam.Recibir(UnFrame) == true)
	{
	    DectorCamLaptop.EscanearImagen(UnFrame, MarcosRostros);

	    for (vector<Rect>::const_iterator Rostro = MarcosRostros.begin() ; Rostro != MarcosRostros.end() ; Rostro++){

		rectangle(UnFrame, *Rostro, colorAzul); //( Mat& input , Rect , const Scalar& color, int thickness, int line type, int shift ), se dibuja un rectangulo en la imagen original

		if(waitKey(1) == 't') 
		    {
			sprintf(path,"%d.pgm",x);

			Mat ImagenRostro;
			// cvtColor(UnFrame(*Rostro), ImagenRostro,CV_BGR2GRAY);
			ImagenRostro = UnFrame(*Rostro);

			Mat RostroNormalizado(atoi(argv[2]), atoi(argv[1]) ,ImagenRostro.type() );
			resize(ImagenRostro, RostroNormalizado, RostroNormalizado.size(), 0, 0, INTER_LINEAR);
			imwrite(path,RostroNormalizado, compresion_param); //se almacena la foto
			x++;
		    }
	    }
	    
	    imshow("Camara", UnFrame);

	    if(waitKey(15) == 27)
		break;

	}
    return 0;
}
