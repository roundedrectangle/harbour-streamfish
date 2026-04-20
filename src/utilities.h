#ifndef UTILITIES_H
#define UTILITIES_H

#include <QJsonDocument>

QJsonDocument readJsonFile(const QString &path);
void writeJsonFile(const QJsonDocument &doc, const QString &path);

#endif // UTILITIES_H
