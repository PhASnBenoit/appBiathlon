#ifndef CSERVEURTCP_H
#define CSERVEURTCP_H

#include <QObject>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QThread>
#include "cgererclient.h"
//#include "cbdd.h"

class CServeurTcp : public QObject
{
    Q_OBJECT
public:
    explicit CServeurTcp(QObject *parent = nullptr);
    ~CServeurTcp();
    static QString message;

private:
    QTcpServer *_serveur;
    QList<QThread *> _listeThreadClients;
    QList<CGererClient *> _listeClients;
//    CBdd _bdd;

private slots:
    void on_newConnection();
    void on_disconnected();

public slots:
    void on_raz();

signals:
    void sig_raz();
};

#endif // CSERVEURTCP_H
