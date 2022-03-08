#include "changetexwindow.h"
#include "ui_changetexwindow.h"
#include <QListWidgetItem>
#include <QDir>
#include "widget.h"

changeTexWindow::changeTexWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changeTexWindow)
{
    ui->setupUi(this);
}

changeTexWindow::changeTexWindow(MainWindow *qMain) : ui(new Ui::changeTexWindow)
{
    ui->setupUi(this);

    if(LoadPaths("textures/trees", ui->listWidget, woodPaths))
        ui->lbNoWood->setVisible(false);
    else
        ui->listWidget->setVisible(false);

    if(LoadPaths("textures/twigs", ui->listWidget_2, twigPaths))
        ui->lbNoTwig->setVisible(false);
    else
        ui->listWidget_2->setVisible(false);

    mainWin = qMain;
    mainWin->btTextureEnabled(false);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(sendTexturePath()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(closeDialog()));
}

changeTexWindow::~changeTexWindow()
{
    closeDialog();
}

bool changeTexWindow::LoadPaths(QString path, QListWidget* widget, QStringList& filePaths)
{
    QDir directory(path);
    QStringList formats;
    formats.append("*.png");
    formats.append("*.jpg");
    formats.append("*.PNG");
    formats.append("*.JPG");

    filePaths =
            directory.entryList(formats,QDir::Files);

    if(filePaths.length() == 0)
        return false;

    QSize sizeOfIcon(100,100);

    for (int i = 0; i < filePaths.length(); i++) {
        filePaths[i] = path + "/" + filePaths[i];
        QIcon icon(filePaths[i]);
        widget->addItem(new QListWidgetItem(icon, "", widget ));
    }
    widget->setIconSize(sizeOfIcon);
    widget->setFlow(QListView::LeftToRight);
    widget->setCurrentRow(0);
    return true;
}

void changeTexWindow::sendTexturePath()
{
    if(woodPaths.length() > 0)
        mainWin->setTexture(woodPaths[ui->listWidget->currentRow()], TextureType::WOOD);
    if(twigPaths.length() > 0)
        mainWin->setTexture(twigPaths[ui->listWidget_2->currentRow()], TextureType::TWIG);
    closeDialog();
}

void changeTexWindow::closeDialog()
{
    mainWin->btTextureEnabled(true);
    this->close();
    delete ui;
}
