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
DectorRostros(float escl = 1.1, double min = 0.16, double max = 0.8, int vecinos = 2);

    bool EscanearImagen(const Mat &ImagenEntrada, vector<Rect>& UbicacionConRect);

private:
    CascadeClassifier Detector_en_Cascada;

    double Escala;
    double minimoTamanyo; //tamanyo minimo posible a detectar de los rostros en el video
    double maximoTamnyo; //tamnyo maximo posible a detectar de los rostros en el video
    int Vecinos;
};


#endif	/* _ROSTROSDECTOR_HPP_ */
