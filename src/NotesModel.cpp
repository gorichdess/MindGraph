#include "NotesModel.h"

NotesModel::NotesModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int NotesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_notes.size();
}

QVariant NotesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_notes.size()) {
        return {};
    }

    const Note &note = m_notes.at(index.row());

    switch (role) {
    case IdRole:
        return note.id;
    case TitleRole:
        return note.title;
    case ContentRole:
        return note.content;
    case XRole:
        return note.x;
    case YRole:
        return note.y;
    case ColorRole:
        return note.color;
    default:
        return {};
    }
}

QHash<int, QByteArray> NotesModel::roleNames() const
{
    return {
        { IdRole, "noteId" },
        { TitleRole, "title" },
        { ContentRole, "content" },
        { XRole, "noteX" },
        { YRole, "noteY" },
        { ColorRole, "color" }
    };
}

void NotesModel::setNotes(const QVector<Note> &notes)
{
    beginResetModel();
    m_notes = notes;
    endResetModel();
}

void NotesModel::addNote(const Note &note)
{
    beginInsertRows(QModelIndex(), m_notes.size(), m_notes.size());
    m_notes.append(note);
    endInsertRows();
}

void NotesModel::updateNote(const Note &note)
{
    for (int i = 0; i < m_notes.size(); ++i) {
        if (m_notes[i].id == note.id) {
            m_notes[i] = note;
            emit dataChanged(index(i), index(i));
            return;
        }
    }
}

void NotesModel::removeNote(const QString &id)
{
    for (int i = 0; i < m_notes.size(); ++i) {
        if (m_notes[i].id == id) {
            beginRemoveRows(QModelIndex(), i, i);
            m_notes.removeAt(i);
            endRemoveRows();
            return;
        }
    }
}

Note NotesModel::noteById(const QString &id) const
{
    for (const Note &note : m_notes) {
        if (note.id == id) {
            return note;
        }
    }

    return {};
}
