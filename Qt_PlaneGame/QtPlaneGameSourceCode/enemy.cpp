#include "enemy.h"
#include "bullet.h"
#include <QRandomGenerator>
#include <QGraphicsScene>

Enemy::Enemy(QGraphicsScene *scene)
    : m_scene(scene)  // 初始化场景指针
{
    QPixmap enemyPixmap(":/images/images/enemy1.png");
    setPixmap(enemyPixmap);

    // 获取宽度
    int sceneWidth = m_scene->width();
    int randomX = QRandomGenerator::global()->bounded(sceneWidth - enemyPixmap.width());
    setPos(randomX, 0);

    m_fallTimer = new QTimer(this);
    connect(m_fallTimer, &QTimer::timeout, this, [=]() {
        if (!m_scene) {
            m_fallTimer->stop();
            delete this;
            return;
        }

        moveBy(0, m_speed);

        if (y() > m_scene->height()) {
            m_scene->removeItem(this);
            delete this;
        }
    });

    m_fallTimer->start(30);

    m_shootTimer = new QTimer(this);
    connect(m_shootTimer, &QTimer::timeout, this, [=]() {
        // 从敌人飞机中心发射子弹（向下）
        qreal bulletX = x() + pixmap().width() / 2;
        qreal bulletY = y() + pixmap().height();
        Bullet *bullet = new Bullet(QPointF(bulletX, bulletY), +1);  // +1 表示向下
        bullet->startMoving();
        m_scene->addItem(bullet);
    });
}

Enemy::~Enemy()
{
    if (m_fallTimer) {
        m_fallTimer->stop();
        delete m_fallTimer;
    }
}

void Enemy::startFalling()
{
    m_fallTimer->start(30);
}

void Enemy::startShooting()
{
    // 首次启动随机延迟
    int initialDelay = QRandomGenerator::global()->bounded(1000, 5000);  // 首次延迟1~5秒
    m_shootTimer->start(initialDelay);
}

void Enemy::shoot()
{
    // 随机决定是否发射子弹（例如 50% 概率）
    if (QRandomGenerator::global()->bounded(2) == 0) {
        // 从敌人飞机中心发射子弹（向下）
        qreal bulletX = x() + pixmap().width() / 2;
        qreal bulletY = y() + pixmap().height();
        Bullet *bullet = new Bullet(QPointF(bulletX, bulletY), +1);  // +1 表示向下
        bullet->startMoving();
        m_scene->addItem(bullet);
    }

    // 下一次发射的随机延迟（2~5秒）
    int nextDelay = QRandomGenerator::global()->bounded(2000, 5000);
    m_shootTimer->start(nextDelay);
}
