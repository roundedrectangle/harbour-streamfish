#include "m3uparser.h"
#include <QRegularExpression>
#include <QStringList>
#include <QDebug>

M3UParser::M3UParser(QObject *parent): QObject(parent) {}

const QString &M3UParser::getLastError() const {
    return this->lastError;
}

bool M3UParser::parse(const QString &data, QList<M3UChannel*> &channels, QObject *owner) {
    QStringList lines = data.split(QRegularExpression("[\r\n]"), QString::SkipEmptyParts);
    QRegularExpression channelNameRegex("#EXTINF:[^,]+?,(.+)");
    QRegularExpression channelLogoRegex("tvg-logo=\"([^\"]+)\"");
    bool extInfoDone = false;

    for (int i = 0; i < lines.length(); i++) {
        QString line = lines[i].trimmed();

        if (line == "#EXTM3U") // Skip header, if any
            continue;

        if (line.startsWith("#EXTINF")) {
            if (extInfoDone) {
                this->warning(tr("Malformed M3U playlist, skipping line %1"), i, extInfoDone);
                continue;
            }

            channels.append(new M3UChannel(channelNameRegex.match(line).captured(1), QString(), channelLogoRegex.match(line).captured(1), owner));
            extInfoDone = true;
        } else {
            if (line.startsWith("#"))
                continue;

            if (!extInfoDone) {
                this->warning(tr("Malformed M3U playlist, skipping line %1"), i, extInfoDone);
                continue;
            }

            channels.last()->setUrl(line);
            extInfoDone = false;
        }
    }

    if (channels.isEmpty())
        return this->error(tr("Invalid M3U playlist"));

    return true;
}

bool M3UParser::error(const QString &message) {
    this->lastError = message;
    return false;
}

void M3UParser::warning(const QString &message, int line, bool &extInfoDone) {
    qWarning() << QString(message).arg(line);
    extInfoDone = !extInfoDone;
}
