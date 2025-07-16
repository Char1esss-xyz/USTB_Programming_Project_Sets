#include "playerexplosion.h"
#include <QGraphicsScene>

PlayerExplosion::PlayerExplosion(QPointF position, QGraphicsScene *scene)
{
    loadFrames();
    if (m_frames.isEmpty()) return;

    // 使用第一帧尺寸定位位置
    int frameWidth = m_frames.first().width();
    int frameHeight = m_frames.first().height();
    setPixmap(m_frames[0]);
    setPos(position.x() - frameWidth / 2, position.y() - frameHeight / 2);  // 居中显示
    scene->addItem(this);

    // 设置定时器播放动画
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &PlayerExplosion::onAnimationTimeout);
    m_animationTimer->start(100);
}

PlayerExplosion::~PlayerExplosion()
{
    if (m_animationTimer) {
        m_animationTimer->stop();
        delete m_animationTimer;
        m_animationTimer = nullptr;
    }
}

void PlayerExplosion::loadFrames()
{
    // 添加所有帧
    m_frames.append(QPixmap(":/images/images/me_destroy_1.png"));
    m_frames.append(QPixmap(":/images/images/me_destroy_2.png"));
    m_frames.append(QPixmap(":/images/images/me_destroy_3.png"));
    m_frames.append(QPixmap(":/images/images/me_destroy_4.png"));

}

void PlayerExplosion::onAnimationTimeout()
{
    m_frameIndex++;
    if (m_frameIndex < m_frames.size()) {
        setPixmap(m_frames[m_frameIndex]);  // 切换帧
    } else {
        // 动画播放完毕，延迟删除对象
        this->deleteLater();  // 安全删除

        // 触发游戏结束
        emit playerExplosionFinished();
    }
}
