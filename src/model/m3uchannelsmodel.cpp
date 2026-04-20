#include "m3uchannelsmodel.h"

M3UChannelsModel::M3UChannelsModel(QObject *parent) : QAbstractListModel(parent) {}

void M3UChannelsModel::setPlaylist(M3UPlayList *playlist) {
    if(this->playlist == playlist)
        return;

    this->playlist = playlist;

    if (this->playlist)
        this->playlist->load();

    emit playlistChanged();
}

QVariant M3UChannelsModel::data(const QModelIndex &index, int role) const {
    if(playlist && role == M3UChannelsModel::ChannelRole)
        return QVariant::fromValue(playlist->channelAt(index.row()));

    return QVariant();
}

int M3UChannelsModel::rowCount(const QModelIndex &) const {
    if (!playlist)
        return 0;

    return playlist->channelsCount();
}

QHash<int, QByteArray> M3UChannelsModel::roleNames() const {
    return {{ChannelRole, "channel"}};
}

