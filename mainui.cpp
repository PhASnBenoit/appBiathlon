#include "mainui.h"
#include "ui_mainui.h"


MainUI::MainUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainUI)
{
    ui->setupUi(this);
    _zdc = new CZdc(this, sizeof(T_ZDC));
    _zdc->attacherOuCreer();
    _bdd.connexionBdd();
    connect(&_tmr, &QTimer::timeout, this, &MainUI::on_timeout);
    _tmr.start(2000);
}

MainUI::~MainUI()
{
    _bdd.fermerConnexion();
    delete _zdc;
    delete ui;
}

void MainUI::on_timeout()
{
    int state = _bdd.lireEtatActivite();
    _zdc->setState(state);
    if (state != 3)
        emit sig_raz();
}

