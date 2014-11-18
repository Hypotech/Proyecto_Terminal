#ifndef _RECEPTORDEIMAG_HPP_
#define _RECEPTORDEIMAG_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include "opencv2/core/core.hpp"
#include <netinet/in.h>
#define SI 100
#define NO 1

class ReceptorDeImagen
{
public:
    ReceptorDeImagen();
    ReceptorDeImagen(const char* const IP);
    ~ReceptorDeImagen();
    bool Recibir(cv::Mat &);
    void MandarApertura(int& ResulReconocimiento) const;
    void cerrarConexion();
    bool conectar(const char* const IP); //conectar con la IP especificada

private:
    int DescripDeConex;  // descriptor de conexion
    struct sockaddr_in InfoAddrEmisor;
};


#endif /* _RECEPTORDEIMAG_HPP_ */
