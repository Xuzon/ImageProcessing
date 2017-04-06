/********************************************************************************
** Form generated from reading UI file 'fpimage.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FPIMAGE_H
#define UI_FPIMAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FPImage
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *ERes;
    QLineEdit *EFile;
    QHBoxLayout *horizontalLayout;
    QPushButton *BLoad;
    QPushButton *BDoIt;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout;
    QLabel *Ecran;
    QVBoxLayout *verticalLayout_2;
    QLabel *LRDesvMulti;
    QSlider *SliderRDesv;
    QLabel *LGDesvMulti;
    QSlider *SliderGDesv;
    QLabel *LBDesvMulti;
    QSlider *SliderBDesv;
    QPushButton *BDilate;
    QPushButton *BErode;
    QTabWidget *tabWidget;
    QWidget *tab_3;
    QLabel *EcranHistoR;
    QLabel *EcranHistoG;
    QLabel *EcranHistoB;
    QPushButton *BHistogram;
    QSlider *SliderBrightness;
    QLabel *LBrightness;
    QLabel *LContrast;
    QSlider *SliderContrast;
    QLabel *TransferenceFunction;
    QWidget *tab_4;
    QLabel *LEdges;
    QSlider *SliderEdges;
    QGroupBox *RadioEdgeGroup;
    QRadioButton *EuclideanRButton;
    QRadioButton *AbsDifferenceRButton;
    QRadioButton *MaxDifferenceRButton;
    QCheckBox *CheckRandomDithering;
    QLabel *LAdaptEq;
    QSlider *SliderAdaptEq;
    QLabel *LKernel;
    QSlider *SliderKernel;
    QPushButton *BBlurInside;
    QSpacerItem *verticalSpacer;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FPImage)
    {
        if (FPImage->objectName().isEmpty())
            FPImage->setObjectName(QStringLiteral("FPImage"));
        FPImage->resize(1264, 743);
        centralWidget = new QWidget(FPImage);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        ERes = new QPlainTextEdit(centralWidget);
        ERes->setObjectName(QStringLiteral("ERes"));

        verticalLayout->addWidget(ERes);

        EFile = new QLineEdit(centralWidget);
        EFile->setObjectName(QStringLiteral("EFile"));

        verticalLayout->addWidget(EFile);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        BLoad = new QPushButton(centralWidget);
        BLoad->setObjectName(QStringLiteral("BLoad"));

        horizontalLayout->addWidget(BLoad);

        BDoIt = new QPushButton(centralWidget);
        BDoIt->setObjectName(QStringLiteral("BDoIt"));

        horizontalLayout->addWidget(BDoIt);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);

        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setFrameShape(QFrame::Panel);
        scrollArea->setLineWidth(2);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 809, 668));
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        Ecran = new QLabel(scrollAreaWidgetContents);
        Ecran->setObjectName(QStringLiteral("Ecran"));

        gridLayout->addWidget(Ecran, 0, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout_2->addWidget(scrollArea);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        LRDesvMulti = new QLabel(centralWidget);
        LRDesvMulti->setObjectName(QStringLiteral("LRDesvMulti"));

        verticalLayout_2->addWidget(LRDesvMulti);

        SliderRDesv = new QSlider(centralWidget);
        SliderRDesv->setObjectName(QStringLiteral("SliderRDesv"));
        SliderRDesv->setMaximum(200);
        SliderRDesv->setSliderPosition(100);
        SliderRDesv->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(SliderRDesv);

        LGDesvMulti = new QLabel(centralWidget);
        LGDesvMulti->setObjectName(QStringLiteral("LGDesvMulti"));

        verticalLayout_2->addWidget(LGDesvMulti);

        SliderGDesv = new QSlider(centralWidget);
        SliderGDesv->setObjectName(QStringLiteral("SliderGDesv"));
        SliderGDesv->setMaximum(200);
        SliderGDesv->setSliderPosition(100);
        SliderGDesv->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(SliderGDesv);

        LBDesvMulti = new QLabel(centralWidget);
        LBDesvMulti->setObjectName(QStringLiteral("LBDesvMulti"));

        verticalLayout_2->addWidget(LBDesvMulti);

        SliderBDesv = new QSlider(centralWidget);
        SliderBDesv->setObjectName(QStringLiteral("SliderBDesv"));
        SliderBDesv->setMaximum(200);
        SliderBDesv->setSliderPosition(100);
        SliderBDesv->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(SliderBDesv);

        BDilate = new QPushButton(centralWidget);
        BDilate->setObjectName(QStringLiteral("BDilate"));

        verticalLayout_2->addWidget(BDilate);

        BErode = new QPushButton(centralWidget);
        BErode->setObjectName(QStringLiteral("BErode"));

        verticalLayout_2->addWidget(BErode);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setMinimumSize(QSize(256, 500));
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        EcranHistoR = new QLabel(tab_3);
        EcranHistoR->setObjectName(QStringLiteral("EcranHistoR"));
        EcranHistoR->setGeometry(QRect(0, 0, 260, 104));
        EcranHistoR->setMinimumSize(QSize(260, 104));
        EcranHistoR->setMaximumSize(QSize(260, 104));
        EcranHistoR->setFrameShape(QFrame::Panel);
        EcranHistoR->setFrameShadow(QFrame::Sunken);
        EcranHistoR->setLineWidth(2);
        EcranHistoR->setMidLineWidth(0);
        EcranHistoG = new QLabel(tab_3);
        EcranHistoG->setObjectName(QStringLiteral("EcranHistoG"));
        EcranHistoG->setGeometry(QRect(0, 100, 260, 104));
        EcranHistoG->setMinimumSize(QSize(260, 104));
        EcranHistoG->setMaximumSize(QSize(260, 104));
        EcranHistoG->setFrameShape(QFrame::Panel);
        EcranHistoG->setFrameShadow(QFrame::Sunken);
        EcranHistoG->setLineWidth(2);
        EcranHistoB = new QLabel(tab_3);
        EcranHistoB->setObjectName(QStringLiteral("EcranHistoB"));
        EcranHistoB->setGeometry(QRect(0, 200, 260, 104));
        EcranHistoB->setMinimumSize(QSize(260, 104));
        EcranHistoB->setMaximumSize(QSize(260, 104));
        EcranHistoB->setFrameShape(QFrame::Panel);
        EcranHistoB->setFrameShadow(QFrame::Sunken);
        EcranHistoB->setLineWidth(2);
        BHistogram = new QPushButton(tab_3);
        BHistogram->setObjectName(QStringLiteral("BHistogram"));
        BHistogram->setGeometry(QRect(0, 300, 256, 23));
        SliderBrightness = new QSlider(tab_3);
        SliderBrightness->setObjectName(QStringLiteral("SliderBrightness"));
        SliderBrightness->setGeometry(QRect(0, 340, 256, 22));
        SliderBrightness->setMaximum(100);
        SliderBrightness->setValue(50);
        SliderBrightness->setOrientation(Qt::Horizontal);
        LBrightness = new QLabel(tab_3);
        LBrightness->setObjectName(QStringLiteral("LBrightness"));
        LBrightness->setGeometry(QRect(0, 330, 256, 13));
        LContrast = new QLabel(tab_3);
        LContrast->setObjectName(QStringLiteral("LContrast"));
        LContrast->setGeometry(QRect(0, 350, 256, 13));
        SliderContrast = new QSlider(tab_3);
        SliderContrast->setObjectName(QStringLiteral("SliderContrast"));
        SliderContrast->setGeometry(QRect(0, 370, 256, 22));
        SliderContrast->setMinimum(0);
        SliderContrast->setMaximum(100);
        SliderContrast->setValue(50);
        SliderContrast->setOrientation(Qt::Horizontal);
        TransferenceFunction = new QLabel(tab_3);
        TransferenceFunction->setObjectName(QStringLiteral("TransferenceFunction"));
        TransferenceFunction->setGeometry(QRect(0, 390, 256, 256));
        TransferenceFunction->setMinimumSize(QSize(256, 256));
        TransferenceFunction->setBaseSize(QSize(256, 256));
        TransferenceFunction->setFrameShape(QFrame::Panel);
        TransferenceFunction->setFrameShadow(QFrame::Sunken);
        TransferenceFunction->setLineWidth(2);
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        LEdges = new QLabel(tab_4);
        LEdges->setObjectName(QStringLiteral("LEdges"));
        LEdges->setGeometry(QRect(10, 0, 256, 13));
        SliderEdges = new QSlider(tab_4);
        SliderEdges->setObjectName(QStringLiteral("SliderEdges"));
        SliderEdges->setGeometry(QRect(0, 10, 256, 22));
        SliderEdges->setMaximum(300);
        SliderEdges->setValue(100);
        SliderEdges->setOrientation(Qt::Horizontal);
        RadioEdgeGroup = new QGroupBox(tab_4);
        RadioEdgeGroup->setObjectName(QStringLiteral("RadioEdgeGroup"));
        RadioEdgeGroup->setGeometry(QRect(0, 40, 256, 100));
        RadioEdgeGroup->setMinimumSize(QSize(0, 100));
        EuclideanRButton = new QRadioButton(RadioEdgeGroup);
        EuclideanRButton->setObjectName(QStringLiteral("EuclideanRButton"));
        EuclideanRButton->setGeometry(QRect(0, 20, 260, 17));
        EuclideanRButton->setChecked(true);
        AbsDifferenceRButton = new QRadioButton(RadioEdgeGroup);
        AbsDifferenceRButton->setObjectName(QStringLiteral("AbsDifferenceRButton"));
        AbsDifferenceRButton->setGeometry(QRect(0, 40, 260, 17));
        MaxDifferenceRButton = new QRadioButton(RadioEdgeGroup);
        MaxDifferenceRButton->setObjectName(QStringLiteral("MaxDifferenceRButton"));
        MaxDifferenceRButton->setGeometry(QRect(0, 60, 260, 17));
        CheckRandomDithering = new QCheckBox(RadioEdgeGroup);
        CheckRandomDithering->setObjectName(QStringLiteral("CheckRandomDithering"));
        CheckRandomDithering->setGeometry(QRect(0, 80, 256, 17));
        LAdaptEq = new QLabel(tab_4);
        LAdaptEq->setObjectName(QStringLiteral("LAdaptEq"));
        LAdaptEq->setGeometry(QRect(0, 140, 256, 13));
        SliderAdaptEq = new QSlider(tab_4);
        SliderAdaptEq->setObjectName(QStringLiteral("SliderAdaptEq"));
        SliderAdaptEq->setGeometry(QRect(0, 160, 256, 22));
        SliderAdaptEq->setMinimum(5);
        SliderAdaptEq->setMaximum(21);
        SliderAdaptEq->setOrientation(Qt::Horizontal);
        LKernel = new QLabel(tab_4);
        LKernel->setObjectName(QStringLiteral("LKernel"));
        LKernel->setGeometry(QRect(0, 190, 71, 16));
        SliderKernel = new QSlider(tab_4);
        SliderKernel->setObjectName(QStringLiteral("SliderKernel"));
        SliderKernel->setGeometry(QRect(-1, 211, 251, 21));
        SliderKernel->setMinimum(3);
        SliderKernel->setMaximum(9);
        SliderKernel->setSingleStep(2);
        SliderKernel->setPageStep(2);
        SliderKernel->setSliderPosition(3);
        SliderKernel->setOrientation(Qt::Horizontal);
        BBlurInside = new QPushButton(tab_4);
        BBlurInside->setObjectName(QStringLiteral("BBlurInside"));
        BBlurInside->setGeometry(QRect(0, 240, 75, 23));
        tabWidget->addTab(tab_4, QString());

        verticalLayout_2->addWidget(tabWidget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout_2);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 5);
        FPImage->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FPImage);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1264, 21));
        FPImage->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FPImage);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FPImage->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FPImage);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FPImage->setStatusBar(statusBar);

        retranslateUi(FPImage);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(FPImage);
    } // setupUi

    void retranslateUi(QMainWindow *FPImage)
    {
        FPImage->setWindowTitle(QApplication::translate("FPImage", "FPImage", Q_NULLPTR));
        BLoad->setText(QApplication::translate("FPImage", "Load", Q_NULLPTR));
        BDoIt->setText(QApplication::translate("FPImage", "Do it", Q_NULLPTR));
        Ecran->setText(QString());
        LRDesvMulti->setText(QApplication::translate("FPImage", "L desviation multiplier", Q_NULLPTR));
        LGDesvMulti->setText(QApplication::translate("FPImage", "S desviation multiplier", Q_NULLPTR));
        LBDesvMulti->setText(QApplication::translate("FPImage", "B desviation multiplier", Q_NULLPTR));
        BDilate->setText(QApplication::translate("FPImage", "Dilate", Q_NULLPTR));
        BErode->setText(QApplication::translate("FPImage", "Erode", Q_NULLPTR));
        EcranHistoR->setText(QString());
        EcranHistoG->setText(QString());
        EcranHistoB->setText(QString());
        BHistogram->setText(QApplication::translate("FPImage", "Do Histograms", Q_NULLPTR));
        LBrightness->setText(QApplication::translate("FPImage", "Brightness", Q_NULLPTR));
        LContrast->setText(QApplication::translate("FPImage", "Contrast", Q_NULLPTR));
        TransferenceFunction->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("FPImage", "Histograms Brightness and Contrast", Q_NULLPTR));
        LEdges->setText(QApplication::translate("FPImage", "Edges", Q_NULLPTR));
        RadioEdgeGroup->setTitle(QApplication::translate("FPImage", "Edge Detection Method", Q_NULLPTR));
        EuclideanRButton->setText(QApplication::translate("FPImage", "Euclidean", Q_NULLPTR));
        AbsDifferenceRButton->setText(QApplication::translate("FPImage", "AbsDifference", Q_NULLPTR));
        MaxDifferenceRButton->setText(QApplication::translate("FPImage", "MaxDifference", Q_NULLPTR));
        CheckRandomDithering->setText(QApplication::translate("FPImage", "Random Dithering", Q_NULLPTR));
        LAdaptEq->setText(QApplication::translate("FPImage", "Adapatative equalization Neighborhood", Q_NULLPTR));
        LKernel->setText(QApplication::translate("FPImage", "Blur Kernel", Q_NULLPTR));
        BBlurInside->setText(QApplication::translate("FPImage", "BlurInside", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("FPImage", "Tab 2", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FPImage: public Ui_FPImage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FPIMAGE_H
