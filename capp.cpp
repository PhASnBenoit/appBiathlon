#include "capp.h"

CApp::CApp(QObject *parent)
    : QObject{parent}
{
    _gui.show();
    connect(&_gui, &MainUI::sig_raz, &_srv, &CServeurTcp::on_raz);
    _zdc = new CZdc(this, sizeof(T_ZDC));
    _zdc->attacherOuCreer();
    _zdc->effacerTout();
}

CApp::~CApp()
{
    delete _zdc;
}
