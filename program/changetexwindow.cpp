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

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(sendTexturePath()));
}

changeTexWindow::~changeTexWindow()
{
    delete ui;
}

void changeTexWindow::LoadPaths()
{
    pathLinks.append(":/pics/tree1.jpg");
    pathLinks.append(":/pics/twig1.png");
    pathLinks.append(":/pics/treetex.png");

    for (int i = 0; i < pathLinks.length(); i++) {
        ui->listWidget->addItem(new QListWidgetItem(QIcon(pathLinks[i]), QString::number(i), ui->listWidget) );
    }
}

void changeTexWindow::sendTexturePath()
{
    mainWin->setTexture(pathLinks[ui->listWidget->currentRow()]);
}
