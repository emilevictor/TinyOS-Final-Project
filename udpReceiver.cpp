#include "udpReceiver.h"
#define PORT 1234

udpReceiver::udpReceiver(QObject *parent)
    : QObject(parent)
{
    //Here we will create our udp socket and connect to it.
    udpSocket = new QUdpSocket(this);
    //Change the port based on what
    udpSocket->bind(QHostAddress::AnyIPv6,PORT);

    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(receiveDatagram()));

}


void udpReceiver::receiveDatagram()
{
    while (udpSocket->hasPendingDatagrams()) {
        //Create QByteArray for datagram
        QByteArray datagram;

        //Resize the datagram based on the size of the received datagram
        datagram.resize(udpSocket->pendingDatagramSize());

        //Read from the datagram
        udpSocket->readDatagram(datagram.data(), datagram.size());
        qDebug() << "Just emitted datagram" << datagram.data();
        emit receivedDatagram(datagram.data());
    }
}
