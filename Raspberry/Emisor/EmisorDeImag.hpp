#ifndef _EMISORDEIMAG_HPP_
#define _EMISORDEIMAG_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include "opencv2/core/core.hpp"
#include <netinet/in.h>

class EmisorDeImagenes
{
public:
    EmisorDeImagenes(const char* const IP);
    ~EmisorDeImagenes();
    int enviar(cv::Mat& Img_a_Enviar) const; //envia la imagene que se le pase
    int GetRespuesta() const;

private:
    int DescripDeConex; // descriptor de conexion
    struct sockaddr_in infoAddrReceptor; //Estructura con la informacion acerca del tipo de direccion
    void retardo(int segundos) const;
};


#endif /* _EMISORDEIMAG_HPP_ */
