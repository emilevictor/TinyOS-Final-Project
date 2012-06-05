#include "QBoardView.h"
#include <math.h>
#include <QImage>
#include <QDebug>


QBoardView::QBoardView(QWidget *parent) : QWidget(parent)
{

    this->setMinimumSize(640,480);
    currentRow = 0;
    update();
}


//Override the paintEvent so that we can do animation and the like.
void QBoardView::paintEvent(QPaintEvent* event)
{
    QPainter p;
    QPen penPurple;
    penPurple.setWidth(10);
    penPurple.setColor(Qt::magenta);

    QPen penRed;
    penRed.setWidth(10);
    penRed.setColor(Qt::red);

    QPen penGreen;
    penGreen.setWidth(10);
    penGreen.setColor(Qt::green);

    QPen penWhite;
    penWhite.setWidth(10);
    penWhite.setColor(Qt::white);

    QPen penBlue;
    penBlue.setWidth(10);
    penBlue.setColor(Qt::blue);

    p.begin(this);

    p.setPen(penPurple);

    //p.drawRect(0,0,640,480);
    p.fillRect(0,0,640,480,Qt::black);
    p.drawRect(0,0,640,480);

    //We need to draw 21 rows of LEDs, 15 columns

    QList<QPen>     listOfPenColours;
    listOfPenColours.append(penBlue);
    listOfPenColours.append(penWhite);
    listOfPenColours.append(penGreen);
    listOfPenColours.append(penRed);

    //Print all the coloured squares, and add them to X and Y lists of co-ordinates.
    int row = 0;
    for (int i = 35; i < 620; i=i+30)
    {

        if (xPixelCoords.length() < 20)
        {
            xPixelCoords.append(i);
        }
        int column = 0;
        for (int j = 35; j < 445; j = j + 27)
        {
            if (yPixelCoords.length() < 15)
            {
                yPixelCoords.append(j);
            }

            if ((row % 4) == 0)
            {
                p.setPen(penRed);
            } else if ((row % 4) == 1)
            {
                p.setPen(penGreen);
            } else if ((row % 4) == 2) {
                p.setPen(penBlue);
            } else {
                p.setPen(penWhite);
            }
            //p.setPen(listOfPenColours.at(rand() % 4));
            /*if (row == currentRow)
            {*/
            p.drawEllipse(i,j,1,1);
            /*}*/

            column++;
        }
        row++;
    }

    //Here we need to place the mote where it currently is.
    QImage tiBot(":/images/transparentTI.png");
    tiBot = tiBot.scaledToWidth(50);
    if (!receivedCommandList.isEmpty())
    {

        if (receivedCommandList.at(HIT_WALL) == 1)
        {
                //p.setPen(penRed);
                p.fillRect(xPixelCoords.at(receivedCommandList.at(CURRENT_X))-23,yPixelCoords.at(receivedCommandList.at(CURRENT_Y))-18,
                           49,40, Qt::red);
        }
        p.drawImage(xPixelCoords.at(receivedCommandList.at(CURRENT_X))-23,
                    yPixelCoords.at(receivedCommandList.at(CURRENT_Y))-18,tiBot);

    }
    if (currentRow++ > 19) {
        currentRow = 0;
    }


}

void QBoardView::acceptCommandList(QList<int> localCommandList) {
    //Do something with the command list.
    receivedCommandList = localCommandList;
}

void QBoardView::updateBoardColours() {

    this->update();
}
