#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widget.h"
#include "json.hpp"
#include <QLayout>
#include <QFileDialog>
#include <QTextStream>
using jsos = nlohmann::json;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    openGlWidget = new OpenGlWidget();

    ui->slider_seed->setValue(462);
    ui->slider_levels->setValue(5);
    j =
    {
        {"mSeed" , 462},
        {"mSegments" , 6},
        {"mLevels" , 5},
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
    connect(ui->slider_seed, SIGNAL(sliderReleased()), this, SLOT(slider_valueChanged()));
    connect(ui->slider_levels, SIGNAL(sliderReleased()), this, SLOT(slider_valueChanged()));
    ui->horizontalLayout->addWidget(openGlWidget, 66);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slider_valueChanged()
{
    j["mSeed"] = ui->slider_seed->value();
    j["mLevels"] = ui->slider_levels->value();
    openGlWidget->loadFromJSON(j);
}


void MainWindow::on_actionOpen_triggered()
{
    opened_file_path = QFileDialog::getOpenFileName(this, "Open a file", "C://");
    if(opened_file_path != "")
    {
        QFile file(opened_file_path);
        if(file.open(QFile::ReadOnly)) {
            QTextStream stream(&file);
            std::string shader_src =  stream.readAll().toStdString();
            j = json::parse(shader_src);
            openGlWidget->loadFromJSON(j);
            set_sliders();
    }
    }
}


void MainWindow::on_actionSave_triggered()
{
    if(opened_file_path != "")
    {
        QFile file(opened_file_path);
        if(file.open(QFile::WriteOnly))
        {
            QTextStream stream(&file);
            stream << j.dump(4).c_str();
        }
    }

}

void MainWindow::set_sliders()
{
    ui->slider_seed->setValue(j["mSeed"]);
    ui->slider_levels->setValue(j["mLevels"]);
}


void MainWindow::on_actionSaveAs_triggered()
{
    opened_file_path = QFileDialog::getSaveFileName(this, "Save a file", "C://");
    if(opened_file_path != "")
    {
        QFile file(opened_file_path);
        if(file.open(QFile::WriteOnly))
        {
            QTextStream stream(&file);
            stream << j.dump(4).c_str();
        }
    }
}

