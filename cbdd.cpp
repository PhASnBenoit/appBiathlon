#include "cbdd.h"

CBdd::CBdd(QObject *parent)
    : QObject(parent)
{
    QString connName = QString("mysql_thread_%1").arg(reinterpret_cast<quintptr>(QThread::currentThreadId()));
    _db = QSqlDatabase::addDatabase("QMYSQL",connName);
    _db.setHostName("localhost");
    _db.setDatabaseName("biathlon");
    _db.setUserName("biathlon");
    _db.setPassword("biathlon");
}

CBdd::~CBdd()
{
    fermerConnexion();
}

bool CBdd::connexionBdd()
{
    if (!_db.open()) {
        qCritical() << "Erreur de connexion:" << _db.lastError().text();
        return false;
    }
    return true;
}

void CBdd::fermerConnexion()
{
    if (_db.isOpen()) {
        _db.close();
    }
}

// ----------- TABLE ACTIVITY ------------
/*
bool CBdd::majEtatActivite(int idActivity, int newState)
{
    QSqlQuery query(_db);
    query.prepare("UPDATE activity SET state = :state WHERE id_activity = :id");
    query.bindValue(":state", newState);
    query.bindValue(":id", idActivity);
    return query.exec();
}
*/
int CBdd::lireEtatActivite()
{
    QSqlQuery query(_db);
    query.prepare("SELECT state FROM activity LIMIT 1");
    if (query.exec()) {
        query.next();
        return query.value(0).toInt();
    }
    return -1;
}

// ----------- TABLE CONFIG ------------

bool CBdd::insererConfig(int id, int code, const QString &token, const QString &nomCourse, int nbJuges, int maxJuges)
{
    QSqlQuery query(_db);
    query.prepare(R"(
        INSERT INTO config (id_config, code, token, nom_course, nb_juges, max_juges)
        VALUES (:id, :code, :token, :nom, :nbj, :maxj)
    )");
    query.bindValue(":id", id);
    query.bindValue(":code", code);
    query.bindValue(":token", token);
    query.bindValue(":nom", nomCourse);
    query.bindValue(":nbj", nbJuges);
    query.bindValue(":maxj", maxJuges);
    return query.exec();
}

QVariantMap CBdd::lireConfig(int id)
{
    QVariantMap result;
    QSqlQuery query(_db);
    query.prepare("SELECT * FROM config WHERE id_config = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        for (int i = 0; i < query.record().count(); ++i) {
            result[query.record().fieldName(i)] = query.value(i);
        }
    }
    return result;
}

// ----------- TABLE RACE ------------

bool CBdd::insererRace(int id, int num, const QString &runner, const QString &judge, const QString &token)
{
    QSqlQuery query(_db);
    query.prepare(R"(
        INSERT INTO race (id_race, num, runnerName, judgeName, token)
        VALUES (:id, :num, :runner, :judge, :token)
    )");
    query.bindValue(":id", id);
    query.bindValue(":num", num);
    query.bindValue(":runner", runner);
    query.bindValue(":judge", judge);
    query.bindValue(":token", token);
    return query.exec();
}

bool CBdd::mettreAJourTemps(int idRace, int tIndex, int temps)
{
    if ( (tIndex < 0) || (tIndex > 9) )
        return false;

    QString champ = QString("t%1").arg(tIndex);
    QSqlQuery query(_db);
    query.prepare(QString("UPDATE race SET %1 = :temps WHERE id_race = :id").arg(champ));
    query.bindValue(":temps", temps);
    query.bindValue(":id", idRace);
    return query.exec();
}

QVariantMap CBdd::lireRace(int idRace)
{
    QVariantMap result;
    QSqlQuery query(_db);
    query.prepare("SELECT * FROM race WHERE id_race = :id");
    query.bindValue(":id", idRace);
    if (query.exec() && query.next()) {
        for (int i = 0; i < query.record().count(); ++i) {
            result[query.record().fieldName(i)] = query.value(i);
        }
    }
    return result;
}

bool CBdd::razPenalite()
{
    QSqlQuery query(_db);
    query.prepare(QString("UPDATE race SET seqTirs1 = :valeur, seqTirs2 = :valeur, seqTirs3 = :valeur "));
    query.bindValue(":valeur", "0/0");
    return query.exec();
}

bool CBdd::mettreAJourPenalite(int y, int numeroSeq, const QString &valeur)
{
    if ( (numeroSeq < 1) || (numeroSeq > 3) )
        return false;

    QString champ = QString("seqTirs%1").arg(numeroSeq);

    QSqlQuery query(_db);
    query.prepare(QString("UPDATE race SET %1 = :valeur WHERE num = :id").arg(champ));
    query.bindValue(":valeur", valeur);
    query.bindValue(":id", y);// ??? à la place idRace
    return query.exec();
}
