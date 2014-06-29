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
    ~ReceptorDeImagen();
    bool Recibir(cv::Mat &);
    int EsperarConexion();
    void MandarApertura(int& ResulReconocimiento) const;
    void cerrarConexion();

private:
    int DescriptorSocket;
    int  DescripDeConexEmisor;
    const struct sockaddr_in InfoAddrReceptor;
    struct sockaddr_in InfoAddrEmisor;
    unsigned int TamInfoAddrEmisor;
};


#endif /* _RECEPTORDEIMAG_HPP_ */
