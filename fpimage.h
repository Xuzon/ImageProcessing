#ifndef FPIMAGE_H
#define FPIMAGE_H

//OCV #include <cv.h>
//OCV #include <cxcore.h>
//OCV #include <highgui.h>
//OCV #include <ml.h>

#include <QMainWindow>
#include "ImageProcessor.h"

namespace Ui {
class FPImage;
}

class FPImage : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit FPImage(QWidget *parent = 0);
    ~FPImage();
    
private:
    Ui::FPImage *ui;

    QString Path;               // Para recordar la carpeta al cargar imágenes
//OCV     cv::Mat Ima;                // La imagen OpenCV (encapsula la imagen Qt)
    QImage Image;               // Imagen Qt
    uchar *pixR, *pixG, *pixB;  // Punteros a los tres canales R, G y B
    int W, H;                   // Tamaño de la imagen actual
    int S, Padding;                // Step en imágenes no continuas

    QPixmap Dib1, Dib2, Dib3;   // Tres lienzos en los que dibujar

    //for transference function
    QPixmap transference;
    int transferenceXLastClicked;
    void ShowIt(void);          // Muestra la imagen actual

    bool eventFilter(QObject *Ob, QEvent *Ev);  // Un "filtro de eventos"

    ImageProcessor::EdgeMetric edgeMetric = ImageProcessor::EdgeMetric::AbsDifference;

    ImageProcessor* processor;

    int rHistogram[256];
    int gHistogram[256];
    int bHistogram[256];

    int maxValue;
    int rRawHistogram[256];
    int gRawHistogram[256];
    int bRawHistogram[256];

    void DrawTransferenceFunction();

    void ChangeUserLUT(int x, int y);

private slots:
    void Load(void);    // Slot para el botón de carga de imagen
    void DoIt(void);    // Slot para el botón de hacer algo con la imagen
    void ChangeBrightness(int value);
    void ChangeContrast(int value);
    void ChangeEdge(int value);
    void ChangedEdgeMethod();
    void RandomDithering(int value);
    void DrawHistograms();

};

#endif // FPIMAGE_H
