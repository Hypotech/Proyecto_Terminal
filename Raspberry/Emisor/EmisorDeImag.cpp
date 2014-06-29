#include "EmisorDeImag.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include "opencv2/core/core.hpp"
#include <arpa/inet.h>
#include <unistd.h>

EmisorDeImagenes::EmisorDeImagenes(const char* const IP): infoAddrReceptor{AF_INET, 2014, {inet_addr(IP)} }
{
    int x = 0;
    while(true){
	DescripDeConex = socket (AF_INET, SOCK_STREAM, 0);
	if( connect(DescripDeConex, (struct sockaddr *) &infoAddrReceptor, sizeof(infoAddrReceptor)) != -1 )
	    break;
        std::cout << "No se pudo conectar con el servidor, intentando de nuevo dentro de ";
	close(DescripDeConex);

	if(x < 5){
	    retardo(30); //30 seg
	    std::cout << "30 seg..." << std::endl;
	}
	else if(x < 10){
	    retardo(5*60); // 5 min
	    std::cout << "5 min..." << std::endl;
	}
	else if(x < 12){
            retardo(10*60); // 10 min
	    std::cout << "10 min.." << std::endl;
	}
	else
            x=0;

        x++;
    }
}

EmisorDeImagenes::~EmisorDeImagenes()
{
    close(DescripDeConex);
}

int EmisorDeImagenes::enviar(cv::Mat& Img_a_Enviar) const
{
    Img_a_Enviar = (Img_a_Enviar.reshape(0,1));
    size_t PesoImagen = Img_a_Enviar.total() * Img_a_Enviar.elemSize();

    return send( DescripDeConex, Img_a_Enviar.data, PesoImagen,0);
}

int EmisorDeImagenes::GetRespuesta() const
{
    int resultado;
    recv(DescripDeConex,&resultado,sizeof(int),0);
    return resultado;
}

void EmisorDeImagenes::retardo(int segundos) const
{
    for (int i = 0; i < segundos; i++) {
            usleep(1000000);
    }

}
