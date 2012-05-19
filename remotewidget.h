#ifndef REMOTEWIDGET_H
#define REMOTEWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QGraphicsView>
#include <QPushButton>
#include "udpReceiver.h"
#include "udpSender.h"
#include "QBoardView.h"
#include <QProcess>

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

public slots:
    void handleMoteResponse();
signals:
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


};

#endif // REMOTEWIDGET_H
