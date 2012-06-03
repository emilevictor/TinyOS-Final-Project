#ifndef QBOARDVIEW_H
#define QBOARDVIEW_H

#endif // QBOARDVIEW_H

#include <QGraphicsView>

/** COMMAND DEFINITIONS **/

#define MODE            0
#define CMD             1
#define CMD_DURATION    2
#define ACK             3
#define REQ_INFO        4
#define CURRENT_X       5
#define CURRENT_Y       6
#define HIT_WALL        7

class QBoardView : public QWidget {
    Q_OBJECT


public:
    QBoardView(QWidget *parent = 0);



private:
    void paintEvent(QPaintEvent* event);
    QList<int>      receivedCommandList;
    QList<int>      xPixelCoords;
    QList<int>      yPixelCoords;

    int                 currentRow;


public slots:
    void updateBoardColours();
    void acceptCommandList(QList<int>);


private slots:



signals:

};
