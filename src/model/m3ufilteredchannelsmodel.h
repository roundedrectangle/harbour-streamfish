#ifndef M3UFILTEREDCHANNELSMODEL_H
#define M3UFILTEREDCHANNELSMODEL_H

#include <QSortFilterProxyModel>
#include "m3uchannelsmodel.h"

class M3UFilteredChannelsModel : public QSortFilterProxyModel {
    Q_OBJECT

    Q_PROPERTY(M3UPlayList* playlist READ playlist WRITE setPlaylist NOTIFY playlistChanged)
    Q_PROPERTY(QString filter MEMBER filter WRITE setFilter NOTIFY filterChanged)

public:
    explicit M3UFilteredChannelsModel(QObject *parent = nullptr);
    M3UPlayList* playlist() const;
    void setPlaylist(M3UPlayList* playlist);
    void setFilter(const QString& filter);

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &) const;

signals:
    void playlistChanged();
    void filterChanged();

private:
    M3UChannelsModel* channelsModel;
    QString filter;
};

#endif // M3UFILTEREDCHANNELSMODEL_H
