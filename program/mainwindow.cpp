#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widget.h"
#include "json.hpp"
#include <QLayout>
#include <QFileDialog>
#include <QTextStream>
#include <QTime>
#include <QKeyEvent>
#include <QStringListModel>
#include <QList>
#include <QString>
#include <QList>
#include "changetexwindow.h"

using json = nlohmann::json;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    j = {
        {"mSeed" , 262},
        {"mSegments" , 6},
        {"mLevels" , 5},
        {"mVMultiplier" , 0.36f},
        {"mTwigScale" , 0.39f},
        {"mInitialBranchLength" , 0.49f},
        {"mLengthFalloffFactor" , 0.85f},
        {"mLengthFalloffPower" , 0.99f},
        {"mClumpMax" , 0.454f},
        {"mClumpMin" , 0.404f},
        {"mBranchFactor" , 2.45f},
        {"mDropAmount" , -0.1f},
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

    openGlWidget = new OpenGlWidget();
    openGlWidget->trees = &(this->trees);
    openGlWidget->listWidget = ui->listWidget;
    ui->horizontalLayout->addWidget(openGlWidget, 80);

    set_sliders();

    sliders.push_back(ui->slider_seed);
    sliders.push_back(ui->slider_treeSteps);
    sliders.push_back(ui->slider_levels);
    sliders.push_back(ui->slBranchFac);
    sliders.push_back(ui->slVMul);
    sliders.push_back(ui->slClumpMin);
    sliders.push_back(ui->slClumpMax);
    sliders.push_back(ui->slClumbRate);
    sliders.push_back(ui->slTwigScale);
    sliders.push_back(ui->slInitBranch);
    sliders.push_back(ui->slLenFallFac);
    sliders.push_back(ui->slLenFallPow);
    sliders.push_back(ui->slDrop);
    sliders.push_back(ui->slSweep);
    sliders.push_back(ui->slGrow);
    sliders.push_back(ui->slMaxRad);
    sliders.push_back(ui->slRadFall);
    sliders.push_back(ui->slTaper);
    sliders.push_back(ui->slTrunkKink);
    sliders.push_back(ui->slTrunkLen);
    sliders.push_back(ui->slTwistRate);


    for (const QSlider* i : sliders) {
        connect(i, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged()));
     }
    connect(ui->buttonGrow, SIGNAL(clicked()), this, SLOT(growTree()));
    connect(ui->btTexture, SIGNAL(clicked()), this, SLOT(loadTexWindow()));
    connect(ui->slGrowth, SIGNAL(valueChanged(int)), this, SLOT(changeGrowthTree(int)));
    connect(ui->btDelete, SIGNAL(clicked()), this, SLOT(deleteTree()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTexture(QString path, TextureType type)
{
    std::string str = path.toStdString();
    const char* p = str.c_str();
    openGlWidget->loadTexture(p, type);
}

void MainWindow::btTextureEnabled(bool enabled)
{
    ui->btTexture->setEnabled(enabled);
}

void MainWindow::slider_valueChanged()
{
    j["mSeed"] = ui->slider_seed->value();
    j["mLevels"] = ui->slider_levels->value();
    j["mTreeSteps"] = ui->slider_treeSteps->value();
    j["mBranchFactor"] = ui->slBranchFac->value() / 100.0f;
    j["mVMultiplier"] = ui->slVMul->value() / 100.0f;
    j["mClumpMax"] = ui->slClumpMax->value() / 100.0f;
    j["mClumpMin"] = ui->slClumpMin->value() / 100.0f;
    j["mClimbRate"] = ui->slClumbRate->value() / 100.0f;
    j["mTwigScale"] = ui->slTwigScale->value() / 100.0f;
    j["mInitialBranchLength"] = ui->slInitBranch->value() / 100.0f;
    j["mLengthFalloffFactor"] = ui->slLenFallFac->value() / 100.0f;
    j["mLengthFalloffPower"] = ui->slLenFallPow->value()/ 100.0f;
    j["mDropAmount"] = ui->slDrop->value() / 100.0f;
    j["mGrowAmount"] = ui->slGrow->value() / 100.0f;
    j["mSweepAmount"] = ui->slSweep->value() / 100.0f;
    j["mMaxRadius"] = ui->slMaxRad->value() / 100.0f;
    j["mTrunkKink"] = ui->slTrunkKink->value() / 1000.0f;
    j["mTaperRate"] = ui->slTaper->value() / 100.0f;
    j["mRadiusFalloffRate"] = ui->slRadFall->value() / 100.0f;
    j["mTwistRate"] = ui->slTwistRate->value() / 100.0f;
    j["mTrunkLength"] = ui->slTrunkLen->value() / 10.0f;
    j["mSeason"] = ui->slSeason->value();

    setPropsAtIndex(ui->listWidget->currentRow(), j);
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
            setPropsAtIndex(ui->listWidget->currentRow(), j);
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
    float VMul = j["mVMultiplier"],
          TwigScale = j["mTwigScale"],
          InitBranch = j["mInitialBranchLength"],
          LenFallFac = j["mLengthFalloffFactor"],
          LenFallPow = j["mLengthFalloffPower"],
          ClumpMax = j["mClumpMax"],
          ClumpMin = j["mClumpMin"],
          BranchFac = j["mBranchFactor"],
          Drop = j["mDropAmount"],
          Grow = j["mGrowAmount"],
          Sweep = j["mSweepAmount"],
          MaxRad = j["mMaxRadius"],
          ClimbRate = j["mClimbRate"],
          TrunkKink = j["mTrunkKink"],
          Taper = j["mTaperRate"],
          RadFall = j["mRadiusFalloffRate"],
          TwistRate = j["mTwistRate"],
          TrunkLen = j["mTrunkLength"];

    VMul *= 100;
    TwigScale *= 100;
    InitBranch *= 100;
    LenFallFac *= 100;
    LenFallPow *= 100;
    ClumpMax *= 100;
    ClumpMin *= 100;
    BranchFac *= 100;
    Drop *= 100;
    Grow *= 100;
    Sweep *= 100;
    MaxRad *= 100;
    ClimbRate *= 100;
    TrunkKink *= 1000;
    Taper *= 100;
    RadFall *= 100;
    TwistRate *= 100;
    TrunkLen *= 10;

    ui->slider_seed->setValue(j["mSeed"]);
    ui->slider_levels->setValue(j["mLevels"]);
    ui->slider_treeSteps->setValue(j["mTreeSteps"]);
    ui->slVMul->setValue(VMul);
    ui->slTwigScale->setValue(TwigScale);
    ui->slInitBranch->setValue(InitBranch);
    ui->slLenFallFac->setValue(LenFallFac);
    ui->slLenFallPow->setValue(LenFallPow);
    ui->slClumpMax->setValue(ClumpMax);
    ui->slClumpMin->setValue(ClumpMin);
    ui->slBranchFac->setValue(BranchFac);
    ui->slDrop->setValue(Drop);
    ui->slGrow->setValue(Grow);
    ui->slSweep->setValue(Sweep);
    ui->slMaxRad->setValue(MaxRad);
    ui->slClumbRate->setValue(ClimbRate);
    ui->slTrunkKink->setValue(TrunkKink);
    ui->slTaper->setValue(Taper);
    ui->slRadFall->setValue(RadFall);
    ui->slTwistRate->setValue(TwistRate);
    ui->slTrunkLen->setValue(TrunkLen);

    ui->slSeason->setValue(0);
}

void MainWindow::growTree()
{
    for(QSlider * slider : sliders)
    {
        slider->setEnabled(false);
    }
    ui->slSeason->setEnabled(false);
    ui->buttonGrow->setEnabled(false);

    for(int i = 0; i <= 100; i++)
    {
        for(Tree* tree : trees)
            tree->growTree(i);
        delay(1);
    }

    ui->buttonGrow->setEnabled(true);
    for(QSlider * slider : sliders)
    {
        slider->setEnabled(true);
    }
    ui->slSeason->setEnabled(true);
}

void MainWindow::changeGrowthTree(int procent)
{
    int seasonLvl = 0;

    if(ui->cbGrowAllTrees->isChecked()) {
        for(Tree* tree : trees)
            tree->growTree(procent);
    }
    else
        trees[ui->listWidget->currentRow()]->growTree(procent);

    j["mSeason"] = seasonLvl;

    if(ui->cbSeason->isChecked())
        openGlWidget->loadSeasonValue(seasonLvl);
}

void MainWindow::loadTexWindow()
{
    changeTexWindow *dialog = new changeTexWindow(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete){
        deleteTree();
    }
    openGlWidget->keys.insert(event->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    openGlWidget->keys.erase(event->key());
}

void MainWindow::delay(int miliseconds)
{
    QTime dieTime = QTime::currentTime().addMSecs(miliseconds);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::setPropsAtIndex(int index, json j)
{
    trees[index]->setProps(j);
    trees[index]->generate();
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

void MainWindow::on_slSeason_valueChanged(int value)
{
    openGlWidget->loadSeasonValue(value);
}


void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    if(currentRow < 0)
        return;
    j.update(trees[currentRow]->getProps());
    set_sliders();
}

void MainWindow::deleteTree()
{
    int index = ui->listWidget->currentRow();
    if(index < 0)
        return;

    trees.erase(trees.begin() + index);
    delete ui->listWidget->takeItem(index);

    if(ui->listWidget->count() == 0)
    {
        //block all widgets
    }
    else if(ui->listWidget->count() == 1)
    {
        ui->listWidget->setCurrentRow(0);
    }
}

