#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QVector>
#include "Note.h"

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    bool openDatabase();
    QVector<Note> loadNotes();
    bool addNote(const Note &note);
    bool updateNote(const Note &note);
    bool deleteNote(const QString &id);

private:
    bool createTables();

    QSqlDatabase m_database;
    QString m_connectionName;
};
