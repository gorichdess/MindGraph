#pragma once

#include <QString>

struct Edge
{
    QString id;
    QString fromNoteId;
    QString toNoteId;

    double fromX = 0;
    double fromY = 0;
    double toX = 0;
    double toY = 0;
};