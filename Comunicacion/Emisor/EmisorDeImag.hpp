#ifndef _EMISORDEIMAG_HPP_
#define _EMISORDEIMAG_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include "opencv2/core/core.hpp"
#include <netinet/in.h>

class EmisorDeImagenes
{
public:
    EmisorDeImagenes();
    ~EmisorDeImagenes();
    int enviar(cv::Mat& Img_a_Enviar) const; //envia la imagen contenida en Img_a_Enviar
    int GetRespuesta() const;
    int EsperarConexion();

private:
    int DescriptorSocket;
    int  DescripDeConexEmisor;

    const struct sockaddr_in infoAddrEmisor; //informacion acerca de la direccion la raspberry
//    void retardo(int segundos) const;
};


#endif /* _EMISORDEIMAG_HPP_ */
