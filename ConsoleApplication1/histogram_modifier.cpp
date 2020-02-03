
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    //Creamos las variables necesarias
    Mat src, histogramResult, imageEqual;

    // Se carga la imagen desde la carpeta donde se encuentra el programa
    src = imread("../../histogram_modifier/histogram_test.jpg", IMREAD_GRAYSCALE);
    //Se observa si  la imagen existe, si no, no se realiza nada
    if (!src.data)
    {
        return -1;
    }

    // Establecemos el numero de bins
    int numberhist = 255;
    // Valores para el dibujo del histograma
    //Se establece el alto del histograma
    int altoHist = 256; 
    //Se estbalece el largo del histograma
    int anchoHist = 400;
    //Se establece el peso de los bins, en este caso es 1
    int pesohist = 1;

    //Seteamos los rangos de nuestro histograma
    float rango[] = { 0, 256 };
    const float* rangoHist = { rango };
    //Seteamos los rangos de iamgen recuperada, con relacion al histograma. Se basan en el largo y alto
    //ya puestos, anteriormente
    float secondRango[] = { 160, 400 };
    const float* rangoEqual = { secondRango };

    //Se caclcula el histograma
    calcHist(&src, 1, 0, Mat(), histogramResult, 1, &numberhist, &rangoHist, true, false);
    //Se creaca una imagen con los cosntructores de:
    //Rows:400
    //cols:256
    //Type: tipo de imagen en este caso es una imagen de un solo canal, por lo tanto se pone 
    //CV_80C1: que significa imagen de 8 bits de un canal
    //Scalar: un escalar en este caso 0
    Mat graphicHist(anchoHist, altoHist, CV_8UC1, Scalar(0, 0, 0));

    ///Se normalizan los resultados, de el valor maximo que pusimos en este caso 400, a un valor normal 
    //de 256, esto incrementala informacion de nuestro algoritmo
    normalize(histogramResult, histogramResult, 0, graphicHist.rows, NORM_MINMAX, -1, Mat());

    //Se dibuja una linea siguiendo los datos de nuestro algoritmo, para crear nuestra grafica
    for (int i = 1; i < numberhist; i++)
    {
        line(graphicHist,
            Point(pesohist * (i - 1), 
            anchoHist - cvRound(histogramResult.at<float>(i - 1))),
            Point(pesohist * (i), 
            anchoHist - cvRound(histogramResult.at<float>(i))),
            Scalar(255, 0, 0), 2, 8, 0);

    }
    //Calculamos en base anuestro histogram y el rango que deseamos la nueva imagen
    calcBackProject(&src, 1, 0, histogramResult, imageEqual, &rangoEqual, 1, true);
    //Agreamos un texto a la imagen 
    putText(imageEqual, "Rango de 160 a 400", Point(50,100), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
    putText(src, "Dos piezas de lego naranja y amarilla", Point(50, 100), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
    //Mostramos las iamgenes
    namedWindow("calcHist Demo", WINDOW_AUTOSIZE);
    imshow("calcHist Demo", graphicHist);
    imshow("Redraw", imageEqual);
    imshow(" Demo", src);

    while (char(waitKey(1)) != 27) {}
    return 0;
}