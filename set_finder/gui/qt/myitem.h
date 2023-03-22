#ifndef MYITEM_H
#define MYITEM_H

#include <QObject>

class MyItem : public QObject
{
    Q_OBJECT

public:
    explicit MyItem(QObject *parent = nullptr);

    void doNothing(){}


private:
};

#endif // MYITEM_H
