#include "gallows.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

Gallows::Gallows()
{
}

const int OFFSET = 15;


void Gallows::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    QPen pen = painter->pen();

    pen.setWidth(2);

    // Draw lines
    QVarLengthArray<QLineF, 10> lines;

    auto addLine = [&](int x1, int y1, int x2, int y2) -> void {lines.append(QLineF(x1+OFFSET, y1+OFFSET, x2+OFFSET, y2+OFFSET));};

    // Using fallthrough to save on if-conditionals
    switch(currentParts) {
    default:
    case 11:
        // RIGHT LEG
        addLine(75, 100, 100, 125);
        Q_FALLTHROUGH();
    case 10:
        // LEFT LEG
        addLine(75, 100, 50, 125);
        Q_FALLTHROUGH();
    case 9:
        // TORSO
        addLine(75, 50, 75, 100);
        Q_FALLTHROUGH();
    case 8:
        // RIGHT ARM
        addLine(75, 50, 100, 75);
        Q_FALLTHROUGH();
    case 7:
        // LEFT ARM
        addLine(75, 50, 50, 75);
        Q_FALLTHROUGH();
    case 6:
        // HEAD
        painter->drawEllipse(QPointF(75+OFFSET, 20+OFFSET+15), 15, 15);
        Q_FALLTHROUGH();
    case 5:
        // ROPE
        addLine(75, 0, 75, 20);
        Q_FALLTHROUGH();
    case 4:
        // HOLDER OR WHATEVER THAT THING IS
        addLine(25, 25, 50, 0);
        Q_FALLTHROUGH();
    case 3:
        // HORIZONTAL POLE
        addLine(25, 0, 75, 0);
        Q_FALLTHROUGH();
    case 2:
        // POLE
        addLine(25, 150, 25, 0);
        Q_FALLTHROUGH();
    case 1:
        // HILL
        addLine(0, 150, 50, 150);
        // Would add this to case 0 but there could be no lines at that point.
        painter->drawLines(lines.data(), lines.size());
        Q_FALLTHROUGH();
    case 0:
        // CONTAINER BOX
        painter->drawRect(OFFSET-5, OFFSET-5, 150+OFFSET+5, 150+OFFSET+5);
        break;
    }
}

int Gallows::increasePartCount(int num)
{
    return currentParts += num;
}

void Gallows::reset()
{
    currentParts = 0;
}

QRectF Gallows::boundingRect() const
{
    return QRectF();
}

QPainterPath Gallows::shape() const
{
    return QPainterPath();
}

