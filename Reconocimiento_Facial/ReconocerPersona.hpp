#ifndef _RECONOCERPERSONA_HPP_
#define _RECONOCERPERSONA_HPP_

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"

#define FILE_CSV "./index.csv"

enum metodo {eigen,fisher,LBPH};

using namespace cv;
using namespace std;
class ReconocerdordePersona
{
public:
    ReconocerdordePersona(int Neigen = 8, double ranConfia = 100, int radio = 1,
                          int regX = 8, int regY = 8, int vecinos = , metodo modelo = LBPH);

    int consulatarBD(Mat& ImagenRostro) const;
    int consulatarBD(Mat& ImagenRostro, double& confianza) const;

private:
    Ptr<FaceRecognizer> Reconocedor;
    string Ruta_a_CSV;

    void obtenerParametrosCSV(const string&, vector<Mat>& , vector <int>& );
    void cambiarModelo(int Neigen, double ranConfia, int radio, int regX,
                       int regY, int vecinos, metodo modelo);
};


#endif /* _RECONOCERPERSONA_HPP_ */
