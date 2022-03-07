#include "changetexwindow.h"
#include "ui_changetexwindow.h"
#include <QListWidgetItem>
#include <iostream>
#include <QDir>
#include <QMessageBox>

changeTexWindow::changeTexWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changeTexWindow)
{
    ui->setupUi(this);
}

changeTexWindow::changeTexWindow(MainWindow *qMain) : ui(new Ui::changeTexWindow)
{
    ui->setupUi(this);

    if(!LoadPaths("textures/trees", ui->listWidget))
    {
        QMessageBox box;
        box.setText("No textures found in /textures/trees/");
        box.show();
        this->close();
    }
    //LoadPaths("textures/twigs", ui->listWidget_2);
    mainWin = qMain;
    mainWin->btTextureEnabled(false);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(sendTexturePath()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(closeDialog()));
}

changeTexWindow::~changeTexWindow()
{
    mainWin->btTextureEnabled(true);
    delete ui;
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
    return true;
}

void changeTexWindow::sendTexturePath()
{
    if(ui->listWidget->currentRow() != NULL)
        mainWin->setTexture(pathLinks[ui->listWidget->currentRow()]);
    closeDialog();
}

void changeTexWindow::closeDialog()
{
    mainWin->btTextureEnabled(true);
    this->close();
    delete ui;
}
