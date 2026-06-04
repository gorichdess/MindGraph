#include "GraphController.h"

#include <QUuid>

GraphController::GraphController(QObject *parent)
    : QObject(parent)
{
    if (m_databaseManager.openDatabase()) {
        m_notesModel.setNotes(m_databaseManager.loadNotes());
    }
}

NotesModel *GraphController::notesModel()
{
    return &m_notesModel;
}

void GraphController::createNote(const QString &title)
{
    const QString cleanTitle = title.trimmed().isEmpty() ? "New note" : title.trimmed();

    Note note;
    note.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    note.title = cleanTitle;
    note.content = "";
    note.x = 80 + m_notesModel.rowCount() * 30;
    note.y = 80 + m_notesModel.rowCount() * 30;
    note.color = "#8EA7E9";

    if (m_databaseManager.addNote(note)) {
        m_notesModel.addNote(note);
    }
}

void GraphController::updateNote(const QString &id, const QString &title, const QString &content)
{
    Note note = m_notesModel.noteById(id);

    if (note.id.isEmpty()) {
        return;
    }

    note.title = title.trimmed().isEmpty() ? "Untitled" : title.trimmed();
    note.content = content;

    if (m_databaseManager.updateNote(note)) {
        m_notesModel.updateNote(note);
    }
}

void GraphController::updateNotePosition(const QString &id, double x, double y)
{
    Note note = m_notesModel.noteById(id);

    if (note.id.isEmpty()) {
        return;
    }

    note.x = x;
    note.y = y;

    if (m_databaseManager.updateNote(note)) {
        m_notesModel.updateNote(note);
    }
}

void GraphController::deleteNote(const QString &id)
{
    if (m_databaseManager.deleteNote(id)) {
        m_notesModel.removeNote(id);
    }
}
