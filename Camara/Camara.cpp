#include "Camara.hpp"

Camara::Camara(int IndiceCamara): FlujoVideo(IndiceCamara)
{

}

 Camara::~Camara()
{
    FlujoVideo.release();
}

bool Camara::GetFrame(Mat& Almacen)
{
    return FlujoVideo.read(Almacen);
}
