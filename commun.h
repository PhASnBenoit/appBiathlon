#ifndef COMMUN_H
#define COMMUN_H
#include <QTcpSocket>

typedef struct s_pistolet {
    QTcpSocket *sock;
    int num;
} T_PISTOLET;

typedef struct s_cible {
    QTcpSocket *sock;
    int num;
} T_CIBLE;


#endif // COMMUN_H
