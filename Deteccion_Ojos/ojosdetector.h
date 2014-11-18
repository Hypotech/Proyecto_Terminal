#ifndef OJOSDETECTOR_H
#define OJOSDETECTOR_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include "opencv2/core/core.hpp"

class ojosDetector
{
public:
    ojosDetector();
    void EscanearImagen(const cv::Mat &ImagenEntrada, std::vector<cv::Rect>& UbicacionRostros);
    bool clasificadorCargado();

private:
    cv::CascadeClassifier Detector_en_Cascada;
};


#endif // OJOSDETECTOR_H
