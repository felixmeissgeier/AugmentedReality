/********************************************************************************
** Form generated from reading UI file 'AugmentedRealityTutorial.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUGMENTEDREALITYTUTORIAL_H
#define UI_AUGMENTEDREALITYTUTORIAL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AugmentedRealityTutorialClass
{
public:
    QWidget *centralWidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QLabel *imageLabel;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_4;
    QRadioButton *radioInputWebCam;
    QRadioButton *radioInputFile;
    QPushButton *pushReloadFileInput;
    QPushButton *pushPause;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_5;
    QPushButton *pushCalibrateGuitar;
    QCheckBox *checkShowCalibration;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_6;
    QPushButton *pushFretBoardDetected;
    QPushButton *pushSaveFretboard2File;
    QSpacerItem *verticalSpacer;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *AugmentedRealityTutorialClass)
    {
        if (AugmentedRealityTutorialClass->objectName().isEmpty())
            AugmentedRealityTutorialClass->setObjectName(QString::fromUtf8("AugmentedRealityTutorialClass"));
        AugmentedRealityTutorialClass->setEnabled(true);
        AugmentedRealityTutorialClass->resize(966, 595);
        centralWidget = new QWidget(AugmentedRealityTutorialClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 941, 521));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_2 = new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        imageLabel = new QLabel(horizontalLayoutWidget);
        imageLabel->setObjectName(QString::fromUtf8("imageLabel"));
        imageLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(imageLabel);

        horizontalSpacer = new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_4 = new QGroupBox(horizontalLayoutWidget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_4 = new QVBoxLayout(groupBox_4);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        radioInputWebCam = new QRadioButton(groupBox_4);
        radioInputWebCam->setObjectName(QString::fromUtf8("radioInputWebCam"));
        radioInputWebCam->setChecked(false);

        verticalLayout_4->addWidget(radioInputWebCam);

        radioInputFile = new QRadioButton(groupBox_4);
        radioInputFile->setObjectName(QString::fromUtf8("radioInputFile"));
        radioInputFile->setChecked(true);

        verticalLayout_4->addWidget(radioInputFile);

        pushReloadFileInput = new QPushButton(groupBox_4);
        pushReloadFileInput->setObjectName(QString::fromUtf8("pushReloadFileInput"));

        verticalLayout_4->addWidget(pushReloadFileInput);

        pushPause = new QPushButton(groupBox_4);
        pushPause->setObjectName(QString::fromUtf8("pushPause"));

        verticalLayout_4->addWidget(pushPause);


        verticalLayout->addWidget(groupBox_4);

        groupBox_5 = new QGroupBox(horizontalLayoutWidget);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        verticalLayout_5 = new QVBoxLayout(groupBox_5);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        pushCalibrateGuitar = new QPushButton(groupBox_5);
        pushCalibrateGuitar->setObjectName(QString::fromUtf8("pushCalibrateGuitar"));

        verticalLayout_5->addWidget(pushCalibrateGuitar);

        checkShowCalibration = new QCheckBox(groupBox_5);
        checkShowCalibration->setObjectName(QString::fromUtf8("checkShowCalibration"));
        checkShowCalibration->setChecked(true);

        verticalLayout_5->addWidget(checkShowCalibration);


        verticalLayout->addWidget(groupBox_5);

        groupBox_6 = new QGroupBox(horizontalLayoutWidget);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        verticalLayout_6 = new QVBoxLayout(groupBox_6);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        pushFretBoardDetected = new QPushButton(groupBox_6);
        pushFretBoardDetected->setObjectName(QString::fromUtf8("pushFretBoardDetected"));

        verticalLayout_6->addWidget(pushFretBoardDetected);

        pushSaveFretboard2File = new QPushButton(groupBox_6);
        pushSaveFretboard2File->setObjectName(QString::fromUtf8("pushSaveFretboard2File"));
        pushSaveFretboard2File->setEnabled(false);
        pushSaveFretboard2File->setCheckable(false);

        verticalLayout_6->addWidget(pushSaveFretboard2File);


        verticalLayout->addWidget(groupBox_6);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        AugmentedRealityTutorialClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(AugmentedRealityTutorialClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 966, 21));
        AugmentedRealityTutorialClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(AugmentedRealityTutorialClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        AugmentedRealityTutorialClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(AugmentedRealityTutorialClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        AugmentedRealityTutorialClass->setStatusBar(statusBar);

        retranslateUi(AugmentedRealityTutorialClass);
        QObject::connect(radioInputWebCam, SIGNAL(toggled(bool)), AugmentedRealityTutorialClass, SLOT(inputDeviceChanged()));
        QObject::connect(pushReloadFileInput, SIGNAL(clicked()), AugmentedRealityTutorialClass, SLOT(reloadFileInputPushed()));
        QObject::connect(pushPause, SIGNAL(clicked()), AugmentedRealityTutorialClass, SLOT(pauseVideo()));
        QObject::connect(checkShowCalibration, SIGNAL(stateChanged(int)), AugmentedRealityTutorialClass, SLOT(showCalibrationChanged()));
        QObject::connect(pushCalibrateGuitar, SIGNAL(clicked()), AugmentedRealityTutorialClass, SLOT(calibrateGuitar()));
        QObject::connect(pushFretBoardDetected, SIGNAL(clicked()), AugmentedRealityTutorialClass, SLOT(fretBoardDetected()));
        QObject::connect(pushSaveFretboard2File, SIGNAL(clicked()), AugmentedRealityTutorialClass, SLOT(saveFretboardToFile()));

        QMetaObject::connectSlotsByName(AugmentedRealityTutorialClass);
    } // setupUi

    void retranslateUi(QMainWindow *AugmentedRealityTutorialClass)
    {
        AugmentedRealityTutorialClass->setWindowTitle(QApplication::translate("AugmentedRealityTutorialClass", "AugmentedRealityTutorial", 0, QApplication::UnicodeUTF8));
        imageLabel->setText(QApplication::translate("AugmentedRealityTutorialClass", "No Camera Input", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("AugmentedRealityTutorialClass", "Input Device", 0, QApplication::UnicodeUTF8));
        radioInputWebCam->setText(QApplication::translate("AugmentedRealityTutorialClass", "WebCam", 0, QApplication::UnicodeUTF8));
        radioInputFile->setText(QApplication::translate("AugmentedRealityTutorialClass", "File Input", 0, QApplication::UnicodeUTF8));
        pushReloadFileInput->setText(QApplication::translate("AugmentedRealityTutorialClass", "Reload File", 0, QApplication::UnicodeUTF8));
        pushPause->setText(QApplication::translate("AugmentedRealityTutorialClass", "Pause", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("AugmentedRealityTutorialClass", "Basic Settings", 0, QApplication::UnicodeUTF8));
        pushCalibrateGuitar->setText(QApplication::translate("AugmentedRealityTutorialClass", "Calibrate Guitar", 0, QApplication::UnicodeUTF8));
        checkShowCalibration->setText(QApplication::translate("AugmentedRealityTutorialClass", "Show Calibration", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("AugmentedRealityTutorialClass", "Calibration Settings", 0, QApplication::UnicodeUTF8));
        pushFretBoardDetected->setText(QApplication::translate("AugmentedRealityTutorialClass", "Fretboard Detected", 0, QApplication::UnicodeUTF8));
        pushSaveFretboard2File->setText(QApplication::translate("AugmentedRealityTutorialClass", "Save Fretboard To File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AugmentedRealityTutorialClass: public Ui_AugmentedRealityTutorialClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUGMENTEDREALITYTUTORIAL_H
