#include "EmisorDeImag.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include "opencv2/core/core.hpp"
#include <arpa/inet.h>
#include <unistd.h>

EmisorDeImagenes::EmisorDeImagenes(): infoAddrEmisor{AF_INET, 2014, {htonl(INADDR_ANY)} }
{
    DescriptorSocket = socket(AF_INET, SOCK_STREAM, 0);

    if ( bind(DescriptorSocket,(struct sockaddr *) &infoAddrEmisor, sizeof(infoAddrEmisor)) == -1 ) //asigna la direccion al socket
	std::cout<< "No se pudo asignar la direccion" << std::endl;
    if( listen(DescriptorSocket,1) == -1) // intenta habilitar para recepcion de conexs. con un cola
	std::cout << "No se pudo recibir peticiones de conexion. Fallo lamada listen" << std::endl;
}

EmisorDeImagenes::~EmisorDeImagenes()
{
    close(DescripDeConexEmisor);
    close(DescriptorSocket);
}

int EmisorDeImagenes::EsperarConexion()
{
    struct sockaddr_in infoAddrReceptor; //info. sobre la dirección de quien haga la peticion ( el receptor de la imagenes, tambien llamado computadora-Servidor)
    unsigned int TamInfoAddrReceptor = sizeof(infoAddrReceptor);

   DescripDeConexEmisor = accept(DescriptorSocket,(struct sockaddr *) &infoAddrReceptor, &TamInfoAddrReceptor);
    return DescripDeConexEmisor;
}

int EmisorDeImagenes::enviar(cv::Mat& Img_a_Enviar) const
{
    Img_a_Enviar = (Img_a_Enviar.reshape(0,1));
    size_t PesoImagen = Img_a_Enviar.total() * Img_a_Enviar.elemSize();

    return send( DescripDeConexEmisor, Img_a_Enviar.data, PesoImagen,0);
}

int EmisorDeImagenes::GetRespuesta() const
{
    int resultado;
    recv(DescripDeConexEmisor,&resultado,sizeof(int),0);
    return resultado;
}

//void EmisorDeImagenes::retardo(int segundos) const
//{
//    for (int i = 0; i < segundos; i++) {
//            usleep(1000000);
//    }

//}
