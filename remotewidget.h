#ifndef REMOTEWIDGET_H
#define REMOTEWIDGET_H

/** COMMAND DEFINITIONS **/

#define MODE            0
#define CMD             1
#define CMD_DURATION    2
#define ACK             3
#define REQ_INFO        4
#define CURRENT_X       5
#define CURRENT_Y       6
#define HIT_WALL        7

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QGraphicsView>
#include <QPushButton>
#include "udpReceiver.h"
#include "udpSender.h"
#include "QBoardView.h"
#include <QProcess>
#include <QLineEdit>

namespace Ui {
    class RemoteWidget;
}

//We need a slot/function to access datagrams, perhaps called by a timer's signal.
//qudpsocket etc
//TODO:
//

class RemoteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RemoteWidget(QWidget *parent = 0);
    ~RemoteWidget();

private slots:
    void autoManualButtonClicked();
    void packetReceivedUpdateUI(QString completeCommand);
    void startListening();

public slots:
    void handleMoteResponse();
signals:
    void commandListReceived(QList<int>);
    //crickets

private:
    Ui::RemoteWidget*   ui;
    QGridLayout*        controlGridLayout;
    QLabel*             autoOrManual;
    QLabel*             autoOrManualDynamic;
    bool                moteIsAutomatic;
    udpReceiver         *udpR;
    udpSender           *udpS;

    QPushButton         *pbForwards;
    QPushButton         *pbBackwards;
    QPushButton         *pbLeft;
    QPushButton         *pbRight;
    QPushButton         *pbStop;
    QPushButton         *pbToggleManualAuto;
    QBoardView          *board;
    QVBoxLayout         *vboxLayout;
    QProcess            *myProcess;
    QLabel              *deviceName;
    QLineEdit           *deviceNameInput;
    QPushButton         *setDeviceButton;
    QStringList         receivedCommandsSplit;
    QList<int>          commandList;


};

#endif // REMOTEWIDGET_H
