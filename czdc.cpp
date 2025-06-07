#include "czdc.h"


CZdc::CZdc(QObject *parent, int size) :
    QSharedMemory(parent)
{
    _parent = parent;
    setKey(KEY);
    _taille = size;
    _adrBase = NULL;
    qDebug() << "Objet CSharedMemory créé par " << _parent->thread();
}

CZdc::~CZdc()
{
    detach();
    qDebug() << "Objet CSharedMemory détruit par " << _parent->thread();
}

int CZdc::attacherOuCreer()
{
    int res;

    attach();   // tentative de s'attacher
    if (!isAttached()) {   // si existe pas alors création
         res = create(_taille);   // on réserve la place
         if (!res) {
              QString mess="CSharedMemory::attacherOuCreer Erreur de création de la mémoire partagée.";
         } // if res
         attach();
    } // if isattached
    _adrBase = (T_ZDC *)data();
    return 0;
}

int CZdc::attacherSeulement()
{
    attach();   // tentative de s'attacher
    if (!isAttached()) {   // si existe pas
        QString mess="CSharedMemory::attacherSeulement Erreur de création de la mémoire partagée.";
    } // if isattached
    _adrBase = (T_ZDC *)data();
    return 0;
}

void CZdc::setAdrCible(int num, void *adr)
{
    lock();
       _adrBase->cible[num-1].adrCible = adr;
    unlock();
}

bool CZdc::getCibleAtteinte(int num)
{
    lock();
       bool res = _adrBase->cible[num-1].cibleAtteinte;
    unlock();
    return res;
}

void CZdc::setCibleAtteinte(int num, bool state)
{
    lock();
       _adrBase->cible[num-1].cibleAtteinte = state; // ??-----??
    unlock();
}

void *CZdc::lireAdrCible(int y)
{
    if ( (y<1) || (y>8) ) {
        QString mess="CSharedMemory::lire ERREUR, numéro incorrect.";
    } // if no
    if (!isAttached()) {   // si existe pas alors création
        QString mess="CSharedMemory::lire Erreur mémoire partagée non attachée. ";
    } // if isattached
    lock();
        void *adr = _adrBase->cible[y-1].adrCible;
    unlock();
    return adr;
}

void CZdc::setCibleToZero(int num)
{
    lock();
       _adrBase->cible[num-1].cibleAtteinte = false;
       unlock();
}


int CZdc::getState()
{
    lock();
       int state = _adrBase->state;
    unlock();
    return state;
}

void CZdc::setState(int state)
{
    lock();
       _adrBase->state = state;
    unlock();
}

void CZdc::effacerTout()
{
  lock();
    memset(_adrBase, 0, 8 * sizeof(T_ZDC));
  unlock();
}
