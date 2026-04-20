#include "m3ufilteredchannelsmodel.h"

M3UFilteredChannelsModel::M3UFilteredChannelsModel(QObject *parent) : QSortFilterProxyModel(parent) {
    this->channelsModel = new M3UChannelsModel(this);
    connect(this->channelsModel, &M3UChannelsModel::playlistChanged, this, &M3UFilteredChannelsModel::playlistChanged);

    this->setSourceModel(this->channelsModel);
}

M3UPlayList *M3UFilteredChannelsModel::playlist() const {
    return channelsModel->playlist;
}

void M3UFilteredChannelsModel::setPlaylist(M3UPlayList *playlist) {
    channelsModel->setPlaylist(playlist);
}

void M3UFilteredChannelsModel::setFilter(const QString &filter) {
    if (this->filter == filter)
        return;

    this->filter = filter;
    this->invalidate();
    emit filterChanged();
}

bool M3UFilteredChannelsModel::filterAcceptsRow(int source_row, const QModelIndex &) const {
    M3UChannel* channel = channelsModel->data(channelsModel->index(source_row, 0), M3UChannelsModel::ChannelRole).value<M3UChannel*>();
    return filter.isEmpty() || (channel->name().indexOf(filter, 0, Qt::CaseInsensitive) != -1);
}

