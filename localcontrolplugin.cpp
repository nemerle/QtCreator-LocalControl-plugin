#include "localcontrolplugin.h"
#include "localcontrolconstants.h"
#include "CommProtocol.h"
#include "CommCodec.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/coreconstants.h>


#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>
#include <QTcpSocket>

#include <QtPlugin>

using namespace LocalControl::Internal;

LocalControlPlugin::LocalControlPlugin()
{
    // Create your members
}

LocalControlPlugin::~LocalControlPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool LocalControlPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    m_listener_socket = new QTcpServer(this);

    connect(m_listener_socket,&QTcpServer::newConnection,this,&LocalControlPlugin::onNewConnection);
    m_listener_socket->listen(QHostAddress::LocalHost,20803);

    return true;
}
void LocalControlPlugin::onNewCommand() {
    QTcpSocket * src_sock = qobject_cast<QTcpSocket *>(sender());
    if(!src_sock) {
        qDebug() << "on New command got nullptr QTcpSocket as a sender";
    }

    m_client_socket_map[src_sock] += src_sock->readAll();
    QByteArray &ba = m_client_socket_map[src_sock];
    ControlPackets::Packet * pa = decPacket(ba);
    if(!pa)
        return;
    switch(pa->kind) {
    case ControlPackets::eOpenSolution:
        break;
    case ControlPackets::eOpenFiles:
        break;
    case ControlPackets::eOpenFileAtLine:
        break;
    case ControlPackets::eSaveAllOpenDocuments:
        break;
    }

}
void LocalControlPlugin::onNewConnection()
{
    QTcpSocket * sock = m_listener_socket->nextPendingConnection();
    QByteArray data;
    ControlPackets::ServerVersion vers;
    vers.major = 0;
    vers.minor = 1;
    encServerVersion(data,vers);
    sock->write(data);
    m_client_socket_map[sock] = QByteArray();
    connect(sock,&QTcpSocket::readyRead,this,&LocalControlPlugin::onNewCommand);
}
void LocalControlPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag LocalControlPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}
