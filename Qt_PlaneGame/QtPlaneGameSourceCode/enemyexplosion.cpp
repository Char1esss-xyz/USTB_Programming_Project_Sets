#include "enemyexplosion.h"
#include <QGraphicsScene>

Explosion::Explosion(QPointF position, QGraphicsScene *scene)
{
    QPixmap explosionPixmap(":/images/images/enemy1_down3.png");

    setPixmap(explosionPixmap);
    setPos(position);

    scene->addItem(this);

    // 设置定时器播放动画
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &Explosion::onAnimationTimeout);
    m_animationTimer->start(500);  // 播放0.5秒后自动删除
}

Explosion::~Explosion()
{
    if (m_animationTimer) {
        m_animationTimer->stop();
        delete m_animationTimer;
    }
}

void Explosion::onAnimationTimeout()
{
    scene()->removeItem(this);
    delete this;
}
