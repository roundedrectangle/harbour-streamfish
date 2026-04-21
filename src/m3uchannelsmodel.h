#ifndef M3UCHANNELSMODEL_H
#define M3UCHANNELSMODEL_H

#include <QAbstractListModel>
#include "m3u/m3uplaylist.h"

class M3UChannelsModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(M3UPlayList* playlist MEMBER playlist WRITE setPlaylist NOTIFY playlistChanged)

public:
    enum { ChannelRole = Qt::DisplayRole };

    explicit M3UChannelsModel(QObject *parent = nullptr);
    void setPlaylist(M3UPlayList* playlist);

    friend class M3UFilteredChannelsModel;

public:
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int rowCount(const QModelIndex &) const;
    virtual QHash<int, QByteArray> roleNames() const;

signals:
    void playlistChanged();

private:
    M3UPlayList* playlist = nullptr;
};

#endif // M3UCHANNELSMODEL_H
