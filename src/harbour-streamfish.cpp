#include <QtQuick>
#include <sailfishapp.h>
#include "m3u/m3uchannel.h"
#include "playlistsmodel.h"
#include "m3uchannelsmodel.h"
#include "m3ufilteredchannelsmodel.h"

int main(int argc, char *argv[]) {
    QScopedPointer<QGuiApplication> application(SailfishApp::application(argc, argv));
    application->setOrganizationDomain("io.github.roundedrectangle");
    application->setOrganizationName("io.github.roundedrectangle");
    application->setApplicationName("streamfish");

    qmlRegisterUncreatableType<M3UChannel>("harbour.streamfish.Model", 1, 0, "M3UChannel", "");
    qmlRegisterUncreatableType<M3UPlayList>("harbour.streamfish.Model", 1, 0, "M3UPlayList", "");
    qmlRegisterType<M3UFilteredChannelsModel>("harbour.streamfish.Model", 1, 0, "M3UFilteredChannelsModel");
    qmlRegisterType<M3UChannelsModel>("harbour.streamfish.Model", 1, 0, "M3UChannelsModel");

    QScopedPointer<QQuickView> view(SailfishApp::createView());
    QQmlContext *context = view.data()->rootContext();

    PlaylistsModel *playlistsModel = new PlaylistsModel();
    context->setContextProperty("playlistsModel", playlistsModel);

    view->setSource(SailfishApp::pathTo("qml/harbour-streamfish.qml"));
    view->show();

    return application->exec();
}

