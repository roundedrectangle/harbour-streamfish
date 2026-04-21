import QtQuick 2.0
import Sailfish.Silica 1.0
import "../modules/Opal/About"

AboutPageBase {
    allowedOrientations: Orientation.All
    appName: "Streamfish"
    appIcon: 'qrc:///res/streamfish.svg'
    appVersion: '1.1.0'
    description: qsTr("Streamfish is an IPTV client for SailfishOS")
    authors: ["roundedrectangle", "Dax89"]
    licenses: License { spdxId: 'GPL-3.0-or-later' }
    sourcesUrl: 'https://github.com/roundedrectangle/harbour-streamfish'

    // Make icon bigger and scale the SVG as intended
    _iconItem.width: Math.min(2 * Theme.itemSizeHuge, Math.min(page.width, page.height) / 2)
    _iconItem.height: _iconItem.width
    _iconItem.sourceSize.width: _iconItem.width
    _iconItem.sourceSize.height: _iconItem.height

    contributionSections: [
        ContributionSection {
            title: qsTr("Translations")
            groups: [
                ContributionGroup {
                    title: qsTr("Italian")
                    entries: ["247"]
                }
            ]
        }
    ]
}
