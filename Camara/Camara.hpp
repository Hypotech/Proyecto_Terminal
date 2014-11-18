#ifndef _CAMARA_HPP_
#define _CAMARA_HPP_

//#include "cv.hpp"
//#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;

class Camara
{
public:
    Camara(int IndiceCamara = 0);
//    ~Camara();

    bool GetFrame(Mat&);

private:
    VideoCapture FlujoVideo;

    int id;
};


#endif /* _CAMARA_HPP_ */
