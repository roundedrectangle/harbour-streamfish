#include "m3uplaylist.h"
#include "../../utilities.h"
#include <QCryptographicHash>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFile>
#include <QDir>

QNetworkAccessManager* M3UPlayList::networkManagerInstance = NULL;
const QString M3UPlayList::VERSION = "1.0";

M3UPlayList::M3UPlayList(const QString &name, const QUrl &url, QObject *parent): QObject(parent), isLoaded(false), count(0), name(name), url(url) {
    this->path = QCryptographicHash::hash(name.toUtf8(), QCryptographicHash::Md5).toHex() + ".playlist";
}

M3UPlayList::M3UPlayList(const QString &name, const QUrl &url, const QString &file, int count, QObject *parent): QObject(parent), isLoaded(false), count(count), name(name), path(file), url(url)
{}

const QString& M3UPlayList::getName() const {
    return this->name;
}

const QString &M3UPlayList::getPath() const {
    return this->path;
}

const QUrl &M3UPlayList::getUrl() const {
    return this->url;
}

int M3UPlayList::channelsCount() const {
    return isLoaded ? channels.count() : count;
}

M3UChannel *M3UPlayList::channelAt(int i) const {
    return this->channels.at(i);
}

QNetworkAccessManager *M3UPlayList::networkManager() {
    if (!M3UPlayList::networkManagerInstance)
        M3UPlayList::networkManagerInstance = new QNetworkAccessManager(this->parent());

    return M3UPlayList::networkManagerInstance;
}

void M3UPlayList::download() {
    QNetworkReply* reply = this->networkManager()->get(QNetworkRequest(this->url));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleDownloadError(QNetworkReply::NetworkError))); // NOTE: Overload madness
    connect(reply, SIGNAL(finished()), this, SLOT(handleDownloadFinished()));
}

void M3UPlayList::save(const QString &dest) {
    QVariantList newChannels;
    for (const M3UChannel *channel : this->channels)
        newChannels.append(QVariantMap{
                                {"name", channel->name()},
                                {"url", channel->url().toString()},
                                {"logo", channel->logo()}
                            });

    const QVariantMap data{
        {"version", M3UPlayList::VERSION},
        {"channels", newChannels}
    };
    writeJsonFile(QJsonDocument::fromVariant(data), dest + this->path);
}

void M3UPlayList::load(bool force) {
    if (!force && this->isLoaded)
        return;

    QJsonDocument doc = readJsonFile(path);
    if (doc.isNull()) {
        qWarning("Couldn't read playlist file");
        return;
    }

    QVariantList channels = doc.object()["channels"].toArray().toVariantList();
    for (const QVariant &channelVariant : channels) {
        const QVariantMap channel = channelVariant.toMap();
        this->channels.append(new M3UChannel(channel.value("name").toString(), channel.value("url").toString(), channel.value("logo").toString(), this));
    }

    this->isLoaded = true;
}

void M3UPlayList::handleDownloadFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(this->sender());
    if (reply->error()) // already handled by handleDownloadError
        return;

    qDebug("Download finished");
    M3UParser m3uparser;
    QString m3udata = QString::fromUtf8(reply->readAll());

    if (!m3uparser.parse(m3udata, this->channels, this)) {
        this->channels.clear(); // Invalid playlist, delete all items
        emit error(m3uparser.lastError());
    } else {
        this->isLoaded = true;
        emit loaded();
    }

    reply->deleteLater();
}

void M3UPlayList::handleDownloadError(QNetworkReply::NetworkError) {
    qDebug("Network error");
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(this->sender());

    emit error(reply->errorString());
    reply->deleteLater();
}
