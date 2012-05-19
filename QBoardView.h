#ifndef QBOARDVIEW_H
#define QBOARDVIEW_H

#endif // QBOARDVIEW_H

#include <QGraphicsView>


class QBoardView : public QWidget {
    Q_OBJECT


public:
    QBoardView(QWidget *parent = 0);



private:
    void paintEvent(QPaintEvent* event);


public slots:
    void updateBoardColours();


private slots:



signals:

};
