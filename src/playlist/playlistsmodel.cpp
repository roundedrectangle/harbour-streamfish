#include "playlistsmodel.h"
#include "../utilities.h"
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QUrl>
#include <QDir>
#include <QStandardPaths>
#include <QRegularExpression>

const QString PlaylistsModel::PLAYLISTS_PATH = "playlists";
const QString PlaylistsModel::VERSION = "1.0";

// TODO: removing playlists

PlaylistsModel::PlaylistsModel(QObject *parent) : QAbstractListModel(parent) {
    this->playlistsPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + PlaylistsModel::PLAYLISTS_PATH + QDir::separator();
    QDir().mkpath(this->playlistsPath);

    load();
}

int PlaylistsModel::playListCount() const {
    return this->playlists.count();
}

M3UPlayList *PlaylistsModel::playListAt(int idx) const {
    return this->playlists.at(idx);
}

void PlaylistsModel::load() {
    qDebug() << "Loading playlists";

    QJsonDocument doc = readJsonFile(playlistsFilePath());
    if (doc.isNull()) {
        qWarning("Couldn't read playlists file");
        return;
    }

    QJsonArray playlists = doc.object()["playlists"].toArray();
    for (const QJsonValue &playlistValue : playlists) {
        QJsonObject playlist = playlistValue.toObject();
        this->playlists.append(new M3UPlayList(playlist["name"].toString(), QUrl(playlist["url"].toString()), playlist["file"].toString(), playlist["count"].toInt(), this));
    }
}

QString PlaylistsModel::playlistsFilePath() const {
    return playlistsPath + PlaylistsModel::PLAYLISTS_PATH + ".json";
}

void PlaylistsModel::save() const {
    QVariantList newPlaylists;
    for (const M3UPlayList *playlist : this->playlists)
        newPlaylists.append(QVariantMap{
                                {"name", playlist->getName()},
                                {"count", playlist->channelsCount()},
                                {"file", playlistsPath + playlist->getPath()},
                                {"url", playlist->getUrl().toString()},
                            });

    const QVariantMap data{
        {"version", PlaylistsModel::VERSION},
        {"playlists", newPlaylists}
    };
    writeJsonFile(QJsonDocument::fromVariant(data), playlistsFilePath());
}

void PlaylistsModel::handlePlaylistError(const QString &message) {
    qWarning() << "ERROR:" << message;
    emit playlistError(message);
}

void PlaylistsModel::handlePlaylistLoaded() {
    M3UPlayList* playlist = qobject_cast<M3UPlayList*>(this->sender());
    qDebug() << "Playlist loaded" << playlist->getName();

    const int i = playlists.size();
    beginInsertRows(QModelIndex(), i, i);

    this->playlists.append(playlist);
    playlist->save(playlistsPath);
    this->save();

    endInsertRows();
}

void PlaylistsModel::add(const QString &name, const QString &url) {
    QString newUrl = url;

    if(url.indexOf(QRegularExpression("^([^:]+)://")) == -1) // Fallback to HTTP
        newUrl.prepend("http://");

    qDebug() << "Adding playlist" << name << url;
    M3UPlayList* playlist = new M3UPlayList(name, QUrl(newUrl), this);

    connect(playlist, &M3UPlayList::loaded, this, &PlaylistsModel::handlePlaylistLoaded);
    connect(playlist, &M3UPlayList::error, this, &PlaylistsModel::handlePlaylistError);

    playlist->download();
}

void PlaylistsModel::remove(int i) {
    if (i >= 0 && i < playlists.count()) {
        qDebug() << "Removing playlist at" << i;
        beginRemoveRows(QModelIndex(), i, i);
        delete playlists.takeAt(i);
        endRemoveRows();
    }
}


QVariant PlaylistsModel::data(const QModelIndex &index, int role) const {
    int i = index.row();
    if (i >= 0 && i < playlists.size() && role == RolePlaylist)
        return QVariant::fromValue(playlists.at(i));

    return QVariant();
}

int PlaylistsModel::rowCount(const QModelIndex &) const {
    return playlists.size();
}

QHash<int, QByteArray> PlaylistsModel::roleNames() const {
    return {{RolePlaylist, "playlist"}};
}
