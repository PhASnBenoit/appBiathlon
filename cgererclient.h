#ifndef CGERERCLIENT_H
#define CGERERCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QTimer>
#include "czdc.h"
#include "cbdd.h"

typedef enum couleur {
    ROUGE,// =0
    VERT// =1
} T_COULEUR;

class CGererClient : public QObject
{
    Q_OBJECT
public:
    explicit CGererClient(QObject *parent = nullptr, QTcpSocket *sockClient = nullptr);
    ~CGererClient();
    void gererPistolet(QString &message);
    void gererCible(QString &message);
    void setCible(T_COULEUR couleur, quint8 z);
    QByteArray _couleurByte;

private:
    QTcpSocket *_sockClient;
    bool verifier(QString &message);
    bool decodeMessage(QString &message);
    void placementTableauQMap(QChar x, QChar y);
    void envoiTrameToCible(QChar &reponse, int numeroLigne);
    QChar _type;
    QChar _numero;
    QChar _reponse;
    CZdc *_zdc;
    CBdd *_bdd;
    int _nbTirs;
    int _tirReussi;
    int _numeroSequence;
    QString _trameBdd;

public slots:
    void on_readyRead();
    void on_raz();
    void initBdd(); // PhA

signals:

};

#endif // CGERERCLIENT_H

