#include "EdgesModel.h"

EdgesModel::EdgesModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int EdgesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_edges.size();
}

QVariant EdgesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_edges.size()) {
        return {};
    }

    const Edge &edge = m_edges[index.row()];

    switch (role) {
    case EdgeIdRole:
        return edge.id;
    case FromNoteIdRole:
        return edge.fromNoteId;
    case ToNoteIdRole:
        return edge.toNoteId;
    case FromXRole:
        return edge.fromX;
    case FromYRole:
        return edge.fromY;
    case ToXRole:
        return edge.toX;
    case ToYRole:
        return edge.toY;
    default:
        return {};
    }
}

QHash<int, QByteArray> EdgesModel::roleNames() const
{
    return {
        { EdgeIdRole, "edgeId" },
        { FromNoteIdRole, "fromNoteId" },
        { ToNoteIdRole, "toNoteId" },
        { FromXRole, "fromX" },
        { FromYRole, "fromY" },
        { ToXRole, "toX" },
        { ToYRole, "toY" }
    };
}

void EdgesModel::setEdges(const QVector<Edge> &edges)
{
    beginResetModel();
    m_edges = edges;
    endResetModel();
}