#include "playlistsmodel.h"
#include "utilities.h"
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

void PlaylistsModel::load() {
    qDebug() << "Loading playlists";

    QJsonDocument doc = readJsonFile(playlistsFilePath());
    if (doc.isNull()) {
        qWarning("Couldn't read playlists file");
        return;
    }

    const QVariantMap map = doc.object().toVariantMap();

    this->lastOpenedIndex = map.value("lastOpenedIndex", -1).toInt();

    for (const QVariant &playlistVariant : map.value("playlists").toList()) {
        QVariantMap playlist = playlistVariant.toMap();
        this->playlists.append(new M3UPlayList(playlist.value("name").toString(), QUrl(playlist.value("url").toString()), playlist.value("file").toString(), playlist.value("count").toInt(), this));
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
                                {"file", playlist->getFileName()},
                                {"url", playlist->getUrl().toString()},
                            });

    const QVariantMap data{
        {"version", PlaylistsModel::VERSION},
        {"playlists", newPlaylists},
        {"lastOpenedIndex", lastOpenedIndex}
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
    playlist->save();
    save();

    endInsertRows();
}

void PlaylistsModel::add(const QString &name, const QString &url) {
    QString newUrl = url;

    if (url.indexOf(QRegularExpression("^([^:]+)://")) == -1) // Fallback to HTTP
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

        if (lastOpenedIndex == i) {
            lastOpenedIndex = -1;
            emit lastOpenedIndexChanged();
            emit lastOpenedPlaylistChanged();
        } else if (lastOpenedIndex > i) {
            lastOpenedIndex--;
            emit lastOpenedIndexChanged();
        }

        save();
        endRemoveRows();
    }
}

QVariant PlaylistsModel::playlistAt(int i) const {
    if (i >= 0 && i < playlists.count())
        return QVariant::fromValue(playlists.at(i));
    return QVariant();
}

QVariant PlaylistsModel::data(const QModelIndex &index, int role) const {
    if (role == RolePlaylist)
        return playlistAt(index.row());
    return QVariant();
}

int PlaylistsModel::rowCount(const QModelIndex &) const {
    return playlists.size();
}

QHash<int, QByteArray> PlaylistsModel::roleNames() const {
    return {{RolePlaylist, "playlist"}};
}


void PlaylistsModel::setLastOpenedIndex(int index) {
    if (lastOpenedIndex != index) {
        qDebug() << "Setting last opened index" << lastOpenedIndex << index;
        lastOpenedIndex = index;
        save();

        emit lastOpenedIndexChanged();
        emit lastOpenedPlaylistChanged();
    }
}

QVariant PlaylistsModel::lastOpened() const {
    qDebug() << "Getting last opened playlist" << lastOpenedIndex;
    return playlistAt(lastOpenedIndex);
}
