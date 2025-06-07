#ifndef CBDD_H
#define CBDD_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>
#include <QDebug>
#include <QThread>

class CBdd : public QObject
{
    Q_OBJECT

public:
    explicit CBdd(QObject *parent = nullptr);
    ~CBdd();

    bool connexionBdd();
    void fermerConnexion();

    QString lirePenalite(int idRace, int numeroPenalite);

    // Gestion de activity
    //bool majEtatActivite(int idActivity, int newState);
    int lireEtatActivite();

    // Gestion de config
    bool insererConfig(int id, int code, const QString &token, const QString &nomCourse, int nbJuges, int maxJuges);
    QVariantMap lireConfig(int id);

    // Gestion de race
    bool insererRace(int id, int num, const QString &runner, const QString &judge, const QString &token);
    bool mettreAJourTemps(int idRace, int tIndex, int temps);
    bool mettreAJourPenalite(int y, int numeroSeq, const QString &valeur);
    QVariantMap lireRace(int idRace);
    bool razPenalite();

private:
    QSqlDatabase _db;
};

#endif // CBDD_H

