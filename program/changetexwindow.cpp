#include "changetexwindow.h"
#include "ui_changetexwindow.h"
#include <QListWidgetItem>
#include <iostream>
#include <QDir>

changeTexWindow::changeTexWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changeTexWindow)
{
    ui->setupUi(this);
}

changeTexWindow::changeTexWindow(MainWindow *qMain) : ui(new Ui::changeTexWindow)
{
    ui->setupUi(this);

    LoadPaths();

    mainWin = qMain;
    mainWin->btTextureEnabled(false);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(sendTexturePath()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(cancelTex()));
}

changeTexWindow::~changeTexWindow()
{
    mainWin->btTextureEnabled(true);
    delete ui;
}

void changeTexWindow::LoadPaths()
{
    QDir directory("textures/trees");
    QStringList images =
            directory.entryList(QStringList() << "*",QDir::Files);


    QSize sizeOfIcon(100,100);

    for (int i = 0; i < images.length(); i++) {
        QIcon icon("textures/trees/" + images[i]);
        ui->listWidget->addItem(new QListWidgetItem(icon, "", ui->listWidget) );
    }
    ui->listWidget->setIconSize(sizeOfIcon);
    ui->listWidget->setFlow(QListView::LeftToRight);
}

void changeTexWindow::sendTexturePath()
{
    mainWin->setTexture(pathLinks[ui->listWidget->currentRow()]);
    cancelTex();
}

void changeTexWindow::cancelTex()
{
    mainWin->btTextureEnabled(true);
    this->close();
    delete ui;
}
