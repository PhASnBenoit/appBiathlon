#include "cserveurtcp.h"
QString CServeurTcp::message;

CServeurTcp::CServeurTcp(QObject *parent)
    : QObject{parent}
{
    //_bdd.ouvrirConnexion();
    _serveur = new QTcpServer();
    connect(_serveur,&QTcpServer::newConnection,this,&CServeurTcp::on_newConnection);
    _serveur->listen(QHostAddress::Any, 2222);
}

CServeurTcp::~CServeurTcp()
{
  for (int i=0 ; i<_listeThreadClients.size() ; i++) {
      //delete _listeClients.at(i);
      delete _listeThreadClients.at(i);
  } // for
  _listeClients.clear();
  _listeThreadClients.clear();
  //_bdd.fermerConnexion();
  delete _serveur;
}

void CServeurTcp::on_newConnection(){
    //char trame[5];
    QTcpSocket *sockClient = _serveur->nextPendingConnection();
    connect(sockClient, &QTcpSocket::disconnected,this, &CServeurTcp::on_disconnected);
    CGererClient *client = new CGererClient(nullptr, sockClient);
    QThread *th = new QThread();
    client->moveToThread(th);
    connect(this, &CServeurTcp::sig_raz, client, &CGererClient::on_raz);
    connect(th, &QThread::started, client, &CGererClient::initBdd); // PhA
    connect(th, &QThread::finished, client, &QObject::deleteLater); // PhA
    _listeThreadClients.append(th);
    th->start();
    qDebug()<<"CServeurTcp::on_newConnection Un client s'est connecté";
    qDebug()<<_listeClients;
    qDebug()<<_listeThreadClients;
}

void CServeurTcp::on_disconnected(){
    QTcpSocket *sock = (QTcpSocket *)sender();
    sock->deleteLater();
    qDebug()<<"Le client s'est déconnecté";
}

void CServeurTcp::on_raz()
{
  emit sig_raz();
}

