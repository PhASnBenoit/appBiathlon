#ifndef CAPP_H
#define CAPP_H


#include <QObject>
#include "mainui.h"
#include "cserveurtcp.h"
#include "czdc.h"


class CApp : public QObject
{
    Q_OBJECT
public:
    explicit CApp(QObject *parent = nullptr);
    ~CApp();

private:
    MainUI _gui;
    CServeurTcp _srv;
    CZdc *_zdc;

signals:

};

#endif // CAPP_H
