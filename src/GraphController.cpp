#include "GraphController.h"

#include <QUuid>

GraphController::GraphController(QObject *parent)
    : QObject(parent)
{
    if (m_databaseManager.openDatabase()) {
        reloadData();
    }
}

void GraphController::reloadData()
{
    QVector<Note> notes = m_databaseManager.loadNotes();
    QVector<Edge> rawEdges = m_databaseManager.loadEdges();

    m_notesModel.setNotes(notes);
    m_edgesModel.setEdges(buildDrawableEdges(rawEdges, notes));
}

QVector<Edge> GraphController::buildDrawableEdges(const QVector<Edge> &rawEdges, const QVector<Note> &notes)
{
    QVector<Edge> drawableEdges;

    for (const Edge &edge : rawEdges) {
        bool fromFound = false;
        bool toFound = false;

        Note fromNote;
        Note toNote;

        for (const Note &note : notes) {
            if (note.id == edge.fromNoteId) {
                fromNote = note;
                fromFound = true;
            }

            if (note.id == edge.toNoteId) {
                toNote = note;
                toFound = true;
            }
        }

        if (!fromFound || !toFound) {
            continue;
        }

        Edge drawableEdge = edge;

        drawableEdge.fromX = fromNote.x + 70;
        drawableEdge.fromY = fromNote.y + 36;
        drawableEdge.toX = toNote.x + 70;
        drawableEdge.toY = toNote.y + 36;

        drawableEdges.append(drawableEdge);
    }

    return drawableEdges;
}

void GraphController::createEdge(const QString &fromNoteId, const QString &toNoteId)
{
    if (m_databaseManager.addEdge(fromNoteId, toNoteId)) {
        reloadData();
    }
}

NotesModel *GraphController::notesModel()
{
    return &m_notesModel;
}

EdgesModel *GraphController::edgesModel()
{
    return &m_edgesModel;
}

void GraphController::createNote(const QString &title)
{
    if (title.trimmed().isEmpty()) {
        return;
    }

    Note note;
    note.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    note.title = title.trimmed();
    note.content = "";
    note.x = 100;
    note.y = 100;

    if (m_databaseManager.addNote(note)) {
        reloadData();
    }
}

void GraphController::updateNote(const QString &id, const QString &title, const QString &content)
{
    if (title.trimmed().isEmpty()) {
        return;
    }

    Note note;
    note.id = id;
    note.title = title.trimmed();
    note.content = content;

    if (m_databaseManager.updateNote(note)) {
        reloadData();
    }
}

void GraphController::updateNotePosition(const QString &id, double x, double y)
{
    if (m_databaseManager.updateNotePosition(id, x, y)) {
        reloadData();
    }
}

void GraphController::deleteNote(const QString &id)
{
    m_databaseManager.deleteEdgesForNote(id);

    if (m_databaseManager.deleteNote(id)) {
        reloadData();
    }
}

void GraphController::createLinkedNote(const QString &fromNoteId, const QString &title)
{
    if (fromNoteId.isEmpty() || title.trimmed().isEmpty()) {
        return;
    }

    QVector<Note> notes = m_databaseManager.loadNotes();

    Note parentNote;
    bool parentFound = false;

    for (const Note &note : notes) {
        if (note.id == fromNoteId) {
            parentNote = note;
            parentFound = true;
            break;
        }
    }

    if (!parentFound) {
        return;
    }

    Note newNote;
    newNote.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    newNote.title = title.trimmed();
    newNote.content = "";

    newNote.x = parentNote.x + 180;
    newNote.y = parentNote.y + 100;

    if (m_databaseManager.addNote(newNote)) {
        m_databaseManager.addEdge(fromNoteId, newNote.id);
        reloadData();
    }
}
