#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widget.h"
#include "json.hpp"
#include <QLayout>
using jsos = nlohmann::json;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    openGlWidget = new OpenGlWidget();

    connect(ui->slider_seed, SIGNAL(sliderReleased()), this, SLOT(slider_valueChanged()));
    connect(ui->slider_segments, SIGNAL(sliderReleased()), this, SLOT(slider_valueChanged()));
    ui->horizontalLayout->addWidget(openGlWidget, 66);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slider_valueChanged()
{
    json j =
    {
        {"mSeed" , ui->slider_seed->value()},
        {"mSegments" , 6},
        {"mLevels" , ui->slider_segments->value()},
        {"mVMultiplier" , 0.36f},
        {"mTwigScale" , 0.7f},
        {"mInitialBranchLength" , 0.5f},
        {"mLengthFalloffFactor" , 0.85f},
        {"mLengthFalloffPower" , 0.99f},
        {"mClumpMax" , 0.454f},
        {"mClumpMin" , 0.404f},
        {"mBranchFactor" , 2.45f},
        {"mDropAmount" , 0},
        {"mGrowAmount" , 0.235f},
        {"mSweepAmount" , 0.01f},
        {"mMaxRadius" , 0.139f},
        {"mClimbRate" , 0.371f},
        {"mTrunkKink" , 0.093f},
        {"mTreeSteps" , 5},
        {"mTaperRate" , 0.947f},
        {"mRadiusFalloffRate" , 0.73f},
        {"mTwistRate" , 3.02f},
        {"mTrunkLength" , 2.4f}
    };
    openGlWidget->loadFromJSON(j);
}

