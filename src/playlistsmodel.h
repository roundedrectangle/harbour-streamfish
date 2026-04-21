#ifndef PLAYLISTSMODEL_H
#define PLAYLISTSMODEL_H

#include <QAbstractListModel>
#include "m3u/m3uplaylist.h"

class PlaylistsModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(int lastOpenedIndex MEMBER lastOpenedIndex WRITE setLastOpenedIndex NOTIFY lastOpenedIndexChanged)
    Q_PROPERTY(QVariant lastOpenedPlaylist READ lastOpened NOTIFY lastOpenedPlaylistChanged)

public:
    enum PlaylistRole { RolePlaylist };

    explicit PlaylistsModel(QObject *parent = nullptr);

    friend class M3UPlayList;

    Q_INVOKABLE void load();
    Q_INVOKABLE void add(const QString &name, const QString &url);
    Q_INVOKABLE void remove(int i);
    Q_INVOKABLE QVariant playlistAt(int i) const;

    void setLastOpenedIndex(int index);
    QVariant lastOpened() const;

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int rowCount(const QModelIndex &) const;
    virtual QHash<int, QByteArray> roleNames() const;

signals:
    void playlistError(const QString &message);
    void lastOpenedIndexChanged();
    void lastOpenedPlaylistChanged();

private:
    QString playlistsFilePath() const;
    void save() const;

private slots:
    void handlePlaylistError(const QString &message);
    void handlePlaylistLoaded();

private:
    QList<M3UPlayList*> playlists;
    QString playlistsPath;
    int lastOpenedIndex = -1;

private:
    static const QString PLAYLISTS_PATH;
    static const int VERSION;
};

#endif // PLAYLISTSMODEL_H
