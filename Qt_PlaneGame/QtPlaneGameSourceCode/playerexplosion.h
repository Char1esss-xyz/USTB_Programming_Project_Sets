#ifndef PLAYEREXPLOSION_H
#define PLAYEREXPLOSION_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QObject>

class PlayerExplosion : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    PlayerExplosion(QPointF position, QGraphicsScene *scene);
    ~PlayerExplosion();

private slots:
    void onAnimationTimeout();

private:
    QTimer *m_animationTimer;
    int m_frameIndex = 0;
    QList<QPixmap> m_frames;
    void loadFrames();

signals:
    void playerExplosionFinished();
};

#endif // PLAYEREXPLOSION_H
