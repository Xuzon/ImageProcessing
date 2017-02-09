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
            statusBar()->showMessage(QString::number(x)+":"+
                                      QString::number(y)+" "+
                                      QString::number(pixR[(y*S+3*x)])+":"+
                                      QString::number(pixG[(y*S+3*x)])+":"+
                                      QString::number(pixB[(y*S+3*x)]));
            // Devolvemos un "true" que significa que hemos gestionado el evento
            return true;
        } else return false;  // No era para nosotros, lo dejamos en paz
    } else return false;
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


    // "Instalamos" un "filtro de eventos" en nuestro QLabel Ecran
    // para capturar clicks de ratón sobre la imagen
    ui->Ecran->installEventFilter(this);


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
    processor->SepiaFilter(65,0,0,15);
    



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

void FPImage::ChangeBrightness(int value) {
    if (processor != nullptr) {
        int contrast = this->ui->SliderContrast->value();;
        int brightness = this->ui->SliderBrightness->value();
        processor->ChangeContrast(contrast, brightness);
        ShowIt();
    }
}

void FPImage::ChangeContrast(int value) {
    if (processor != nullptr) {
        int contrast = this->ui->SliderContrast->value();;
        int brightness = this->ui->SliderBrightness->value();
        processor->ChangeContrast(contrast, brightness);
        ShowIt();
    }
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
