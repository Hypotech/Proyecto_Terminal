#ifndef _RECONOCERPERSONA_HPP_
#define _RECONOCERPERSONA_HPP_

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"

#define FILE_CSV "./index.csv"

using namespace cv;
using namespace std;
class ReconocerdordePersona
{
public:
    ReconocerdordePersona(int Neigen = 8, double ranConfia = 0, int radio = 1,int regX = 8, int regY = 8);

    int consulatarBD(Mat& ImagenRostro) const;


private:

    Ptr<FaceRecognizer> Reconocedor;
    // Vector <Mat> imagenes;
    // Vector<int> etiquetas;
    string Ruta_a_CSV;

    void obtenerParametrosCSV(const string&, vector<Mat>& , vector <int>& );
};


#endif /* _RECONOCERPERSONA_HPP_ */
