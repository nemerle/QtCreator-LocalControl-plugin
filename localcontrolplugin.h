#pragma once

#include "localcontrol_global.h"

#include <extensionsystem/iplugin.h>

#include <QtNetwork/QTcpServer>
#include <QtCore/QVector>
#include <QtCore/QMap>

namespace LocalControl {
namespace Internal {

class LocalControlPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "LocalControl.json")
    QTcpServer *m_listener_socket;
    QMap<QTcpSocket *,QByteArray> m_client_socket_map;
public:
    LocalControlPlugin();
    ~LocalControlPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private slots:
    void onNewConnection();
    void onNewCommand();
};

} // namespace Internal
} // namespace LocalControl
