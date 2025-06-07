#ifndef MAINUI_H
#define MAINUI_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QTimer>
#include <QByteArray>
#include "cbdd.h"
#include "czdc.h"
#include "cserveurtcp.h"

namespace Ui {
class MainUI;
}

class MainUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainUI(QWidget *parent = nullptr);
    ~MainUI();

private slots:
    void on_timeout();

private:
    Ui::MainUI *ui;
    QTimer _tmr;
    CBdd _bdd;
    CZdc *_zdc;

signals:
    void sig_raz();

};

#endif // MAINUI_H
