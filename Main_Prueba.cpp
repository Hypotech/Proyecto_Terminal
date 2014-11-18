#include "Reconocimiento_Facial/ReconocerPersona.hpp"
#include "Deteccion_Facial/RostrosDector.hpp"
//#include "Camara/Camara.hpp" // camara usb
#include "Comunicacion/Receptor/ReceptorDeImag.hpp" //raspicam remota
#include"cv.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>

#define ESCALA_TEXT 2 //Tamanyo o escala del texto
#define AZUL 255,0,0
#define VERDE 0,255,0
#define ROJO 0,0,255

using namespace cv;
using namespace std;

bool LeerArchivoConfig(int& ,int& ,int& ,float& ,int& ,int& ,int&);

int main(int argc, char *argv[])
{
    int anchImag, altImag, Neigen; float rangoConfianza; int radio, regX, regY;
    if( LeerArchivoConfig(anchImag,altImag,Neigen,rangoConfianza,radio,regX,regY) == false){
	cout << "**Error: Archivo Parametros.conf no encontrado\nSaliendo de aplicacion..." << endl;
	return -2;
    }

    ReceptorDeImagen Servidor;
    DectorRostros DectorServidor;
    ReconocerdordePersona ReconocedorServidor(Neigen,rangoConfianza,radio,regX,regY,metodo modelo = LBPH);
    // ReconocerdordePersona ReconocedorServidor(atoi(argv[3]),atof(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));

    Mat ImgRecibida;
    vector<Rect> MarcosRostros;
    Scalar colorTexto(AZUL); //representa el color azul en BGR
    int Respuesta= -9;

    if ( DectorServidor.clasificadorCargado() == false )
	{
	    cout<< "**Error: Falta el archivo o archivo danyado haarcascade.xml\nSaliendo de la aplicacion..."<<endl;
	    return -1;
	}

    if( Servidor.conectar() ) //esperamos a que la raspberry se conecte
	{
	    cout << "**Error al intentar esperar la conexion\nSaliendo de la aplicacion..." << endl;
	    return -2;
	}

    std::cout << "Conexion con la Rasberry... exitosa" << std::endl;

    while( true) //mientras no haya error en la recepcion de datos
	{
	    if(Servidor.Recibir(ImgRecibida) == true){
		DectorServidor.EscanearImagen(ImgRecibida, MarcosRostros);
		Respuesta = -9;

		for (vector<Rect>::const_iterator Rostro = MarcosRostros.begin() ; Rostro != MarcosRostros.end() ; Rostro++){

		    Mat ImagenRostro;

		    rectangle(ImgRecibida, *Rostro,colorTexto ); //( Mat& input , Rect , const Scalar& color, int thickness, int line type, int shift ), se dibuja un rectangulo que enmarca el rostro
		    ImagenRostro = ImgRecibida(*Rostro); //recortamos solo el rostro y lo almacenamos
		    //cvtColor(ImgRecibida(*Rostro), ImagenRostro,CV_BGR2GRAY);
		    Mat RostroNormalizado(anchImag, altImag ,ImagenRostro.type() ); //aqui se almacenara la imagen ya normalizada en tamanyo
		    // Mat RostroNormalizado(atoi(argv[2]), atoi(argv[1]) ,ImagenRostro.type() ); //aqui se almacenara la imagen ya normalizada en tamanyo
		    double confianza;
		    resize(ImagenRostro, RostroNormalizado, RostroNormalizado.size(), 0, 0, INTER_LINEAR); //normalizamos la imagen en tamanyo
		    int IDpersona = ReconocedorServidor.consulatarBD(RostroNormalizado,confianza);

		    Point P(Rostro->x, Rostro->y); //obtenemos la coordenada inicial del rectangulo que enmarca el rostro (esquina superior izquierda)

		    if(IDpersona == -1 || confianza < 150.0){
			putText(ImgRecibida,"Rostro desconocido" ,P ,FONT_HERSHEY_PLAIN, ESCALA_TEXT, colorTexto);
			Respuesta = NO;
			// cout<< "Rostro desconocido"<<endl;
		    }
		    else if(IDpersona == 1){
			putText(ImgRecibida, "Christian",P ,FONT_HERSHEY_PLAIN, ESCALA_TEXT, colorTexto);
			Respuesta = SI;
			cout<< "Rostro encontrado: Persona con ID " << IDpersona << endl;
		    }
		    else{
			putText(ImgRecibida,format("Subjeto ID %d", IDpersona),P ,FONT_HERSHEY_PLAIN, ESCALA_TEXT, colorTexto);
			Respuesta = SI;
			cout<< "Rostro encontrado: Persona con ID " << IDpersona << endl;
		    }
		}  
		Servidor.MandarApertura(Respuesta);
		imshow("Prueba",ImgRecibida);
	
		if(waitKey(1) == 27) break; //esperar 1ms para pulsar la tecla ESC para salir
	    }

	    else{
		std::cout << "Se cerro la comunicacion" << std::endl;
		Servidor.cerrarConexion();
		Servidor.conectar();
		continue;
	    }
	}
    return 0;
}

bool LeerArchivoConfig(int &anchImag,int  &altImag,int &Neigen,float &rangoConfianza,int  &radio,int  &regX,int &regY)
{
    string line;
    ifstream ArchivoDeConfig("Parametros.conf", ios::in);

    if ( ArchivoDeConfig.is_open() )
	{
	    getline (ArchivoDeConfig,line);
	    istringstream (line) >> anchImag;

	    getline (ArchivoDeConfig,line);
	    istringstream (line) >> altImag;

	    getline (ArchivoDeConfig,line);
	    istringstream (line) >> Neigen;

	    getline (ArchivoDeConfig,line);
	    istringstream (line) >> rangoConfianza;

	    getline (ArchivoDeConfig,line);
	    istringstream (line) >> radio;

	    getline (ArchivoDeConfig,line);
	    istringstream (line) >> regX;

	    getline (ArchivoDeConfig,line);
	    istringstream (line) >> regY;

	    ArchivoDeConfig.close();
	    return true;
	}

    return false;
}
