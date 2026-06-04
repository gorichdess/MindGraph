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
    QSqlQuery query(m_database);

    const QString sql = R"(
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

    if (!query.exec(sql)) {
        qWarning() << "Create table error:" << query.lastError().text();
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
