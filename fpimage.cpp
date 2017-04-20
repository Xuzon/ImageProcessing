#include "fpimage.h"
#include "ui_fpimage.h"

#include <QFileDialog>
#include <QPainter>

//OCV #include "opencv2/imgproc.hpp"

//OCV using namespace cv;

//--------------------------------------------------
//-- Filtro de eventos para capturar mouse clicks --
//--------------------------------------------------
bool FPImage::eventFilter(QObject *Ob, QEvent *Ev){
    // Comprobamos que el evento capturado es un  mouseclick
    if(Ev->type()==QEvent::MouseButtonPress) {
        // Comprobamos que el click ocurrió sobre nuestro QLabel
        if(Ob==ui->Ecran) {
            // Hacemos un cast del evento para poder acceder a sus propiedades
            const QMouseEvent *me=static_cast<const QMouseEvent *>(Ev);
            // Nos interesan las coordenadas del click
            int y=me->y(), x=me->x();
            // Si estamos fuera de la imagen, nos vamos
            if(y>=H||x>=W) return true;
            // Hacemos algo con las coordenadas y el píxel
            ui->ERes->appendPlainText("R: "+
                                      QString::number(pixR[(y*S+3*x)])+" G: "+
                                      QString::number(pixG[(y*S+3*x)])+" B: "+
                                      QString::number(pixB[(y*S+3*x)]));
                                      
            if (processor != nullptr) {
                ui->ERes->appendPlainText("L: " +
                    QString::number(this->processor->pixL[(y*S + 3 * x)]) + " H:" +
                    QString::number(this->processor->pixH[(y*S + 3 * x)]) + " S:" +
                    QString::number(this->processor->pixS[(y*S + 3 * x)]));
            }
            if (processor != nullptr) {
                //add to skin in LHS color spacethis->processor->pixS
                uchar r = pixR[(y*S + 3 * x)];
                uchar g = pixG[(y*S + 3 * x)];
                uchar b = pixB[(y*S + 3 * x)];
                ColorSpace::RGBtoLHS(&r,&g,&b);
                processor->faceDetector->AddPixel(this->processor->pixL[(y*S + 3 * x)], this->processor->pixH[(y*S + 3 * x)], this->processor->pixS[(y*S + 3 * x)]);
                AddPointToHistogram(pixR[(y*S + 3 * x)], pixG[(y*S + 3 * x)], pixB[(y*S + 3 * x)]);

               statusBar()->showMessage(
                    "Count:" + QString::number(processor->faceDetector->count)
                    +" SumR:" + QString::number(processor->faceDetector->sumR)
                    + " SumG:" + QString::number(processor->faceDetector->sumG)
                    + " SumB:" + QString::number(processor->faceDetector->sumB)
                    +" MedR:" + QString::number(processor->faceDetector->average.x)
                    + " MedG:" + QString::number(processor->faceDetector->average.y)
                    + " MedB:" + QString::number(processor->faceDetector->average.z)
                    + " DesvR:" + QString::number(processor->faceDetector->typicalDesviation.x)
                    + " DesvG:" + QString::number(processor->faceDetector->typicalDesviation.y)
                    + " DesvB:" + QString::number(processor->faceDetector->typicalDesviation.z)
                );
            }
            // Devolvemos un "true" que significa que hemos gestionado el evento
            return true;
        } else if(Ob == ui->TransferenceFunction){
            // Hacemos un cast del evento para poder acceder a sus propiedades
            const QMouseEvent *me = static_cast<const QMouseEvent *>(Ev);
            // Nos interesan las coordenadas del click
            int y = me->y(), x = me->x();
            this->ChangeUserLUT(x, y);
            this->DrawTransferenceFunction();
            return true;
        }else return false;  // No era para nosotros, lo dejamos en paz
        
    } else if (Ev->type() == QEvent::MouseMove) {
        if (Ob == ui->TransferenceFunction) {
            // Hacemos un cast del evento para poder acceder a sus propiedades
            const QMouseEvent *me = static_cast<const QMouseEvent *>(Ev);
            // Nos interesan las coordenadas del click
            int y = me->y();
            this->ChangeUserLUT(transferenceXLastClicked, y);
            this->DrawTransferenceFunction();
            return true;
        }
    }else return false;
}

//-------------------------------------------------
//-- Constructor: Conexiones e inicializaciones ---
//-------------------------------------------------
FPImage::FPImage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FPImage){

    ui->setupUi(this);

    // CONEXIONES de nuestros objetos (botones, etc...) a nuestros slots
    connect(ui->BLoad,SIGNAL(clicked()),this,SLOT(Load()));
    connect(ui->BDoIt,SIGNAL(clicked()),this,SLOT(DoIt()));
    connect(ui->SliderBrightness, SIGNAL(valueChanged(int)), this, SLOT(ChangeBrightness(int)));
    connect(ui->SliderContrast, SIGNAL(valueChanged(int)), this, SLOT(ChangeContrast(int)));
    connect(ui->SliderEdges, SIGNAL(valueChanged(int)), this, SLOT(ChangeEdge(int)));
    connect(ui->AbsDifferenceRButton, SIGNAL(clicked()), this, SLOT(ChangedEdgeMethod()));
    connect(ui->MaxDifferenceRButton, SIGNAL(clicked()), this, SLOT(ChangedEdgeMethod()));
    connect(ui->EuclideanRButton, SIGNAL(clicked()), this, SLOT(ChangedEdgeMethod()));

    connect(ui->CheckRandomDithering, SIGNAL(stateChanged(int)), this, SLOT(RandomDithering(int)));

    connect(ui->BHistogram, SIGNAL(clicked()), this, SLOT(DrawHistograms()));

    connect(ui->SliderRDesv, SIGNAL(valueChanged(int)), this, SLOT(SkinChange(int)));
    connect(ui->SliderGDesv, SIGNAL(valueChanged(int)), this, SLOT(SkinChange(int)));
    connect(ui->SliderBDesv, SIGNAL(valueChanged(int)), this, SLOT(SkinChange(int)));

    connect(ui->BBlurInside, SIGNAL(clicked()), this, SLOT(BlurInside()));

    connect(ui->BDetectSkin, SIGNAL(clicked()), this, SLOT(CallSkinChange()));
    connect(ui->BErode, SIGNAL(clicked()), this, SLOT(Erode()));
    connect(ui->BDilate, SIGNAL(clicked()), this, SLOT(Dilate()));


    // "Instalamos" un "filtro de eventos" en nuestro QLabel Ecran
    // para capturar clicks de ratón sobre la imagen
    ui->Ecran->installEventFilter(this);
    ui->TransferenceFunction->installEventFilter(this);
    transference = QPixmap(256, 256);
    transference.fill(Qt::black);
    ui->TransferenceFunction->setPixmap(transference);


    // INICIALIZACIONES
    W=H=0;      // Empezamos sin imagen cargada
    Path="..";  // Carpeta inicial
    this->processor = NULL;

    // Inicializamos a negro los lienzos (QPixmap) y los asociamos a las "pantallas" gráficas (QLabel)
    //   Le damos tamaño
    Dib1=QPixmap(256,100);
    //   Lo pintamos de negro
    Dib1.fill(Qt::black);
    //   Lo asignamos a un QLabel
    ui->EcranHistoR->setPixmap(Dib1);

    //   Idem
    Dib2=QPixmap(256,100);
    Dib2.fill(Qt::black);
    ui->EcranHistoG->setPixmap(Dib2);

    // De ídem
    Dib3=QPixmap(256,100);
    Dib3.fill(Qt::black);

/*
    // Ejemplo de cómo dibujar usando funciones de alto nivel (QPainter)
    //   Declaramos un pintor (QPainter) y lo asociamos a un lienzo (QPixmap)
    QPainter p(&Dib3);
    //   Escogemos un lápiz (QPen; también hay pinceles, QBrush, para los rellenos)
    p.setPen(QPen(Qt::yellow));
    //   Trazamos un par de líneas, por ejemplo
    p.drawLine(0,0,255,99);
    p.drawLine(0,99,255,0);
*/

    ui->EcranHistoB->setPixmap(Dib3);
}

//-------------------------------------------------
//------ Destructor: Limpieza antes de salir ------
//-------------------------------------------------
FPImage::~FPImage()
{
    delete ui;
    delete processor;
}

//-------------------------------------------------
//----------- Carga una imagen de disco -----------
//-------------------------------------------------
void FPImage::Load(void){
    // Permite al usuario escoger un fichero de imagen
    QString file=QFileDialog::getOpenFileName(this,tr("Abrir imagen"),Path,tr("Image Files (*.png *.jpg *.bmp)"));
    // Si no escogió nada, nos vamos
    if(file.isEmpty()) return;

    // Creamos un QFileInfo para manipular cómodamente el nombre del fichero a efectos informativos
    // Por ejemplo deshacernos del path para que el nombre no ocupe demasiado
    QFileInfo finfo(file);
    // Memorizamos la carpeta usando la variable global Path, para la próxima vez
    Path=finfo.path();
    // Ponemos el nombre del fichero en el recuadro de texto
    ui->EFile->setText(finfo.fileName());
    // Decoración: Añadimos el nombre del fichero al título de la ventana
    setWindowTitle("FPImage v0.1b - "+finfo.fileName());

    // Cargamos la imagen a nuestra variable "Image" usando la función apropiada de la clase QImage
    Image.load(file);
    // Convertimos a RGB (eliminamos el canal A)
    Image=Image.convertToFormat(QImage::Format_RGB888);

    // Almacenamos las dimensiones de la imagen
    W=Image.width();
    H=Image.height();

    // Ponemos nuestros punteros apuntando a cada canal del primer píxel
    pixB=(pixG=(pixR=Image.bits())+1)+1;

    // Ojo! La imagen puede llevar "relleno" (zero padding)
    // Longitud en bytes de cada línea incluyendo el padding
    S=3*W+(4-(3*W)%4)%4;
    // Padding
    Padding=S-3*W;

    if (this->processor != nullptr) {
        delete this->processor;
    }
    this->processor = new ImageProcessor(pixR, pixG, pixB, W, H, Padding, S);
    this->DrawTransferenceFunction();
    // Creamos una Mat de OpenCV (Ima) que "encapsula" los pixels de la QImage Image
//OCV     Ima=Mat(H,W,CV_8UC3,pixR,S);

    // Mostramos algo de texto informativo
    ui->ERes->appendPlainText("Loaded "+finfo.fileName());
    ui->ERes->appendPlainText("Size "+QString::number(W)+"x"+QString::number(H));
    ui->ERes->appendPlainText("Padded length "+QString::number(S));
    ui->ERes->appendPlainText("Pad "+QString::number(Padding));
    ui->ERes->appendPlainText("");

    // Ponemos algo en la barra de estado
    statusBar()->showMessage("Loaded.");

    // Ajustamos el tamaño de la "pantalla" al de la imagen
    ui->Ecran->setFixedWidth(W);
    ui->Ecran->setFixedHeight(H);

    // Volcamos la imagen a pantalla
    ShowIt();
}

//-------------------------------------------------
//------------- Jugamos con la imagen -------------
//-------------------------------------------------
void FPImage::DoIt(void){
    // Nos aseguramos de que hay una imagen cargada
    if(!H) return;

    // Ejemplo de procesamiento A BAJO NIVEL
    //   Recorremos toda la imagen manipulando los píxeles uno a uno
    //   Atención a los límites y a los saltos de 3 en 3 (3 canales)
    /*for(int y=0,i=0;y<H;y++,i+=Padding)
        for(int x=0;x<W;x++,i+=3) {
            //Swap G and R pixel colors
            uchar swap=pixG[i]; pixG[i]=pixR[i]; pixR[i]=swap;
            //Invert B pixel color
            pixB[i]=255-pixB[i];
    }*/
    //processor->UpsideDown();
    //processor->Invert();
    //processor->GrayScale();
    //processor->SepiaFilter();
    //processor->SepiaFilter(65,0,0,15);
    //processor->Dithering(ui->CheckRandomDithering->isChecked());
    //processor->LinearStretch(rHistogram, gHistogram, bHistogram);
    //processor->HistogramEqualization(rRawHistogram, gRawHistogram, bRawHistogram, W * H);
    //processor->ApplyHistogramEqualization();
    //processor->AdapativeHistogramEqualization(this->ui->SliderAdaptEq->value());
    //this->DrawHistograms();
    //this->DrawTransferenceFunction();
    
    //this->SkinChange(0);
    this->processor->Blur(this->ui->SliderKernel->value());
    //this->processor->Sharpening(this->ui->SliderKernel->value());
    //this->processor->BlurInsideObjects(this->ui->SliderKernel->value(), this->ui->SliderEdges->value(), edgeMetric);

    // Ejemplo de procesamiento CON OpenCV
//OCV     Mat radio5(11,11,CV_8U,Scalar(0));
//OCV     circle(radio5,Point(5,5),5,Scalar(1),-1);
//OCV     erode(Ima,Ima,radio5);

    // Sacamos algo de texto informativo
    ui->ERes->appendPlainText("Did it");

    // Ponemos algo en la barra de estado
    statusBar()->showMessage("Did it.");

    // Volcamos la imagen a pantalla
    // OJO: Si os olvidáis de esto, la imagen en pantalla no refleja los cambios y
    // pensaréis que no habéis hecho nada, pero Image e Ima (que son la misma) sí
    // que han cambiado aunqu eno lo veáis
    ShowIt();
}

void FPImage::BlurInside(void) {
    if (!H) return;
    this->processor->BlurInsideObjects(this->ui->SliderKernel->value(), this->ui->SliderEdges->value(), edgeMetric);
    ShowIt();
}

void FPImage::CallSkinChange(void){
    this->SkinChange(0);
}

void FPImage::SkinChange(int value) {
    float rDesv = this->ui->SliderRDesv->value() / 100.0f;
    float gDesv = this->ui->SliderGDesv->value() / 100.0f;
    float bDesv = this->ui->SliderBDesv->value() / 100.0f;
    for (int i = 0; i < 6; i++) {
        processor->Blur(this->ui->SliderKernel->value());
    }
    processor->CreateLHS();
    processor->faceDetector->DetectSkin(rDesv, gDesv, bDesv);
    ShowIt();
}
void FPImage::ChangeBrightness(int value) {
    if (processor != nullptr) {
        int contrast = this->ui->SliderContrast->value();
        int brightness = this->ui->SliderBrightness->value();
        processor->ChangeContrast(contrast, brightness);
        this->DrawTransferenceFunction();
        ShowIt();
    }
}

void FPImage::Dilate() {
    this->processor->faceDetector->Dilate(this->ui->SliderBlobKernel->value());
    ShowIt();
}

void FPImage::Erode() {
    this->processor->faceDetector->Erode(this->ui->SliderBlobKernel->value());
    ShowIt();
}

void FPImage::ChangeContrast(int value) {
    if (processor != nullptr) {
        int contrast = this->ui->SliderContrast->value();
        int brightness = this->ui->SliderBrightness->value();
        processor->ChangeContrast(contrast, brightness);
        this->DrawTransferenceFunction();
        ShowIt();
    }
}

void FPImage::ChangeEdge(int value) {
    if (processor != nullptr) {
        if (ui->EuclideanRButton->isChecked()) {
            edgeMetric = ImageProcessor::EdgeMetric::EuclideanDistance;
        }
        if (ui->AbsDifferenceRButton->isChecked()) {
            edgeMetric = ImageProcessor::EdgeMetric::AbsDifference;
        }
        if (ui->MaxDifferenceRButton->isChecked()) {
            edgeMetric = ImageProcessor::EdgeMetric::MaxDifference;
        }
        processor->Edges(ui->SliderEdges->value(),edgeMetric);
        ShowIt();
    }
}

void FPImage::ChangedEdgeMethod() {
    this->ChangeEdge(0);
}

void FPImage::RandomDithering(int value) {
    processor->Dithering(ui->CheckRandomDithering->isChecked());
}

void FPImage::DrawTransferenceFunction() {
    QPainter p(&transference);
    p.setPen(QPen(Qt::yellow));
    int lastPos = 0;
    transference.fill(Qt::black);
    this->DrawHistograms();
    for (int i = 1; i < 256; i++) {

        p.setPen(QPen(Qt::yellow));
        p.drawLine(lastPos, 256 - processor->LUT[lastPos], i, 256 - processor->LUT[i]);

        p.setPen(QPen(Qt::red));
        float r = rHistogram[i];
        r = (rRawHistogram[i] * 99) / maxValue;
        p.drawLine(i, 256, i, 256 - r);

        p.setPen(QPen(Qt::green));
        float g = gHistogram[i];
        g = (gRawHistogram[i] * 99) / maxValue;
        p.drawLine(i, 256, i, 256 - g);

        p.setPen(QPen(Qt::blue));
        float b = bHistogram[i];
        b = (bRawHistogram[i] * 99) / maxValue;
        p.drawLine(i, 256, i, 256 - b);
        lastPos = i;
    }
    this->ui->TransferenceFunction->setPixmap(transference);
}

void FPImage::ChangeUserLUT(int x, int y) {
    transferenceXLastClicked = x;
    if (processor != nullptr) {
        processor->AddToSlope(x, 256 - y);
        processor->UserContrastTransform();
        ChangeContrast(0);
    }
}

void FPImage::DrawHistograms() {
    if (processor == nullptr) {
        return;
    }
    histogramsFilled = true;
    maxValue = processor->Histograms(rHistogram, gHistogram, bHistogram, rRawHistogram, gRawHistogram, bRawHistogram);

    QPainter p(&Dib1);
    p.setPen(QPen(Qt::red));
    int lastPos = 0;
    Dib1.fill(Qt::black);
    for (int i = 1; i < 256; i++) {
        p.drawLine(i, 99, i, 99 - rHistogram[i]);
        lastPos = i;
    }
    this->ui->EcranHistoR->setPixmap(Dib1);

    QPainter p2(&Dib2);
    p2.setPen(QPen(Qt::green));
    lastPos = 0;
    Dib2.fill(Qt::black);
    for (int i = 1; i < 256; i++) {
        p2.drawLine(i, 99, i, 99 - gHistogram[i]);
        lastPos = i;
    }
    this->ui->EcranHistoG->setPixmap(Dib2);

    QPainter p3(&Dib3);
    p3.setPen(QPen(Qt::blue));
    lastPos = 0;
    Dib3.fill(Qt::black);
    for (int i = 1; i < 256; i++) {
        p3.drawLine(i, 99, i, 99 - bHistogram[i]);
        lastPos = i;
    }
    this->ui->EcranHistoB->setPixmap(Dib3);
}

void FPImage::ResetHistograms() {
    if (!histogramsFilled) {
        return;
    }
    Dib1.fill(Qt::black);
    this->ui->EcranHistoR->setPixmap(Dib1);
    Dib2.fill(Qt::black);
    this->ui->EcranHistoG->setPixmap(Dib2);
    Dib3.fill(Qt::black);
    this->ui->EcranHistoB->setPixmap(Dib3);
    histogramsFilled = false;
}

void FPImage::AddPointToHistogram(uchar r, uchar g, uchar b) {
    ResetHistograms();
    float x = g / 255.0f;
    float y = r / 255.0f;
    x *= 100;
    y *= 100;
    QPainter p(&Dib1);
    p.setPen(QPen(Qt::yellow));
    p.drawLine(x, y, x,y);
    this->ui->EcranHistoR->setPixmap(Dib1);

    x = b / 255.0f;
    y = r / 255.0f;
    x *= 100;
    y *= 100;
    QPainter p2(&Dib2);
    p2.setPen(QPen(Qt::yellow));
    p2.drawLine(x, y, x, y);
    this->ui->EcranHistoG->setPixmap(Dib2);

    x = b / 255.0f;
    y = g / 255.0f;
    x *= 100;
    y *= 100;
    QPainter p3(&Dib3);
    p3.setPen(QPen(Qt::yellow));
    p3.drawLine(x, y, x, y);
    this->ui->EcranHistoB->setPixmap(Dib3);
}

//-------------------------------------------------
//-------------- Mostramos la imagen --------------
//-------------------------------------------------
inline void FPImage::ShowIt(void)
{
    // Creamos un lienzo (QPixmap) a partir de la QImage
    // y lo asignamos a la QLabel central
    ui->Ecran->setPixmap(QPixmap::fromImage(Image));
}
 