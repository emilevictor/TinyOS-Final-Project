#include "remotewidget.h"
#include "ui_remotewidget.h"
//#include "udpReceiver.h"
//#include "udpSender.h"
#include <QTcpSocket>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QProcess>
#include <QDebug>

/** COMMAND DEFINITIONS **/

#define MODE            0
#define CMD             1
#define CMD_DURATION    2
#define ACK             3
#define REQ_INFO        4
#define CURRENT_X       5
#define CURRENT_Y       6
#define HIT_WALL        7

//Then run serialforwarder: java net.tinyos.sf.SerialForwarder -comm serial@/dev/ttyUSB1:iris
////java -cp /home/user/local/src/tinyos-2.x/support/sdk/java/tinyos.jar:/home/user/local/src/finalQtProject/finalProject/Final/mote roboTalker


RemoteWidget::RemoteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RemoteWidget)
{
    vboxLayout = new QVBoxLayout(this);

    moteIsAutomatic = true;
    //ui->setupUi(this);
    //ui->currentModeText->setText("Auto");
    controlGridLayout = new QGridLayout(this);
    pbForwards = new QPushButton(this);
    pbForwards->setText("Forward");
    controlGridLayout->addWidget(pbForwards,0,1);

    pbBackwards = new QPushButton(this);
    pbBackwards->setText("Backwards");
    controlGridLayout->addWidget(pbBackwards,2,1);


    pbStop = new QPushButton(this);
    pbStop->setText("Stop");
    controlGridLayout->addWidget(pbStop,1,1);

    pbStop = new QPushButton(this);
    pbStop->setText("Stop");
    controlGridLayout->addWidget(pbStop,1,1);

    pbRight = new QPushButton(this);
    pbRight->setText("Right");
    controlGridLayout->addWidget(pbRight,1,2);

    pbLeft = new QPushButton(this);
    pbLeft->setText("Left");
    controlGridLayout->addWidget(pbLeft,1,0);

    deviceName = new QLabel(this);
    deviceName->setText("Device port:");
    controlGridLayout->addWidget(deviceName,3,0);

    deviceNameInput = new QLineEdit(this);
    deviceNameInput->setText("9004");
    controlGridLayout->addWidget(deviceNameInput,3,1);

    setDeviceButton = new QPushButton(this);
    setDeviceButton->setText("Start listening");
    controlGridLayout->addWidget(setDeviceButton,3,2);

    connect(setDeviceButton,SIGNAL(clicked()), this, SLOT(startListening()));


    /*** BOARD ***/
    QTimer *timer = new QTimer(this);
    timer->setInterval(400);
    timer->start();
    board = new QBoardView(this);
    connect(timer, SIGNAL(timeout()), board, SLOT(updateBoardColours()));

    vboxLayout->addLayout(controlGridLayout);
    vboxLayout->addWidget(board);

    //Connections

    connect(this,SIGNAL(commandListReceived(QList<int>),board,SLOT(acceptCommandList(QList<int>))))



}

//Handles the responses from the java program. Splits the string,
//converts to ints, and puts them into a global array.
void RemoteWidget::handleMoteResponse(){
    if (myProcess->canReadLine())
    {

        QString receivedResponse = myProcess->readLine();

        receivedResponse = receivedResponse.trimmed();

        receivedCommandsSplit = receivedResponse.split(" ");
        if (!receivedCommandsSplit.isEmpty() && (receivedCommandsSplit.length() >= 7))
        {
            commandList.clear();

            for (int i=0; i<receivedCommandsSplit.length();i++)
            {
                commandList.append(receivedCommandsSplit.at(i).toInt());
            }

            emit commandListReceived(commandList);

        }

    }
}

void RemoteWidget::startListening() {

    QString program = "java";

    QStringList arguments;

    arguments << "-classpath" << "/home/user/local/src/tinyos-2.x/support/sdk/java/tinyos.jar:/home/user/local/src/finalQtProject/finalProject/Final/mote" << "roboTalker";
    //arguments << "-cp" << "mote/"<< 'roboTalker';
    myProcess = new QProcess(this);
    myProcess->setProcessChannelMode(QProcess::MergedChannels);
    myProcess->start(program, arguments);
    connect(myProcess,SIGNAL(readyRead()),this,SLOT(handleMoteResponse()));

}


RemoteWidget::~RemoteWidget()
{
    delete ui;
    delete udpR;
    delete udpS;
}

/**
  * This slot propagates information from the auto/manual button click
  */
void RemoteWidget::autoManualButtonClicked()
{
    if (moteIsAutomatic)
    {
        moteIsAutomatic = !moteIsAutomatic;
        udpS->sendDatagram("MANUAL");
    } else {
        moteIsAutomatic = !moteIsAutomatic;
        udpS->sendDatagram("AUTO");
    }
}

/**
  * The packetReceivedUpdateUI(QString) function is called when the UDP receiver receives a packet. If the
  * packet is relevant to the UI, it will update the UI.
  */

void RemoteWidget::packetReceivedUpdateUI(QString completeCommand)
{
    if (completeCommand == "COMMAND AUTO RECVD")
    {
        ui->currentModeText->setText("Auto");
    } else if (completeCommand == "COMMAND MANUAL RECVD")
    {
        ui->currentModeText->setText("Manual");
    }


}
