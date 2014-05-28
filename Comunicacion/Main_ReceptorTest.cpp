#include "Receptor/ReceptorDeImag.hpp"
#define IP_SERVIDOR "192.168.1.116"
#include "opencv2/highgui/highgui.hpp"

int main(int argc, char *argv[])
{
    ReceptorDeImagen RaspiCam(IP_SERVIDOR);
    cv::Mat ImgRecibida;

    RaspiCam.EsperarConexion();

    while (true) {
	RaspiCam.Recibir(ImgRecibida);
	cv::imshow("Imagen Recibida",ImgRecibida);
	if ( cv::waitKey(1) == 27) break;
    }

    return 0;
}
