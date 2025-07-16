#include "supply.h"
#include <QGraphicsScene>

Supply::Supply(QPointF position, QGraphicsScene *scene)
{
    QPixmap supplyPixmap(":/images/images/supply.png");

    setPixmap(supplyPixmap);
    setPos(position);
    scene->addItem(this);

    m_moveTimer = new QTimer(this);
    connect(m_moveTimer, &QTimer::timeout, this, &Supply::onMoveTimeout);
    m_moveTimer->start(30);  // 每30ms下落一次
}

Supply::~Supply()
{
    if (m_moveTimer) {
        m_moveTimer->stop();
        delete m_moveTimer;
    }
}

void Supply::onMoveTimeout()
{
    moveBy(0, m_speed);  // 向下移动

    // 检查是否移出屏幕底部
    if (y() > scene()->height()) {
        scene()->removeItem(this);
        delete this;
    }
}
