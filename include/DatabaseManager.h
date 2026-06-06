#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QVector>
#include "Note.h"
#include "Edge.h"

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
    bool updateNotePosition(const QString &id, double x, double y);

    QVector<Edge> loadEdges();
    bool addEdge(const QString &fromNoteId, const QString &toNoteId);
    bool edgeExists(const QString &fromNoteId, const QString &toNoteId);
    bool deleteEdgesForNote(const QString &noteId);

private:
    bool createTables();

    QSqlDatabase m_database;
    QString m_connectionName;
};
