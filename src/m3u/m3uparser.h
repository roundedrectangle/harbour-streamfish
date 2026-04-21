#ifndef M3UPARSER_H
#define M3UPARSER_H

#include <QObject>
#include <QString>
#include "m3uchannel.h"

class M3UParser : public QObject {
    Q_OBJECT

public:
    M3UParser(QObject *parent = nullptr);
    const QString &getLastError() const;
    bool parse(const QString &data, QList<M3UChannel*> &channels, QObject *owner);

private:
    bool error(const QString &message);
    void warning(const QString &message, int line, bool &extInfoDone);

private:
    QString lastError;
};

#endif // M3UPARSER_H
