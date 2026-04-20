#ifndef UTILITIES_H
#define UTILITIES_H

#include <QJsonDocument>
#include <QFile>
#include <QDebug>

QJsonDocument readJsonFile(const QString &path) {
    QFile f(path);
    if (!f.open(QFile::ReadOnly)) {
        qWarning() << "Couldn't open file for reading" << path;
        return QJsonDocument();
    }

    const QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    f.close();
    return doc;
}

void writeJsonFile(const QJsonDocument &doc, const QString &path) {
    QFile f(path);
    if (!f.open(QFile::WriteOnly)) {
        qWarning() << "Couldn't open file for writing" << path;
        return;
    }

    f.write(doc.toJson());
    f.close();
}

#endif // UTILITIES_H
