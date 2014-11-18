#include "ojosdetector.h"

ojosDetector::ojosDetector()
{
    Detector_en_Cascada.load("haarcascade_eyes.xml");
}

void ojosDetector::EscanearImagen(const cv::Mat &ImagenEntrada, std::vector<cv::Rect> &UbicacionRostros)
{
    cv::Mat ImagenEqual;

//    cvtColor(ImagenEntrada,ImagenEqual,CV_BGR2GRAY);

    cv::equalizeHist(ImagenEntrada,ImagenEntrada);

    Detector_en_Cascada.detectMultiScale(ImagenEntrada, UbicacionRostros, 1.1, 4,
                                         0| CV_HAAR_DO_CANNY_PRUNING,cv::Size(90,90));
}
