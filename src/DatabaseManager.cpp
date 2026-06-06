#include "DatabaseManager.h"

#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QUuid>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
    , m_connectionName(QUuid::createUuid().toString(QUuid::WithoutBraces))
{
}

DatabaseManager::~DatabaseManager()
{
    if (m_database.isOpen()) {
        m_database.close();
    }

    QSqlDatabase::removeDatabase(m_connectionName);
}

bool DatabaseManager::openDatabase()
{
    const QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataPath);

    if (!dir.exists()) {
        dir.mkpath(".");
    }

    const QString dbPath = dir.filePath("graph_notes.db");

    m_database = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    m_database.setDatabaseName(dbPath);

    if (!m_database.open()) {
        qWarning() << "Database open error:" << m_database.lastError().text();
        return false;
    }

    return createTables();
}

bool DatabaseManager::createTables()
{
    QSqlQuery notesQuery(m_database);

    const QString notesSql = R"(
        CREATE TABLE IF NOT EXISTS notes (
            id TEXT PRIMARY KEY,
            title TEXT NOT NULL,
            content TEXT,
            x REAL DEFAULT 0,
            y REAL DEFAULT 0,
            color TEXT DEFAULT '#8EA7E9',
            created_at TEXT DEFAULT CURRENT_TIMESTAMP,
            updated_at TEXT DEFAULT CURRENT_TIMESTAMP,
            sync_status TEXT DEFAULT 'pending'
        )
    )";

    if (!notesQuery.exec(notesSql)) {
        qWarning() << "Create notes table error:" << notesQuery.lastError().text();
        return false;
    }

    QSqlQuery edgesQuery(m_database);

    const QString edgesSql = R"(
        CREATE TABLE IF NOT EXISTS edges (
            id TEXT PRIMARY KEY,
            from_note_id TEXT NOT NULL,
            to_note_id TEXT NOT NULL,
            created_at TEXT DEFAULT CURRENT_TIMESTAMP,
            sync_status TEXT DEFAULT 'pending',
            FOREIGN KEY (from_note_id) REFERENCES notes(id),
            FOREIGN KEY (to_note_id) REFERENCES notes(id)
        )
    )";

    if (!edgesQuery.exec(edgesSql)) {
        qWarning() << "Create edges table error:" << edgesQuery.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::updateNotePosition(const QString &id, double x, double y)
{
    QSqlQuery query(m_database);

    query.prepare(
        "UPDATE notes "
        "SET x = :x, y = :y, updated_at = CURRENT_TIMESTAMP "
        "WHERE id = :id"
        );

    query.bindValue(":x", x);
    query.bindValue(":y", y);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Update note position error:" << query.lastError().text();
        return false;
    }

    return true;
}

QVector<Note> DatabaseManager::loadNotes()
{
    QVector<Note> notes;

    QSqlQuery query(m_database);
    query.prepare("SELECT id, title, content, x, y, color FROM notes ORDER BY created_at ASC");

    if (!query.exec()) {
        qWarning() << "Load notes error:" << query.lastError().text();
        return notes;
    }

    while (query.next()) {
        Note note;
        note.id = query.value(0).toString();
        note.title = query.value(1).toString();
        note.content = query.value(2).toString();
        note.x = query.value(3).toDouble();
        note.y = query.value(4).toDouble();
        note.color = query.value(5).toString();
        notes.append(note);
    }

    return notes;
}

bool DatabaseManager::addNote(const Note &note)
{
    QSqlQuery query(m_database);
    query.prepare(R"(
        INSERT INTO notes (id, title, content, x, y, color, sync_status)
        VALUES (:id, :title, :content, :x, :y, :color, 'pending')
    )");

    query.bindValue(":id", note.id);
    query.bindValue(":title", note.title);
    query.bindValue(":content", note.content);
    query.bindValue(":x", note.x);
    query.bindValue(":y", note.y);
    query.bindValue(":color", note.color);

    if (!query.exec()) {
        qWarning() << "Add note error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::updateNote(const Note &note)
{
    QSqlQuery query(m_database);
    query.prepare(R"(
        UPDATE notes
        SET title = :title,
            content = :content,
            x = :x,
            y = :y,
            color = :color,
            updated_at = CURRENT_TIMESTAMP,
            sync_status = 'pending'
        WHERE id = :id
    )");

    query.bindValue(":id", note.id);
    query.bindValue(":title", note.title);
    query.bindValue(":content", note.content);
    query.bindValue(":x", note.x);
    query.bindValue(":y", note.y);
    query.bindValue(":color", note.color);

    if (!query.exec()) {
        qWarning() << "Update note error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::deleteNote(const QString &id)
{
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM notes WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Delete note error:" << query.lastError().text();
        return false;
    }

    return true;
}

QVector<Edge> DatabaseManager::loadEdges()
{
    QVector<Edge> edges;

    QSqlQuery query(m_database);
    query.prepare("SELECT id, from_note_id, to_note_id FROM edges");

    if (!query.exec()) {
        qWarning() << "Failed to load edges:" << query.lastError().text();
        return edges;
    }

    while (query.next()) {
        Edge edge;
        edge.id = query.value("id").toString();
        edge.fromNoteId = query.value("from_note_id").toString();
        edge.toNoteId = query.value("to_note_id").toString();

        edges.append(edge);
    }

    return edges;
}

bool DatabaseManager::edgeExists(const QString &fromNoteId, const QString &toNoteId)
{
    QSqlQuery query(m_database);
    query.prepare(
        "SELECT COUNT(*) FROM edges "
        "WHERE (from_note_id = :from AND to_note_id = :to) "
        "OR (from_note_id = :to AND to_note_id = :from)"
        );

    query.bindValue(":from", fromNoteId);
    query.bindValue(":to", toNoteId);

    if (!query.exec() || !query.next()) {
        return false;
    }

    return query.value(0).toInt() > 0;
}

bool DatabaseManager::addEdge(const QString &fromNoteId, const QString &toNoteId)
{
    if (fromNoteId.isEmpty() || toNoteId.isEmpty() || fromNoteId == toNoteId) {
        return false;
    }

    if (edgeExists(fromNoteId, toNoteId)) {
        return false;
    }

    QSqlQuery query(m_database);
    query.prepare(
        "INSERT INTO edges (id, from_note_id, to_note_id, created_at) "
        "VALUES (:id, :from, :to, datetime('now'))"
        );

    query.bindValue(":id", QUuid::createUuid().toString(QUuid::WithoutBraces));
    query.bindValue(":from", fromNoteId);
    query.bindValue(":to", toNoteId);

    if (!query.exec()) {
        qWarning() << "Failed to add edge:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::deleteEdgesForNote(const QString &noteId)
{
    QSqlQuery query(m_database);
    query.prepare(
        "DELETE FROM edges "
        "WHERE from_note_id = :noteId OR to_note_id = :noteId"
        );

    query.bindValue(":noteId", noteId);

    if (!query.exec()) {
        qWarning() << "Failed to delete edges for note:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::deleteEdgeBetween(const QString &fromNoteId, const QString &toNoteId)
{
    QSqlQuery query(m_database);

    query.prepare(
        "DELETE FROM edges "
        "WHERE (from_note_id = :from AND to_note_id = :to) "
        "OR (from_note_id = :to AND to_note_id = :from)"
        );

    query.bindValue(":from", fromNoteId);
    query.bindValue(":to", toNoteId);

    if (!query.exec()) {
        qWarning() << "Delete edge error:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}
