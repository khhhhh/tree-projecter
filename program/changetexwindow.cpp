#include "changetexwindow.h"
#include "ui_changetexwindow.h"
#include <QListWidgetItem>
#include <iostream>

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
    for(int i = 1; i < 6; i++)
        pathLinks.append(":/textures/trees/" + QString::number(i) + ".jpg");

    QSize sizeOfIcon(100,100);
    for (int i = 0; i < pathLinks.length(); i++) {
        QIcon *icon = new QIcon(pathLinks[i]);
        icon->actualSize(sizeOfIcon);
        ui->listWidget->addItem(new QListWidgetItem(*icon, "Tree " + QString::number(i), ui->listWidget) );
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
