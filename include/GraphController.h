#pragma once

#include <QObject>
#include "DatabaseManager.h"
#include "NotesModel.h"

class GraphController : public QObject
{
    Q_OBJECT

public:
    explicit GraphController(QObject *parent = nullptr);

    NotesModel *notesModel();

    Q_INVOKABLE void createNote(const QString &title);
    Q_INVOKABLE void updateNote(const QString &id, const QString &title, const QString &content);
    Q_INVOKABLE void updateNotePosition(const QString &id, double x, double y);
    Q_INVOKABLE void deleteNote(const QString &id);

private:
    DatabaseManager m_databaseManager;
    NotesModel m_notesModel;
};
