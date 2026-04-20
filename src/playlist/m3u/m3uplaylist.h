#ifndef M3UPLAYLIST_H
#define M3UPLAYLIST_H

#include <QObject>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "m3uparser.h"

class M3UPlayList : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ getName CONSTANT FINAL)
    Q_PROPERTY(int channelsCount READ channelsCount NOTIFY channelsCountChanged)

public:
    explicit M3UPlayList(const QString& name, const QUrl& url, QObject *parent = nullptr);
    explicit M3UPlayList(const QString& name, const QUrl& url, const QString& file, int count, QObject *parent = nullptr);
    const QString& getName() const;
    const QString& getPath() const;
    const QUrl& getUrl() const;
    int channelsCount() const;
    M3UChannel* channelAt(int i) const;
    void save(const QString& dest);
    void load(bool force = false);
    void download();

private slots:
    void handleDownloadFinished();
    void handleDownloadError(QNetworkReply::NetworkError);

signals:
    void loaded();
    void error(const QString& errmsg);
    void channelsCountChanged();

private:
    bool isLoaded;
    int count;
    QList<M3UChannel*> channels;
    QString name;
    QString path;
    QUrl url;

private:
    QNetworkAccessManager* networkManager();

    static QNetworkAccessManager* networkManagerInstance;
    static const QString VERSION;
};

Q_DECLARE_METATYPE(M3UPlayList*)

#endif // M3UPLAYLIST_H
