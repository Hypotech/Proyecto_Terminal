#include "Camara/Camara.hpp"
#include "Deteccion_Facial/RostrosDector.hpp"
#include <stdio.h>

int main(int argc, char *argv[])
{
    Camara CamLaptop;
    DectorRostros DectorCamLaptop;
    Mat UnFrame;
    Scalar colorAzul (255,0,0);
    int x = 1;
    char path[50];
    vector<Rect> MarcosRostros;

    vector<int> compresion_param;
    compresion_param.push_back(CV_IMWRITE_PXM_BINARY);
    compresion_param.push_back(1);

    while(CamLaptop.GetFrame(UnFrame) == true)
	{
	    DectorCamLaptop.EscanearImagen(UnFrame, MarcosRostros);

	    for (vector<Rect>::const_iterator Rostro = MarcosRostros.begin() ; Rostro != MarcosRostros.end() ; Rostro++){

		rectangle(UnFrame, *Rostro, colorAzul); //( Mat& input , Rect , const Scalar& color, int thickness, int line type, int shift ), se dibuja un rectangulo en la imagen original

		if(waitKey(15) == 't') 
		    {
			sprintf(path,"%d.pgm",x);

			Mat ImagenRostro;
			cvtColor(UnFrame(*Rostro), ImagenRostro,CV_BGR2GRAY);

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
