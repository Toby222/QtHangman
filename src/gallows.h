#ifndef GALLOWS_H
#define GALLOWS_H

#include <QColor>
#include <QGraphicsItem>

class Gallows : public QGraphicsObject
{
    Q_OBJECT
public:
    Gallows();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    int increasePartCount(int num = 1);
    void reset();

    static const int PART_COUNT = 11;
private:
    int currentParts = 0;


signals:

};

#endif // GALLOWS_H
