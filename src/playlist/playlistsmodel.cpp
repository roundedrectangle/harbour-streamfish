#include "playlistsmodel.h"
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
    for (const M3UPlayList* playlist : this->playlists) {
        newPlaylists.append(QVariantMap{
                                {"name", playlist->name()},
                                {"count", playlist->channelsCount()},
                                {"file", playlistsPath + playlist->file()},
                                {"url", playlist->url().toString()},
                            });
    }

    const QVariantMap data{
        {"version", PlaylistsModel::VERSION},
        {"playlists", newPlaylists}
    };
    writeJsonFile(QJsonDocument::fromVariant(data), playlistsFilePath());
}

void PlaylistsModel::onPlayListError(const QString &errmsg) {
    qWarning() << "ERROR:" << errmsg;
}

void PlaylistsModel::onPlayListLoaded() {
    M3UPlayList* playlist = qobject_cast<M3UPlayList*>(this->sender());

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

    M3UPlayList* playlist = new M3UPlayList(name, QUrl(newUrl), this);

    connect(playlist, &M3UPlayList::loaded, this, &PlaylistsModel::onPlayListLoaded);
    connect(playlist, &M3UPlayList::error, this, &PlaylistsModel::onPlayListError);

    playlist->download();
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
