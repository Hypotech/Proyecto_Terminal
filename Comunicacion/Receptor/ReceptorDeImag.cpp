#include "ReceptorDeImag.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include "opencv2/core/core.hpp"
#include <arpa/inet.h>

ReceptorDeImagen::ReceptorDeImagen()
{
    DescripDeConex = socket (AF_INET, SOCK_STREAM, 0);
}

ReceptorDeImagen::ReceptorDeImagen(const char* const IP)//:InfoAddrEmisor{AF_INET, 2014, {inet_addr(IP)}}
{
    conectar(IP);
}

ReceptorDeImagen::~ReceptorDeImagen()
{
    close(DescripDeConex);
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
        if ( (bytes = recv(DescripDeConex, BufferDatos+i,PesoImagen-i,0)) == -1){
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
            ptr++; //ByN
        }
    }
    return true;
}

void ReceptorDeImagen::MandarApertura(int& ResulReconocimiento) const
{
    send(DescripDeConex, &ResulReconocimiento, sizeof(int),0);
}

void ReceptorDeImagen::cerrarConexion()
{
    close(DescripDeConex);
}

bool ReceptorDeImagen::conectar(const char* const IP)
{
    DescripDeConex = socket (AF_INET, SOCK_STREAM, 0);
    sockaddr_in InfoAddrEmisor = {AF_INET,2014,inet_addr(IP)}; //informacion acerca de la dir. de la raspberry

    if( connect(DescripDeConex, (struct sockaddr*)&InfoAddrEmisor, sizeof(InfoAddrEmisor))  == -1 ){
        cerrarConexion();
        return false;
    }
    else
        return true;
}
