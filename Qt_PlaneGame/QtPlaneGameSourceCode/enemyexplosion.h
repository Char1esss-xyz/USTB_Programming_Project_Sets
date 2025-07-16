#ifndef ENEMYEXPLOSION_H
#define ENEMYEXPLOSION_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Explosion : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Explosion(QPointF position, QGraphicsScene *scene);
    ~Explosion();

private slots:
    void onAnimationTimeout();

private:
    QTimer *m_animationTimer;
    int m_frameIndex = 0;
    int m_maxFrames = 1;  // 当前只有1帧
};

#endif // ENEMYEXPLOSION_H
