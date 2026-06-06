#pragma once

#include <QAbstractListModel>
#include <QVector>
#include "Edge.h"

class EdgesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum EdgeRoles {
        EdgeIdRole = Qt::UserRole + 1,
        FromNoteIdRole,
        ToNoteIdRole,
        FromXRole,
        FromYRole,
        ToXRole,
        ToYRole
    };

    explicit EdgesModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setEdges(const QVector<Edge> &edges);

private:
    QVector<Edge> m_edges;
};