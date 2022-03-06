#ifndef QIMAGEITEMMODEL_H
#define QIMAGEITEMMODEL_H

#include <QAbstractItemModel>

class QImageItemModel : public QAbstractItemModel
{
public:
    explicit QImageItemModel(QObject *parent = nullptr);
};

#endif // QIMAGEITEMMODEL_H
