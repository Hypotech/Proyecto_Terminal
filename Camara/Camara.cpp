#include "Camara.hpp"

Camara::Camara(int IndiceCamara): FlujoVideo(IndiceCamara)
{

}

bool Camara::GetFrame(Mat& Almacen)
{
    return FlujoVideo.read(Almacen);
}
