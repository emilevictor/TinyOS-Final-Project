#include "udpSender.h"

udpSender :: udpSender(QObject *parent) : QObject(parent)
{
    udpSenderSock = new QUdpSocket(this);

}

void udpSender::sendDatagram(QString command) {
    qDebug() << "Broadcasting message: COMMAND " + command;

    QByteArray datagram = "COMMAND ";
    datagram.append(command);

    udpSenderSock->writeDatagram(datagram.data(), datagram.size(),QHostAddress::AnyIPv6, 1234);

    //Let the rest of the program know that the command was sent.
    emit datagramSent("COMMAND " + command);
}

void udpSender::autoManualToggleClicked(){

}
