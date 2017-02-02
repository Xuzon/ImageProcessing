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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
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
    QLabel *EcranHistoR;
    QLabel *EcranHistoG;
    QLabel *EcranHistoB;
    QSpacerItem *verticalSpacer;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FPImage)
    {
        if (FPImage->objectName().isEmpty())
            FPImage->setObjectName(QStringLiteral("FPImage"));
        FPImage->resize(932, 623);
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
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 476, 548));
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
        EcranHistoR = new QLabel(centralWidget);
        EcranHistoR->setObjectName(QStringLiteral("EcranHistoR"));
        EcranHistoR->setMinimumSize(QSize(260, 104));
        EcranHistoR->setMaximumSize(QSize(260, 104));
        EcranHistoR->setFrameShape(QFrame::Panel);
        EcranHistoR->setFrameShadow(QFrame::Sunken);
        EcranHistoR->setLineWidth(2);
        EcranHistoR->setMidLineWidth(0);

        verticalLayout_2->addWidget(EcranHistoR);

        EcranHistoG = new QLabel(centralWidget);
        EcranHistoG->setObjectName(QStringLiteral("EcranHistoG"));
        EcranHistoG->setMinimumSize(QSize(260, 104));
        EcranHistoG->setMaximumSize(QSize(260, 104));
        EcranHistoG->setFrameShape(QFrame::Panel);
        EcranHistoG->setFrameShadow(QFrame::Sunken);
        EcranHistoG->setLineWidth(2);

        verticalLayout_2->addWidget(EcranHistoG);

        EcranHistoB = new QLabel(centralWidget);
        EcranHistoB->setObjectName(QStringLiteral("EcranHistoB"));
        EcranHistoB->setMinimumSize(QSize(260, 104));
        EcranHistoB->setMaximumSize(QSize(260, 104));
        EcranHistoB->setFrameShape(QFrame::Panel);
        EcranHistoB->setFrameShadow(QFrame::Sunken);
        EcranHistoB->setLineWidth(2);

        verticalLayout_2->addWidget(EcranHistoB);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout_2);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 5);
        FPImage->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FPImage);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 932, 21));
        FPImage->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FPImage);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FPImage->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FPImage);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FPImage->setStatusBar(statusBar);

        retranslateUi(FPImage);

        QMetaObject::connectSlotsByName(FPImage);
    } // setupUi

    void retranslateUi(QMainWindow *FPImage)
    {
        FPImage->setWindowTitle(QApplication::translate("FPImage", "FPImage", Q_NULLPTR));
        BLoad->setText(QApplication::translate("FPImage", "Load", Q_NULLPTR));
        BDoIt->setText(QApplication::translate("FPImage", "Do it", Q_NULLPTR));
        Ecran->setText(QString());
        EcranHistoR->setText(QString());
        EcranHistoG->setText(QString());
        EcranHistoB->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FPImage: public Ui_FPImage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FPIMAGE_H
