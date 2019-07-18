#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QDateTime>
#include <QMap>

class Notifications : public QObject
{
    Q_OBJECT

    // list of all notifications
    Q_PROPERTY  (QVariantList            list            READ    list           NOTIFY listChanged      CONSTANT)
    Q_PROPERTY  (bool                    isThereError    READ    isThereError   NOTIFY errorChanged     CONSTANT)

public:
    // get all notifications
    QVariantList                list            ();

    // get if error
    bool                        isThereError    ();

    // add notification
    Q_INVOKABLE void            add             (const bool &type, const QString &text, const QString &actionlabel, const QVariant &action);
    Q_INVOKABLE void            add             (const bool &type, const QString &text);

    // remove notification
    Q_INVOKABLE void            remove          (const int id);

    // check if there's an error notification
//    Q_INVOKABLE bool            isThereError    ();


    explicit Notifications(QQmlApplicationEngine *engine = NULL);
    virtual ~Notifications();

    static Notifications*       getInstance     ()
    { return s_instance; }

signals:
    void listChanged();
    void errorChanged();

private:
    int                         m_id = 0;
    QMap<int, QVariantMap>      m_notifications;

    static Notifications*       s_instance;
    QQmlApplicationEngine*      m_engine;

    void                        show            (const int id);

};

#endif // NOTIFICATIONS_H
