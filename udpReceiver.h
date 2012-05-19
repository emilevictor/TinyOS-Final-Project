#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QWidget>
#include <QUdpSocket>
#include <QNetworkInterface>

/**
  *This class receives UDP datagrams and emits the strings contained in them. This will be used for communication with the mote.
  */
class udpReceiver : public QObject
{
    Q_OBJECT

public:
    udpReceiver(QObject* parent = 0);


private:
    QUdpSocket *udpSocket;


private slots:
    void receiveDatagram();

signals:
    void receivedDatagram(QString string);
};



#endif // UDPRECEIVER_H
