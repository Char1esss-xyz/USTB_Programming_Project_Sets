#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Bullet : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Bullet(QPointF startPos, int direction);  // direction: +1（下） / -1（上）
    void startMoving();
    int direction();
    QTimer* moveTimer() { return m_moveTimer; }

private:
    QTimer *m_moveTimer;
    int m_speed = 10;  // 子弹速度
    int m_direction;   // 移动方向
};

#endif // BULLET_H
