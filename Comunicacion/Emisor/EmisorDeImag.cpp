#include "EmisorDeImag.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include "opencv2/core/core.hpp"
#include <arpa/inet.h>

EmisorDeImagenes::EmisorDeImagenes(const char* const IP): infoAddrReceptor{AF_INET, 2014, {inet_addr(IP)} }
{
    DescripDeConex = socket (AF_INET, SOCK_STREAM, 0);
    if( connect(DescripDeConex, (struct sockaddr *) &infoAddrReceptor, sizeof(infoAddrReceptor)) == -1 )
	std::cout << "No se pudo conectar con el servidor, no se enviaran las imagenes" << std::endl;
}

EmisorDeImagenes::~EmisorDeImagenes()
{
    close(DescripDeConex);
}

bool EmisorDeImagenes::enviar(cv::Mat& Img_a_Enviar) const
{
    Img_a_Enviar = (Img_a_Enviar.reshape(0,1));
    size_t PesoImagen = Img_a_Enviar.total() * Img_a_Enviar.elemSize();

    send( DescripDeConex, Img_a_Enviar.data, PesoImagen,0);
}
