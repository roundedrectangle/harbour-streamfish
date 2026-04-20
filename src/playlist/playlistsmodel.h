#ifndef PLAYLISTSMODEL_H
#define PLAYLISTSMODEL_H

#include <QAbstractListModel>
#include "m3u/m3uplaylist.h"

class PlaylistsModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit PlaylistsModel(QObject *parent = nullptr);
    int playlistCount() const;
    M3UPlayList* playlistAt(int idx) const;

    Q_INVOKABLE void load();
    Q_INVOKABLE void add(const QString &name, const QString &url);
    Q_INVOKABLE void remove(int i);

    enum PlaylistRole { RolePlaylist };

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int rowCount(const QModelIndex &) const;
    virtual QHash<int, QByteArray> roleNames() const;

signals:
    void playlistError(const QString &message);

private:
    QString playlistsFilePath() const;
    void save() const;

private slots:
    void handlePlaylistError(const QString &message);
    void handlePlaylistLoaded();

private:
    QList<M3UPlayList*> playlists;
    QString playlistsPath;

private:
    static const QString PLAYLISTS_PATH;
    static const QString VERSION;
};

#endif // PLAYLISTSMODEL_H
