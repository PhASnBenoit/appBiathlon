#include "cgererclient.h"

/* DOCUMENTATION TRAME PISTOLET :
 *    @XYZ% : X=1 Y = NUMÉRO = [1;8], Z = Tir envoyé
 DOCUMENTATION TRAME CIBLE :
 *    @XYZ% : X=0 Y = NUMÉRO = [1;8], Z = 1 Tir réussi
*/

CGererClient::CGererClient(QObject *parent, QTcpSocket *sockClient)
    : QObject{parent}
{
    _tirReussi = 0;
    _nbTirs = 0;
    _numeroSequence = 1;
    _sockClient = sockClient;
    _zdc = new CZdc(this, sizeof(T_ZDC));
    _zdc->attacherSeulement();
    connect(sockClient, &QTcpSocket::readyRead,this,&CGererClient::on_readyRead);
}

CGererClient::~CGererClient()
{
  _bdd->fermerConnexion();
  delete _bdd;
  delete _zdc;
}

void CGererClient::on_readyRead(){
    QString message = _sockClient->readAll();
    //numero = message.at(2);
    bool res = verifier(message);
    if (res) {
        qDebug()<<"George, la trame" << message << " est au bon format !";
        decodeMessage(message);
    }
}

void CGererClient::on_raz()
{
    _nbTirs = 0;
    _tirReussi = 0;
    _numeroSequence = 1;
    _bdd->razPenalite();
}

void CGererClient::gererPistolet(QString &message){

    quint8 y = message.at(2).digitValue();
    quint8 z = message.at(3).digitValue();
    CGererClient *cible = (CGererClient *)_zdc->lireAdrCible(y);
    int temps = 8000;
    QThread::msleep(temps);
    _nbTirs++;
    if (_nbTirs == 6){
        if (_numeroSequence == 3){
            _numeroSequence = 0;
        }
        _nbTirs = 1;
        _numeroSequence++;
        _tirReussi = 0;
    }

    bool res = _zdc->getCibleAtteinte(y);
    int state = _zdc->getState();
    if (res){
        if (state == 3){
            ++_tirReussi;
            _trameBdd = QString::number(_tirReussi) +"/"+ QString::number(_nbTirs);
            _bdd->mettreAJourPenalite(y ,_numeroSequence ,_trameBdd);
            _zdc->setCibleToZero(y);
            cible->setCible(VERT,z);
        }
    }else{
        if (state == 3){
            _trameBdd = QString::number(_tirReussi) +"/"+ QString::number(_nbTirs);
            _bdd->mettreAJourPenalite(y, _numeroSequence, _trameBdd); //y à la place de idRace
            cible->setCible(ROUGE,z);
            qDebug()<<"Le temps d'attente est dépassé";
        }
    }
}

void CGererClient::gererCible(QString &message){
    quint8 y = message.at(2).digitValue();
    _zdc->setCibleAtteinte(y, true);
    qDebug()<<"Trame de la cible reçu n°" << y;
}

bool CGererClient::verifier(QString &message)
{
    bool res=false;
    QChar x = message.at(1);
    QChar y = message.at(2);
    QChar z = message.at(3);
    message = message.left(5);
    if (message.startsWith("@") && message.endsWith("%"))
        if ( (x=='0') || (x=='1'))
            if ( (y>='1') && (y<='8') )
                if ( (z>='0') && (z<='5') )
                    res = true;
    return res;
}

bool CGererClient::decodeMessage(QString &message)
{
    quint8 x = message.at(1).digitValue();
    quint8 y = message.at(2).digitValue();
    quint8 z = message.at(3).digitValue();

    switch(x) {
    case 0: // cible
        if (z==0) { // trame ident
            _zdc->setAdrCible(y, this);
           // qDebug()<<"La cible de n°" << y << " a été ajouté dans la liste";
        } else {
            gererCible(message);
        } // else
        return true;
    case 1: // pistolet00
        if(z == 0){//tir envoyé
            gererPistolet(message);
            return true;
        }
        // normal qu'il n'y ait pas de break
    default:
        qDebug()<<"CGererClient::La trame envoyée n'est pas conforme";
        return false;
    } // sw
}

void CGererClient::setCible(T_COULEUR couleur, quint8 z)
{
    //_sockClient->write(couleur, z);
        _couleurByte.clear();
        _couleurByte.append(couleur+0x30);
        _couleurByte.append(z+0x30);
        _sockClient->write(_couleurByte);
        if (couleur == ROUGE){
            qDebug()<<"La couleur est Rouge, tir manqué :(";
        }else
            qDebug()<<"La couleur est Verte, tir réussi";
}

// PhA -----------------
void CGererClient::initBdd()
{
    _bdd = new CBdd();
    _bdd->connexionBdd();
}
//----------------------
