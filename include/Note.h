#pragma once

#include <QString>

struct Note
{
    QString id;
    QString title;
    QString content;
    double x = 0;
    double y = 0;
    QString color = "#8EA7E9";
};
