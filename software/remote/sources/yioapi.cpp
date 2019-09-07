#include <QtDebug>

#include "yioapi.h"

#include <QJsonDocument>
#include <QJsonArray>

YioAPI* YioAPI::s_instance = nullptr;

YioAPI::YioAPI(QQmlApplicationEngine *engine) :
    m_engine(engine)
{
    s_instance = this;
}

YioAPI::~YioAPI()
{
    s_instance = nullptr;
}

void YioAPI::start()
{
    m_server = new QWebSocketServer(QStringLiteral("YIO API"), QWebSocketServer::NonSecureMode, this);

    // start websocket server on port 946(YIO)
    if (m_server->listen(QHostAddress::Any, 946)) {
        connect(m_server, &QWebSocketServer::newConnection, this, &YioAPI::onNewConnection);
        connect(m_server, &QWebSocketServer::closed, this, &YioAPI::closed);
        m_running = true;
        emit runningChanged();
    }
}

void YioAPI::stop()
{
    m_server->close();
    m_clients.clear();
    m_running = false;
    emit runningChanged();
}

void YioAPI::onNewConnection()
{
    QWebSocket *socket = m_server->nextPendingConnection();

    connect(socket, &QWebSocket::textMessageReceived, this, &YioAPI::processMessage);
    connect(socket, &QWebSocket::disconnected, this, &YioAPI::onClientDisconnected);

    // send message to client after connected to authenticate
    QVariantMap map;
    map.insert("type", "auth_required");
    QJsonDocument doc = QJsonDocument::fromVariant(map);
    QString message = doc.toJson(QJsonDocument::JsonFormat::Compact);

    socket->sendTextMessage(message);

    m_clients.insert(socket, false);
}

void YioAPI::processMessage(QString message)
{
    QVariantMap r_map;

    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if (client) {
        qDebug() << message;

        // convert message to json
        QJsonParseError parseerror;
        QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8(), &parseerror);
        if (parseerror.error != QJsonParseError::NoError) {
            qDebug() << "JSON error : " << parseerror.errorString();
            return;
        }
        QVariantMap map = doc.toVariant().toMap();

        QString type = map.value("type").toString();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // AUTHENTICATION
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (type == "auth") {
             qDebug() << m_clients[client];

            if (map.contains("token")) {
                qDebug() << "Has token";
                if (map.value("token").toString() == m_token) {
                    qDebug() << "Token OK";
                    r_map.insert("type", "auth_ok");
                    QJsonDocument r_doc = QJsonDocument::fromVariant(r_map);
                    QString r_message = r_doc.toJson(QJsonDocument::JsonFormat::Compact);

                    client->sendTextMessage(r_message);

                    m_clients[client] = true;

                    qDebug() << m_clients[client];

                } else {
                    qDebug() << "Token NOT OK";
                    r_map.insert("type", "auth_error");
                    r_map.insert("message", "Invalid token");
                    QJsonDocument r_doc = QJsonDocument::fromVariant(r_map);
                    QString r_message = r_doc.toJson(QJsonDocument::JsonFormat::Compact);

                    client->sendTextMessage(r_message);
                }
            } else {
                qDebug() << "No token";
                r_map.insert("type", "auth_error");
                r_map.insert("message", "Token needed");
                QJsonDocument r_doc = QJsonDocument::fromVariant(r_map);
                QString r_message = r_doc.toJson(QJsonDocument::JsonFormat::Compact);

                client->sendTextMessage(r_message);
            }
        }
    }
}

void YioAPI::onClientDisconnected()
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if (client) {
        m_clients.remove(client);
        client->deleteLater();
    }
}
