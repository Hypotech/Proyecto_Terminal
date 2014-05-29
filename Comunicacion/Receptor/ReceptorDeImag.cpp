#include "ReceptorDeImag.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include "opencv2/core/core.hpp"
#include <arpa/inet.h>

ReceptorDeImagen::ReceptorDeImagen(const char* const IP):InfoAddrReceptor{AF_INET, 2014, {inet_addr(IP)}},
TamInfoAddrEmisor( sizeof(InfoAddrEmisor)) 
{
    DescripDeConex = socket(AF_INET, SOCK_STREAM, 0);

    if ( bind(DescripDeConex,(struct sockaddr *) &InfoAddrReceptor, sizeof(InfoAddrReceptor)) == -1 ) // intentando asignar la direccion
	std::cout<< "No se pudo asignar la direccion dada: " << IP << std::endl;
    if( listen(DescripDeConex,1) == -1) // intenta habilitar para recepcion de conexs. con un cola
	std::cout << "No se pudo recibir peticiones de conexion. Fallo lamada listen" << std::endl;
}

ReceptorDeImagen::~ReceptorDeImagen()
{
    close(DescripDeConexEmisor);
    close(DescripDeConex);
}

int ReceptorDeImagen::EsperarConexion()
{
    DescripDeConexEmisor = accept(DescripDeConex,(struct sockaddr *) &InfoAddrEmisor, &TamInfoAddrEmisor);
    return DescripDeConexEmisor;
}

bool ReceptorDeImagen::Recibir(cv::Mat& ImgRecibida)
{
    // ImgRecibida =cv::Mat::zeros(480,640, CV_8UC3); //color
    ImgRecibida =cv::Mat::zeros(480,640, CV_8UC1); //ByN
    size_t PesoImagen = ImgRecibida.total() * ImgRecibida.elemSize();
    uchar BufferDatos[PesoImagen];

    ssize_t bytes = 0;
    int i;
    for (i = 0; i < PesoImagen; i+=bytes) {
	if ( (bytes = recv(DescripDeConexEmisor, BufferDatos+i,PesoImagen-i,0)) == -1){
	    std::cout << "Error: Fallo llamada recv al intentar recibir Datos" << std::endl;
	    return false;
	}
      }

    // std::cout << "Total de bytes recibidos: " <<  i << std::endl;


    int ptr = 0;
    for (int i = 0; i < ImgRecibida.rows; i++) {
	for (int j = 0; j < ImgRecibida.cols; j++) {
	    ImgRecibida.at<uchar>(i,j) = BufferDatos[ptr];
	    // ImgRecibida.at<cv::Vec3b>(i,j) = cv::Vec3b(BufferDatos[ptr+0], BufferDatos[ptr+1], BufferDatos[ptr+2]); //color
	    // ptr = ptr+3; //color
	    ptr++;
	}

    }
    return true;
}
