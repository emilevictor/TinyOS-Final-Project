#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <QObject>
#include <QUdpSocket>

class udpSender : public QObject
{
    Q_OBJECT
public:
    udpSender(QObject *parent = 0);

public slots:
    void sendDatagram(QString command); //Sends the string as a datagram over IPv6
    void autoManualToggleClicked();

signals:
    void datagramSent(QString commandSent);

private:
    QUdpSocket *udpSenderSock;
    QString *gCommand;
};


#endif // UDPSENDER_H
