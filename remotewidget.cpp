#include "remotewidget.h"
#include "ui_remotewidget.h"
//#include "udpReceiver.h"
//#include "udpSender.h"
#include <QTcpSocket>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QProcess>


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
    pbLeft->setText("Right");
    controlGridLayout->addWidget(pbLeft,1,0);


    /*** BOARD ***/
    QTimer *timer = new QTimer(this);
    timer->setInterval(50);
    timer->start();
    board = new QBoardView(this);
    connect(timer, SIGNAL(timeout()), board, SLOT(updateBoardColours()));

    vboxLayout->addLayout(controlGridLayout);
    vboxLayout->addWidget(board);

    //QString program = "/usr/bin/java";
    QString program = "java";

    QStringList arguments;

    arguments << "-classpath" << "/home/user/local/src/tinyos-2.x/support/sdk/java/tinyos.jar" << "net.tinyos.tools.PrintfClient" << "-comm" << "serial@/dev/ttyUSB1:iris";
    myProcess = new QProcess(parent);
    myProcess->setProcessChannelMode(QProcess::ForwardedChannels);
    myProcess->start(program, arguments);
    connect(myProcess,SIGNAL(readyRead()),this,SLOT(handleMoteResponse()));





    /*port = new QextSerialPort(QLatin1String("/dev/ttyUSB1"), QextSerialPort::EventDriven);

    port->setBaudRate(BAUD57600);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);
    //set timeouts to 500 ms
    port->setTimeout(500);

    if (port->open(QIODevice::ReadWrite) == true) {
        connect(port, SIGNAL(readyRead()), this, SLOT(messageReady()));
        connect(port, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)));
        if (!(port->lineStatus() & LS_DSR))
        {

            qDebug() << "WARNING: DEVICE INACTIVE";
        }
        qDebug() << "listening for data on" << port->portName();
    }
    else {
        qDebug() << "device failed to open:" << port->errorString();
    }*/



    //udpR = new udpReceiver();
    //udpS = new udpSender();

    //Connect up the buttons
    //connect(ui->autoManualToggle,SIGNAL(clicked()),this,SLOT(autoManualButtonClicked()));
    //connect(udpR, SIGNAL(receivedDatagram(QString)), this, SLOT(packetReceivedUpdateUI(QString)));

}

void RemoteWidget::handleMoteResponse(){
    qDebug() << "I have issues";
    qDebug() << myProcess->readLine() << "<-- received line";
    //qDebug() << myProcess->readAll() << "<-- output from program";
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
