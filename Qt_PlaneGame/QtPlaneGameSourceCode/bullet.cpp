#include "bullet.h"
#include <QGraphicsScene>

Bullet::Bullet(QPointF startPos, int direction)
    : m_direction(direction)
{
    QPixmap bulletPixmap(":/images/images/bullet1.png");
    setPixmap(bulletPixmap);

    setPos(startPos);

    m_moveTimer = new QTimer(this);
    connect(m_moveTimer, &QTimer::timeout, this, [=]() {
        moveBy(0, m_speed * m_direction);  // 根据方向移动

        // 检查是否移出屏幕（根据方向判断）
        if ((m_direction == -1 && y() < -pixmap().height()) ||
            (m_direction == +1 && y() > scene()->height())) {
            scene()->removeItem(this);
            delete this;
        }
    });
}

void Bullet::startMoving()
{
    m_moveTimer->start(30);
}

int Bullet::direction()
{
    return m_direction;
}
