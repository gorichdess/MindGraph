#pragma once

#include <QAbstractListModel>
#include <QVector>
#include "Note.h"

class NotesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum NoteRoles {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        ContentRole,
        XRole,
        YRole,
        ColorRole
    };

    explicit NotesModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setNotes(const QVector<Note> &notes);
    void addNote(const Note &note);
    void updateNote(const Note &note);
    void removeNote(const QString &id);
    Note noteById(const QString &id) const;

private:
    QVector<Note> m_notes;
};
