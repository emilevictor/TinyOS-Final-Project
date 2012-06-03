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
    durationMilliseconds = 500;
    vboxLayout = new QVBoxLayout(this);

    moteIsAutomatic = false;
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

    pbRight = new QPushButton(this);
    pbRight->setText("Right");
    controlGridLayout->addWidget(pbRight,1,2);

    pbLeft = new QPushButton(this);
    pbLeft->setText("Left");
    controlGridLayout->addWidget(pbLeft,1,0);

    durationText = new QLabel(this);
    durationText->setText("Command duration:");
    controlGridLayout->addWidget(durationText,3,0);

    setCmdDurationButton = new QPushButton(this);
    setCmdDurationButton->setText("Set duration");
    controlGridLayout->addWidget(setCmdDurationButton,3,2);

    connect(setCmdDurationButton, SIGNAL(clicked()),this,SLOT(setDurationMilliseconds()));

    cmdDuration = new QLineEdit(this);
    cmdDuration->setPlaceholderText("Enter in milliseconds");
    controlGridLayout->addWidget(cmdDuration,3,1);

    pbToggleManualAuto = new QPushButton(this);
    pbToggleManualAuto->setText("Auto Mode");
    controlGridLayout->addWidget(pbToggleManualAuto,4,1);
    pbToggleManualAuto->setVisible(false);

    pbStartListening = new QPushButton(this);
    pbStartListening->setText("Start listening");
    controlGridLayout->addWidget(pbStartListening,4,2);

    connect(pbStartListening,SIGNAL(clicked()), this, SLOT(startListening()));
    connect(pbToggleManualAuto,SIGNAL(clicked()),this,SLOT(autoManualButtonClicked()));

    /******* Manual commands *****/

    connect(pbForwards, SIGNAL(clicked()), this, SLOT(moveForwards()));
    connect(pbBackwards,SIGNAL(clicked()), this, SLOT(moveBackwards()));
    connect(pbLeft,SIGNAL(clicked()), this, SLOT(rotateAnticlockwise()));
    connect(pbRight, SIGNAL(clicked()), this, SLOT(rotateClockwise()));
    connect(pbStop, SIGNAL(clicked()), this, SLOT(stopVehicle()));

    /*** BOARD ***/
    QTimer *timer = new QTimer(this);
    timer->setInterval(25);
    timer->start();
    board = new QBoardView(this);
    connect(timer, SIGNAL(timeout()), board, SLOT(updateBoardColours()));

    vboxLayout->addLayout(controlGridLayout);
    vboxLayout->addWidget(board);

    //Connections

    connect(this,SIGNAL(commandListReceived(QList<int>)),board,SLOT(acceptCommandList(QList<int>)));



}

void RemoteWidget::setDurationMilliseconds() {

    durationMilliseconds = cmdDuration->displayText().toInt();
    durationMilliseconds = (int)(((float)durationMilliseconds/1000)*1024);
    qDebug() << durationMilliseconds;
}



//Handles the responses from the java program. Splits the string,
//converts to ints, and puts them into a global array.
void RemoteWidget::handleMoteResponse(){
    if (roboReceiverProcess->canReadLine())
    {

        QString receivedResponse = roboReceiverProcess->readLine();

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

/**
  * SendMessage sends a message, and takes as input a command list.
  * The definition for this localCommandList is seen in packet.h
  */
void RemoteWidget::sendMessage(QList<int> localCommandList)
{
    //Construct string including a newline character
    QString sentString = "";
    for (int i = 0; i < 8; i++)
    {
        sentString = sentString + QString::number(localCommandList.at(i)) + " ";
    }
    sentString.chop(1);
    sentString += "\n";
    qDebug() << sentString;


    //Convert it into a QByteArray*

    QByteArray data = sentString.toUtf8();

    roboSenderProcess->write(data);

    //Then write it to stdin of roboSenderProcess

}


/**
  * StartListening: this function will conenct to the two java programs (for sending and listening to the mote(s)).
  *
  */
void RemoteWidget::startListening() {

    QString program = "java";

    QStringList arguments;

    arguments << "-classpath" << "/home/user/local/src/tinyos-2.x/support/sdk/java/tinyos.jar:/home/user/local/src/finalQtProject/finalProject/Final/mote" << "roboTalker";
    roboReceiverProcess = new QProcess(this);
    roboReceiverProcess->setProcessChannelMode(QProcess::MergedChannels);
    roboReceiverProcess->start(program, arguments);
    connect(roboReceiverProcess,SIGNAL(readyRead()),this,SLOT(handleMoteResponse()));


    arguments.clear();
    arguments << "-classpath" << "/home/user/local/src/tinyos-2.x/support/sdk/java/tinyos.jar:/home/user/local/src/finalQtProject/finalProject/Final/mote" << "roboSender";
    roboSenderProcess = new QProcess(this);
    roboSenderProcess->setProcessChannelMode(QProcess::ForwardedChannels);
    roboSenderProcess->start(program,arguments);
    //No need for a readyRead() connection on this one due to forwarded channels.

    pbToggleManualAuto->setVisible(true);

}


RemoteWidget::~RemoteWidget()
{
    delete ui;
    roboReceiverProcess->terminate();
    roboSenderProcess->terminate();
    delete roboReceiverProcess;
    delete roboSenderProcess;
}


/**
  * This slot propagates information from the auto/manual button click
  */
void RemoteWidget::autoManualButtonClicked()
{
    QList<int> commandListToBeSent;

    if (moteIsAutomatic)
    {
        moteIsAutomatic = !moteIsAutomatic;

        commandListToBeSent.append(1); //0 = auto, 1 = manual
        commandListToBeSent.append(254);
        commandListToBeSent.append(254);
        commandListToBeSent.append(254);
        commandListToBeSent.append(254);
        commandListToBeSent.append(254);
        commandListToBeSent.append(254);
        commandListToBeSent.append(254);
        sendMessage(commandListToBeSent);
        pbToggleManualAuto->setText("Current: Manual Mode");

        pbForwards->setVisible(true);
        pbBackwards->setVisible(true);
        pbStop->setVisible(true);
        pbRight->setVisible(true);
        pbLeft->setVisible(true);


    } else {
        moteIsAutomatic = !moteIsAutomatic;
        commandListToBeSent.append(0); //0 = auto, 1 = manual
        commandListToBeSent.append(254);
        commandListToBeSent.append(254);
        commandListToBeSent.append(254);
        commandListToBeSent.append(254);
        commandListToBeSent.append(254);
        commandListToBeSent.append(254);
        commandListToBeSent.append(254);
        sendMessage(commandListToBeSent);
        pbToggleManualAuto->setText("Current: Auto Mode");

        pbForwards->setVisible(false);
        pbBackwards->setVisible(false);
        pbStop->setVisible(false);
        pbRight->setVisible(false);
        pbLeft->setVisible(false);

    }
}

/**
  * Move forwards: sends a command packet to the mote to move forward for a specific amount of time.
  *
  */
void RemoteWidget::moveForwards()
{

    qDebug() << "Driving forwards";

    QList<int> commandListToBeSent;

    //Here, we construct our command list to be sent using sendMessage.
    commandListToBeSent.append(254); //0 = auto, 1 = manual
    commandListToBeSent.append(0); //CMD nx_uint8_t: 0(forward) 1(clockwise) 2(backward) 3(anticlockwise) 4(stop)
    commandListToBeSent.append(durationMilliseconds); //CMD_DURATION
    commandListToBeSent.append(254); //ACK - we don't care
    commandListToBeSent.append(254); //REQ_INFO - we don't care
    commandListToBeSent.append(254); //CURRENT_X - we don't care
    commandListToBeSent.append(254); //CURRENT_Y - we don't care
    commandListToBeSent.append(254); //HIT_WALL - we don't care
    sendMessage(commandListToBeSent);

}

/**
  * stopVehicle: stops the vehicle
  */
void RemoteWidget::stopVehicle()
{
    qDebug() << "Stopping vehicle";


    QList<int> commandListToBeSent;

    //Here, we construct our command list to be sent using sendMessage.
    commandListToBeSent.append(254); //0 = auto, 1 = manual
    commandListToBeSent.append(4); //CMD nx_uint8_t: 0(forward) 1(clockwise) 2(backward) 3(anticlockwise) 4(stop)
    commandListToBeSent.append(254); //CMD_DURATION
    commandListToBeSent.append(254); //ACK - we don't care
    commandListToBeSent.append(254); //REQ_INFO - we don't care
    commandListToBeSent.append(254); //CURRENT_X - we don't care
    commandListToBeSent.append(254); //CURRENT_Y - we don't care
    commandListToBeSent.append(254); //HIT_WALL - we don't care
    sendMessage(commandListToBeSent);

}

/**
  * moveBackwards: reverses the vehicle
  *
  */
void RemoteWidget::moveBackwards()
{
    qDebug() << "Reversing";


    QList<int> commandListToBeSent;

    //Here, we construct our command list to be sent using sendMessage.
    commandListToBeSent.append(254); //0 = auto, 1 = manual
    commandListToBeSent.append(2); //CMD nx_uint8_t: 0(forward) 1(clockwise) 2(backward) 3(anticlockwise) 4(stop)
    commandListToBeSent.append(durationMilliseconds); //CMD_DURATION
    commandListToBeSent.append(254); //ACK - we don't care
    commandListToBeSent.append(254); //REQ_INFO - we don't care
    commandListToBeSent.append(254); //CURRENT_X - we don't care
    commandListToBeSent.append(254); //CURRENT_Y - we don't care
    commandListToBeSent.append(254); //HIT_WALL - we don't care
    sendMessage(commandListToBeSent);

}

/**
  * rotateAnticlockwise: Rotates vehicle anticlockwise
  *
  */
void RemoteWidget::rotateAnticlockwise()
{
    qDebug() << "Rotating anticlockwise";


    QList<int> commandListToBeSent;

    //Here, we construct our command list to be sent using sendMessage.
    commandListToBeSent.append(254); //0 = auto, 1 = manual
    commandListToBeSent.append(3); //CMD nx_uint8_t: 0(forward) 1(clockwise) 2(backward) 3(anticlockwise) 4(stop)
    commandListToBeSent.append(durationMilliseconds); //CMD_DURATION
    commandListToBeSent.append(254); //ACK - we don't care
    commandListToBeSent.append(254); //REQ_INFO - we don't care
    commandListToBeSent.append(254); //CURRENT_X - we don't care
    commandListToBeSent.append(254); //CURRENT_Y - we don't care
    commandListToBeSent.append(254); //HIT_WALL - we don't care
    sendMessage(commandListToBeSent);

}

/**
  * rotateClockwise: Rotates vehicle clockwise
  *
  */
void RemoteWidget::rotateClockwise()
{

    qDebug() << "Rotating clockwise ";


    QList<int> commandListToBeSent;

    //Here, we construct our command list to be sent using sendMessage.
    commandListToBeSent.append(254); //0 = auto, 1 = manual
    commandListToBeSent.append(1); //CMD nx_uint8_t: 0(forward) 1(clockwise) 2(backward) 3(anticlockwise) 4(stop)
    commandListToBeSent.append(durationMilliseconds); //CMD_DURATION
    commandListToBeSent.append(254); //ACK - we don't care
    commandListToBeSent.append(254); //REQ_INFO - we don't care
    commandListToBeSent.append(254); //CURRENT_X - we don't care
    commandListToBeSent.append(254); //CURRENT_Y - we don't care
    commandListToBeSent.append(254); //HIT_WALL - we don't care
    sendMessage(commandListToBeSent);

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
