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

    LoadPaths("textures/trees", ui->listWidget);
    // TODO: add info that there are no textures
    //    "No textures found in /textures/trees/"
    //LoadPaths("textures/twigs", ui->listWidget_2);
    mainWin = qMain;
    mainWin->btTextureEnabled(false);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(sendTexturePath()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(closeDialog()));
}

changeTexWindow::~changeTexWindow()
{
    closeDialog();
}

bool changeTexWindow::LoadPaths(QString path, QListWidget* widget)
{
    QDir directory(path);
    QStringList formats;
    formats.append("*.png");
    formats.append("*.jpg");
    formats.append("*.PNG");
    formats.append("*.JPG");

    pathLinks =
            directory.entryList(formats,QDir::Files);

    if(pathLinks.length() == 0)
        return false;

    QSize sizeOfIcon(100,100);

    for (int i = 0; i < pathLinks.length(); i++) {
        pathLinks[i] = path + "/" + pathLinks[i];
        QIcon icon(pathLinks[i]);
        widget->addItem(new QListWidgetItem(icon, "", widget ));
    }
    widget->setIconSize(sizeOfIcon);
    widget->setFlow(QListView::LeftToRight);
    widget->setCurrentRow(0);
    return true;
}

void changeTexWindow::sendTexturePath()
{
    mainWin->setTexture(pathLinks[ui->listWidget->currentRow()]);
    closeDialog();
}

void changeTexWindow::closeDialog()
{
    mainWin->btTextureEnabled(true);
    this->close();
    delete ui;
}
