#ifndef _CAMARA_HPP_
#define _CAMARA_HPP_

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;

class Camara
{
public:
    Camara(int IndiceCamara = 0);

    bool GetFrame(Mat& Almacen);

private:
    VideoCapture FlujoVideo;
};


#endif /* _CAMARA_HPP_ */
