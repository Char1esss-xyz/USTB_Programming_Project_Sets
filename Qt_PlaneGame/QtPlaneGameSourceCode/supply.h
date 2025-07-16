#ifndef SUPPLY_H
#define SUPPLY_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Supply : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit Supply(QPointF position, QGraphicsScene *scene);
    ~Supply();
    QTimer* moveTimer() { return m_moveTimer; }

private slots:
    void onMoveTimeout();

private:
    QTimer *m_moveTimer;
    int m_speed = 2;  // 下落速度
};

#endif // SUPPLY_H
