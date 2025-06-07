#ifndef CZDC_H
#define CZDC_H
#define KEY "Biathlon"

#include <QSharedMemory>
#include <QDebug>
#include <QTcpSocket>

// définition de la mémoire partagée
typedef struct {
    bool cibleAtteinte;
    void *adrCible;
} T_CIBLE;

typedef struct {
    T_CIBLE cible[8];
    int state;
} T_ZDC;


class CZdc : public QSharedMemory
{
    Q_OBJECT

public:
    explicit CZdc(QObject *parent = 0, int size = 0);
    ~CZdc();
    int attacherOuCreer();
    int attacherSeulement();
    void effacerTout();
    void setAdrCible(int num, void *adr);
    bool getCibleAtteinte(int num);
    void setCibleAtteinte(int num, bool state);
    void *lireAdrCible(int y);
    void setCibleToZero(int num);
    void setState(int state);
    int getState();

private:
    int _taille;
    T_ZDC *_adrBase;
    QObject *_parent;

signals:
    void sig_erreur(QString mess);

public slots:

};

#endif // CZDC_H
