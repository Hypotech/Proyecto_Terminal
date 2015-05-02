#ifndef _ROSTROSDECTOR_HPP_
#define _ROSTROSDECTOR_HPP_

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include "opencv2/core/core.hpp"

using namespace cv;

class DectorRostros
{
public:
    DectorRostros();//float escl = 1.1, double min = 50, double max = 50, int vecinos = 4);

    void EscanearImagen(const Mat &ImagenEntrada, vector<Rect>& UbicacionConRect,const int minEscala);
    bool clasificadorCargado();
private:
    CascadeClassifier Detector_en_Cascada;

};


#endif	/* _ROSTROSDECTOR_HPP_ */
