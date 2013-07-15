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
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
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
    QLabel *label;
    QSlider *playingSpeedSlider;
    QLabel *lblCurrentSpeed;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout_2;
    QLabel *label_4;
    QComboBox *comboThresholdType;
    QRadioButton *radioFixedLevelThreshold;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *labelThresholdValue;
    QSlider *sliderThresholdValue;
    QSpacerItem *verticalSpacer_2;
    QRadioButton *radioAdaptiveThreshold;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout;
    QComboBox *comboAdaptiveMode;
    QLabel *label_3;
    QSpinBox *spinBlocksize;
    QLabel *label_2;
    QDoubleSpinBox *spinConstantC;
    QLabel *label_5;
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
        AugmentedRealityTutorialClass->resize(969, 801);
        centralWidget = new QWidget(AugmentedRealityTutorialClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 941, 822));
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

        label = new QLabel(groupBox_5);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_5->addWidget(label);

        playingSpeedSlider = new QSlider(groupBox_5);
        playingSpeedSlider->setObjectName(QString::fromUtf8("playingSpeedSlider"));
        playingSpeedSlider->setMinimum(1);
        playingSpeedSlider->setMaximum(5);
        playingSpeedSlider->setSingleStep(1);
        playingSpeedSlider->setSliderPosition(1);
        playingSpeedSlider->setOrientation(Qt::Horizontal);
        playingSpeedSlider->setTickPosition(QSlider::TicksBelow);
        playingSpeedSlider->setTickInterval(1);

        verticalLayout_5->addWidget(playingSpeedSlider);

        lblCurrentSpeed = new QLabel(groupBox_5);
        lblCurrentSpeed->setObjectName(QString::fromUtf8("lblCurrentSpeed"));

        verticalLayout_5->addWidget(lblCurrentSpeed);


        verticalLayout->addWidget(groupBox_5);

        groupBox = new QGroupBox(horizontalLayoutWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setSpacing(6);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_4);

        comboThresholdType = new QComboBox(groupBox);
        comboThresholdType->setObjectName(QString::fromUtf8("comboThresholdType"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, comboThresholdType);


        verticalLayout_2->addLayout(formLayout_2);

        radioFixedLevelThreshold = new QRadioButton(groupBox);
        radioFixedLevelThreshold->setObjectName(QString::fromUtf8("radioFixedLevelThreshold"));
        radioFixedLevelThreshold->setChecked(false);

        verticalLayout_2->addWidget(radioFixedLevelThreshold);

        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setEnabled(true);
        verticalLayout_3 = new QVBoxLayout(groupBox_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        labelThresholdValue = new QLabel(groupBox_3);
        labelThresholdValue->setObjectName(QString::fromUtf8("labelThresholdValue"));
        labelThresholdValue->setEnabled(true);

        verticalLayout_3->addWidget(labelThresholdValue);

        sliderThresholdValue = new QSlider(groupBox_3);
        sliderThresholdValue->setObjectName(QString::fromUtf8("sliderThresholdValue"));
        sliderThresholdValue->setEnabled(true);
        sliderThresholdValue->setMaximum(255);
        sliderThresholdValue->setValue(60);
        sliderThresholdValue->setOrientation(Qt::Horizontal);

        verticalLayout_3->addWidget(sliderThresholdValue);


        verticalLayout_2->addWidget(groupBox_3);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        radioAdaptiveThreshold = new QRadioButton(groupBox);
        radioAdaptiveThreshold->setObjectName(QString::fromUtf8("radioAdaptiveThreshold"));
        radioAdaptiveThreshold->setChecked(true);

        verticalLayout_2->addWidget(radioAdaptiveThreshold);

        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setEnabled(true);
        formLayout = new QFormLayout(groupBox_2);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        comboAdaptiveMode = new QComboBox(groupBox_2);
        comboAdaptiveMode->setObjectName(QString::fromUtf8("comboAdaptiveMode"));

        formLayout->setWidget(0, QFormLayout::FieldRole, comboAdaptiveMode);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_3);

        spinBlocksize = new QSpinBox(groupBox_2);
        spinBlocksize->setObjectName(QString::fromUtf8("spinBlocksize"));
        spinBlocksize->setMinimum(1);
        spinBlocksize->setMaximum(9999);
        spinBlocksize->setSingleStep(2);
        spinBlocksize->setValue(101);

        formLayout->setWidget(1, QFormLayout::FieldRole, spinBlocksize);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        spinConstantC = new QDoubleSpinBox(groupBox_2);
        spinConstantC->setObjectName(QString::fromUtf8("spinConstantC"));
        spinConstantC->setMaximum(9999.99);
        spinConstantC->setValue(5);

        formLayout->setWidget(2, QFormLayout::FieldRole, spinConstantC);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_5);


        verticalLayout_2->addWidget(groupBox_2);


        verticalLayout->addWidget(groupBox);

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
        menuBar->setGeometry(QRect(0, 0, 969, 22));
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
        QObject::connect(comboThresholdType, SIGNAL(currentIndexChanged(QString)), AugmentedRealityTutorialClass, SLOT(thresholdTypeChanged()));
        QObject::connect(radioFixedLevelThreshold, SIGNAL(toggled(bool)), AugmentedRealityTutorialClass, SLOT(thresholdTypeChanged()));
        QObject::connect(radioFixedLevelThreshold, SIGNAL(toggled(bool)), groupBox_3, SLOT(setEnabled(bool)));
        QObject::connect(radioAdaptiveThreshold, SIGNAL(toggled(bool)), groupBox_2, SLOT(setEnabled(bool)));
        QObject::connect(sliderThresholdValue, SIGNAL(valueChanged(int)), AugmentedRealityTutorialClass, SLOT(thresholdValueChanged(int)));
        QObject::connect(comboAdaptiveMode, SIGNAL(currentIndexChanged(int)), AugmentedRealityTutorialClass, SLOT(adaptiveThresholdSettingsChanged()));
        QObject::connect(spinBlocksize, SIGNAL(valueChanged(int)), AugmentedRealityTutorialClass, SLOT(adaptiveThresholdSettingsChanged()));
        QObject::connect(spinConstantC, SIGNAL(valueChanged(double)), AugmentedRealityTutorialClass, SLOT(adaptiveThresholdSettingsChanged()));
        QObject::connect(playingSpeedSlider, SIGNAL(valueChanged(int)), AugmentedRealityTutorialClass, SLOT(playSpeedChanged()));

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
        label->setText(QApplication::translate("AugmentedRealityTutorialClass", "Playing Speed", 0, QApplication::UnicodeUTF8));
        lblCurrentSpeed->setText(QApplication::translate("AugmentedRealityTutorialClass", "1", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("AugmentedRealityTutorialClass", "Threshold Settings", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("AugmentedRealityTutorialClass", "Threshold Type:", 0, QApplication::UnicodeUTF8));
        comboThresholdType->clear();
        comboThresholdType->insertItems(0, QStringList()
         << QApplication::translate("AugmentedRealityTutorialClass", "BINARY", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AugmentedRealityTutorialClass", "INVERSE BINARY", 0, QApplication::UnicodeUTF8)
        );
        radioFixedLevelThreshold->setText(QApplication::translate("AugmentedRealityTutorialClass", "Fixed-Level Threshold", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("AugmentedRealityTutorialClass", "Fixed-Level Threshold Settings", 0, QApplication::UnicodeUTF8));
        labelThresholdValue->setText(QApplication::translate("AugmentedRealityTutorialClass", "threshold value: ", 0, QApplication::UnicodeUTF8));
        radioAdaptiveThreshold->setText(QApplication::translate("AugmentedRealityTutorialClass", "Adaptive Threshold", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("AugmentedRealityTutorialClass", "Adaptive Threshold Settings", 0, QApplication::UnicodeUTF8));
        comboAdaptiveMode->clear();
        comboAdaptiveMode->insertItems(0, QStringList()
         << QApplication::translate("AugmentedRealityTutorialClass", "MEAN", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AugmentedRealityTutorialClass", "GAUSSIAN", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("AugmentedRealityTutorialClass", "Adaptive Mode:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AugmentedRealityTutorialClass", "Blocksize:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("AugmentedRealityTutorialClass", "constant C:", 0, QApplication::UnicodeUTF8));
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
