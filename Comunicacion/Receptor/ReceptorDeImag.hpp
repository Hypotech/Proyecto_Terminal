#ifndef _RECEPTORDEIMAG_HPP_
#define _RECEPTORDEIMAG_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include "opencv2/core/core.hpp"
#include <netinet/in.h>

class ReceptorDeImagen
{
public:
    ReceptorDeImagen(const char* const IP);
    ~ReceptorDeImagen();
    void Recibir(cv::Mat &);
    int EsperarConexion();
private:
    int DescripDeConex;
    int  DescripDeConexEmisor;
    const struct sockaddr_in InfoAddrReceptor;
    struct sockaddr_in InfoAddrEmisor;
    unsigned int TamInfoAddrEmisor;
};


#endif /* _RECEPTORDEIMAG_HPP_ */
