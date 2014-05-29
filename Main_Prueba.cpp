#include "Reconocimiento_Facial/ReconocerPersona.hpp"
#include "Deteccion_Facial/RostrosDector.hpp"
//#include "Camara/Camara.hpp" // para acceder a la camara que se tenga conectada al equipo
#include "Comunicacion/Receptor/ReceptorDeImag.hpp" //Para recibir imagenes de la raspberry
#include"cv.hpp"
#include <iostream>

#define ESCALA_TEXT 2 //Tamanyo o escala del texto
#define IP_SERVIDOR "192.168.1.116"
#define AZUL 255,0,0
#define VERDE 0,255,0
#define ROJO 0,0,255

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    ReceptorDeImagen RaspiCam(IP_SERVIDOR);
    DectorRostros DectorRaspiCam;
    ReconocerdordePersona ReconocedorRaspiCam(atoi(argv[3]),atof(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));

    Mat ImgRecibida;
    vector<Rect> MarcosRostros;
    Scalar colorTexto(AZUL); //representa el color azul en BGR

    if ( DectorRaspiCam.clasificadorCargado() == false )
	{
	    cout<< "**Error: Falta el archivo o archivo danyado haarcascade.xml\nSaliendo de la aplicacion..."<<endl;
	    return -1;
	}

    if( RaspiCam.EsperarConexion() < 0) //esperamos a que la raspberry se conecte
	{
	    cout << "**Error al intentar esperar la conexion\nSaliendo de la aplicacion..." << endl;
	    return -2;
	}

    std::cout << "Conexion con la Rasberry... exitosa" << std::endl;

    while( RaspiCam.Recibir(ImgRecibida) == true) //mientras no haya error en la recepcion de datos
	{
	    DectorRaspiCam.EscanearImagen(ImgRecibida, MarcosRostros);
	   
	    for (vector<Rect>::const_iterator Rostro = MarcosRostros.begin() ; Rostro != MarcosRostros.end() ; Rostro++){

		Mat ImagenRostro;

		rectangle(ImgRecibida, *Rostro,colorTexto ); //( Mat& input , Rect , const Scalar& color, int thickness, int line type, int shift ), se dibuja un rectangulo que enmarca el rostro
		ImagenRostro = ImgRecibida(*Rostro); //recortamos solo el rostro y lo almacenamos
		//cvtColor(ImgRecibida(*Rostro), ImagenRostro,CV_BGR2GRAY);
		Mat RostroNormalizado(atoi(argv[2]), atoi(argv[1]) ,ImagenRostro.type() ); //aqui se almacenara la imagen ya normalizada en tamanyo

		resize(ImagenRostro, RostroNormalizado, RostroNormalizado.size(), 0, 0, INTER_LINEAR); //normalizamos la imagen en tamanyo
		int IDpersona = ReconocedorRaspiCam.consulatarBD(RostroNormalizado);

		Point P(Rostro->x, Rostro->y); //obtenemos la coordenada inicial del rectangulo que enmarca el rostro (esquina superior izquierda)

		if(IDpersona == -1){
		    putText(ImgRecibida,"Rostro desconocido" ,P ,FONT_HERSHEY_PLAIN, ESCALA_TEXT, colorTexto);
		    cout<< "Rostro desconocido"<<endl;
		}
		else if(IDpersona == 1){
		    putText(ImgRecibida, "Christian",P ,FONT_HERSHEY_PLAIN, ESCALA_TEXT, colorTexto);
		    cout<< "Rostro encontrado: Persona con ID " << IDpersona << endl;
		}
		else{
		    putText(ImgRecibida,format("Subjeto ID %d", IDpersona),P ,FONT_HERSHEY_PLAIN, ESCALA_TEXT, colorTexto);
		    cout<< "Rostro encontrado: Persona con ID " << IDpersona << endl;
		}
	    }  

	    imshow("Prueba",ImgRecibida);
	
	    if(waitKey(1) == 27) break; //esperar 1ms para pulsar la tecla ESC para salir
	}
    return 0;
}
