#pragma once

#include <QObject>
#include "DatabaseManager.h"
#include "NotesModel.h"
#include "EdgesModel.h"

class GraphController : public QObject
{
    Q_OBJECT

public:
    explicit GraphController(QObject *parent = nullptr);

    NotesModel *notesModel();
    EdgesModel *edgesModel();

    Q_INVOKABLE void createNote(const QString &title);
    Q_INVOKABLE void updateNote(const QString &id, const QString &title, const QString &content);
    Q_INVOKABLE void updateNotePosition(const QString &id, double x, double y);
    Q_INVOKABLE void deleteNote(const QString &id);
    Q_INVOKABLE void createEdge(const QString &fromNoteId, const QString &toNoteId);
    Q_INVOKABLE void createLinkedNote(const QString &fromNoteId, const QString &title);


private:
    DatabaseManager m_databaseManager;
    NotesModel m_notesModel;
    EdgesModel m_edgesModel;

    void reloadData();
    QVector<Edge> buildDrawableEdges(const QVector<Edge> &rawEdges, const QVector<Note> &notes);
};
