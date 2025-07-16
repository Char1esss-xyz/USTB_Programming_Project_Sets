#ifndef ENEMY_H
#define ENEMY_H

#include <QRandomGenerator>
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class Enemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit Enemy(QGraphicsScene *scene);
    ~Enemy();

    void startFalling();
    void startShooting();
    QTimer* fallingTimer() const { return m_fallTimer; }
    QTimer* shootingTimer() const { return m_shootTimer; }
    int shootInterval() const { return 2000; }

private:
    QTimer *m_fallTimer;
    QTimer *m_shootTimer;
    int m_speed = 3;
    QGraphicsScene *m_scene;

    void shoot();
};

#endif
