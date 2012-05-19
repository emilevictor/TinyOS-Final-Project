#include "QBoardView.h"
#include <math.h>

QBoardView::QBoardView(QWidget *parent) : QWidget(parent)
{

    this->setMinimumSize(640,480);
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

    for (int i = 35; i < 620; i=i+30)
    {
        for (int j = 35; j < 445; j = j + 27)
        {
            p.setPen(listOfPenColours.at(rand() % 4));
            p.drawPoint(i,j);
        }
    }





}

void QBoardView::updateBoardColours() {

    this->update();
}
