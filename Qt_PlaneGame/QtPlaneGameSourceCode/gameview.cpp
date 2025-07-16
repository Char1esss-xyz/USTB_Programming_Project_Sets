#include "gameview.h"
#include "enemy.h"
#include "bullet.h"
#include "enemyexplosion.h"
#include "playerexplosion.h"
#include "supply.h"
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QApplication>

GameView::GameView(QWidget *parent)
    : QGraphicsView(parent), m_scene(new QGraphicsScene(this))
{
    // 加载背景图
    QPixmap background(":/images/images/background.jpg");
    int backgroundWidth = background.width();
    m_backgroundHeight = background.height();

    // 设置场景大小
    m_scene->setSceneRect(0, 0, backgroundWidth, m_backgroundHeight);

    // 设置窗口大小
    setFixedSize(backgroundWidth, m_backgroundHeight);
    setScene(m_scene);

    // 隐藏滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 添加两个背景图（一个在上，一个在下）
    m_background1 = m_scene->addPixmap(background);
    m_background2 = m_scene->addPixmap(background);
    m_background1->setPos(0, 0);
    m_background2->setPos(0, -m_backgroundHeight);  // 第二张背景图位于第一张上方

    // 加载玩家飞机图片并获取尺寸
    QPixmap playerPixmap(":/images/images/me.png");
    int playerWidth = playerPixmap.width();
    int playerHeight = playerPixmap.height();

    // 计算居中位置
    int centerX = (backgroundWidth - playerWidth) / 2;
    int bottomY = m_backgroundHeight - playerHeight;

    // 创建并添加玩家飞机
    m_player = m_scene->addPixmap(playerPixmap);
    m_player->setPos(centerX, bottomY);  // 设置新位置
    m_playerX = centerX;
    m_playerY = bottomY;

    m_playerTimer = new QTimer(this);
    connect(m_playerTimer, &QTimer::timeout, this, &GameView::updatePlayerPosition);
    m_playerTimer->start(30);  // 每30ms更新一次

    m_spawnInterval = 1000;
    m_spawnTimer = new QTimer(this);
    connect(m_spawnTimer, &QTimer::timeout, this, &GameView::spawnEnemy);
    m_spawnTimer->start(m_spawnInterval);

    m_difficultyTimer = new QTimer(this);
    connect(m_difficultyTimer, &QTimer::timeout, this, &GameView::increaseDifficulty);
    m_difficultyTimer->start(10000);  // 每10秒提升一次难度

    m_scrollTimer = new QTimer(this);
    connect(m_scrollTimer, &QTimer::timeout, this, &GameView::updateScroll);
    m_scrollTimer->start(30);  // 每30ms更新一次

    QTimer *collisionTimer = new QTimer(this);
    connect(collisionTimer, &QTimer::timeout, this, &GameView::checkCollisions);
    collisionTimer->start(30);  // 每30ms检查一次

    QPixmap lifePixmap(":/images/images/life.png");
    m_lifeSize = lifePixmap.width();  // 获取生命图片宽度

    // 显示生命图标
    for (int i = 0; i < m_health; ++i) {
        QGraphicsPixmapItem *lifeIcon = m_scene->addPixmap(lifePixmap);
        lifeIcon->setPos(10 + i * m_lifeSize, 10);  // 水平排列
        m_lifeIcons.append(lifeIcon);
    }

    // 初始化得分显示
    m_scoreFont.setPointSize(20);
    m_scoreFont.setBold(true);
    m_scoreText = m_scene->addText("Score: 0", m_scoreFont);
    m_scoreText->setDefaultTextColor(Qt::red);

    // 设置得分位置
    m_scoreText->setPos(10, 10 + lifePixmap.height());

    // 连接爆炸动画结束信号
    connect(this, &GameView::playerExplosionFinished, this, &GameView::gameOver);

    // 初始化暂停按钮
    QPixmap pausePixmap(":/images/images/pause.png");
    if (!pausePixmap.isNull()) {
        m_pauseButton = m_scene->addPixmap(pausePixmap);
        int sceneWidth = m_scene->width();
        int btnWidth = pausePixmap.width();
        m_pauseButton->setPos(sceneWidth - btnWidth - 10, 10);  // 右上角（距离右边距10px）
        m_pauseButton->setAcceptHoverEvents(true);
        m_pauseButton->setData(0, "pause_button");
    }

    // 初始化恢复按钮
    QPixmap resumePixmap(":/images/images/resume.png");
    if (!resumePixmap.isNull()) {
        m_resumeButton = m_scene->addPixmap(resumePixmap);
        int sceneWidth = m_scene->width();
        int btnWidth = resumePixmap.width();
        m_resumeButton->setPos(sceneWidth - btnWidth - 10, 10);
        m_resumeButton->setAcceptHoverEvents(true);
        m_resumeButton->setData(0, "resume_button");
        m_resumeButton->setVisible(false);  // 默认隐藏
    }

    // 收集所有定时器
    m_gameTimers << m_spawnTimer << m_scrollTimer << m_playerTimer << m_difficultyTimer;
}

void GameView::keyPressEvent(QKeyEvent *event)
{
    if (m_isPaused) return;
    if (event->key() == Qt::Key_W) {
        m_playerVertSpeed = -5;  // A 键：向上移动
    } else if (event->key() == Qt::Key_S) {
        m_playerVertSpeed = +5;  // D 键：向下移动
    } else if (event->key() == Qt::Key_A) {
        m_playerHorizSpeed = -5; // 左移
    } else if (event->key() == Qt::Key_D) {
        m_playerHorizSpeed = +5; // 右移
    } else if (event->key() == Qt::Key_Space) {
        shootBullet();  // 空格键发射子弹
    }
}

void GameView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_S) {
        m_playerVertSpeed = 0;  // 松开 W/S 键时停止垂直移动
    }
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D) {
        m_playerHorizSpeed = 0; // 松开左右键时停止水平移动
    }
}

void GameView::updatePlayerPosition()
{
    bool moved = false;

    if (m_playerHorizSpeed != 0) {
        m_playerX += m_playerHorizSpeed;
        moved = true;
    }

    if (m_playerVertSpeed != 0) {
        m_playerY += m_playerVertSpeed;
        moved = true;
    }

    if (moved) {
        int playerWidth = m_player->pixmap().width();
        int playerHeight = m_player->pixmap().height();
        qreal sceneWidth = m_scene->sceneRect().width();
        qreal sceneHeight = m_scene->sceneRect().height();

        // 边界检测（X轴）
        if (m_playerX < 0) m_playerX = 0;
        else if (m_playerX > sceneWidth - playerWidth) m_playerX = sceneWidth - playerWidth;

        // 边界检测（Y轴）
        if (m_playerY < 0) m_playerY = 0;
        else if (m_playerY > sceneHeight - playerHeight) m_playerY = sceneHeight - playerHeight;

        m_player->setPos(m_playerX, m_playerY);
    }
}

void GameView::spawnEnemy()
{
    Enemy *enemy = new Enemy(m_scene);
    enemy->startFalling();
    enemy->startShooting();  // 随机射击
    m_scene->addItem(enemy);

    if (m_isPaused) {
        enemy->fallingTimer()->stop();
        enemy->shootingTimer()->stop();
    }
}

void GameView::shootBullet()
{
    // 获取玩家飞机中心点
    qreal playerX = m_player->x() + m_player->pixmap().width() / 2;
    qreal playerY = m_player->y();

    // 创建子弹并设置初始位置
    Bullet *bullet = new Bullet(QPointF(playerX, playerY), -1);
    bullet->startMoving();
    m_scene->addItem(bullet);
}

void GameView::updateScroll()
{
    int scrollSpeed = 2;  // 滚动速度

    // 向下移动两张背景图
    m_background1->moveBy(0, scrollSpeed);
    m_background2->moveBy(0, scrollSpeed);

    // 检查是否超出屏幕，若超出则重置位置
    if (m_background1->y() >= m_backgroundHeight) {
        m_background1->setY(-m_backgroundHeight);
    }
    if (m_background2->y() >= m_backgroundHeight) {
        m_background2->setY(-m_backgroundHeight);
    }
}

void GameView::checkBulletEnemyCollisions()
{
    // 获取所有玩家子弹
    QList<Bullet*> bullets;
    for (QGraphicsItem* item : m_scene->items()) {
        if (Bullet* bullet = dynamic_cast<Bullet*>(item)) {
            if (bullet->direction() == -1) {
                bullets.append(bullet);
            }
        }
    }

    // 遍历子弹并检测碰撞
    for (Bullet* bullet : bullets) {
        if (!bullet->scene()) continue;

        QList<QGraphicsItem*> colliding = bullet->collidingItems();
        for (QGraphicsItem* target : colliding) {
            if (Enemy* enemy = dynamic_cast<Enemy*>(target)) {
                m_scene->removeItem(bullet);
                m_scene->removeItem(enemy);

                // 创建爆炸动画
                new Explosion(enemy->pos(), m_scene);
                // 创建补给
                spawnSupply(enemy->pos());

                delete bullet;
                delete enemy;
                addScore(10);
                break;
            }
        }
    }
}

void GameView::checkPlayerEnemyCollision()
{
    // 检查玩家是否仍然存在
    if (!m_player || !m_player->scene()) {
        return;  // 玩家已销毁，停止检测
    }

    // 获取所有碰撞项
    QList<QGraphicsItem*> colliding = m_player->collidingItems();
    for (QGraphicsItem* item : colliding) {
        if (Enemy* enemy = dynamic_cast<Enemy*>(item)) {
            m_scene->removeItem(enemy);
            delete enemy;
            decreaseHealth(1);  // 扣除生命值

            if (getHealth() <= 0) {
                gameOver();  // 游戏结束
            }
        }
    }
}

void GameView::checkEnemyBulletPlayerCollisions()
{
    // 先获取所有敌人子弹的副本
    QList<Bullet*> enemyBullets;
    for (QGraphicsItem* item : m_scene->items()) {
        if (Bullet* bullet = dynamic_cast<Bullet*>(item)) {
            if (bullet->direction() == +1) {  // 敌人子弹
                enemyBullets.append(bullet);
            }
        }
    }

    // 检查玩家是否仍然存在
    if (!m_player || !m_player->scene()) {
        return;  // 玩家已销毁，停止检测
    }

    // 遍历敌人子弹并检测碰撞
    for (Bullet* bullet : enemyBullets) {
        // 检查子弹是否废除边界
        if (!bullet->scene()) {
            continue;  // 子弹已被销毁
        }
        // 检查子弹是否与玩家碰撞
        if (bullet->collidesWithItem(m_player)) {
            m_scene->removeItem(bullet);
            delete bullet;

            decreaseHealth(1);  // 扣除生命值
            if (getHealth() <= 0) {
                gameOver();  // 游戏结束
            }
        }
    }
}

void GameView::checkPlayerSupplyCollision()
{
    if (!m_player || !m_player->scene()) {
        return;
    }

    // 获取所有补给道具
    QList<QGraphicsItem*> colliding = m_player->collidingItems();
    for (QGraphicsItem* item : colliding) {
        Supply* supply = dynamic_cast<Supply*>(item);
        if (supply && supply->scene()) {
            m_scene->removeItem(supply);
            delete supply;
            addHealth(1);  // 恢复1点生命值
        }
    }
}

void GameView::checkCollisions()
{
    checkBulletEnemyCollisions();        // 子弹与敌人碰撞
    checkPlayerEnemyCollision();        // 玩家与敌人碰撞
    checkEnemyBulletPlayerCollisions(); // 敌人子弹与玩家碰撞
    checkPlayerSupplyCollision();      // 玩家与补给碰撞
}

void GameView::decreaseHealth(int damage)
{
    m_health -= damage;
    updateLifeIcons();
    m_scoreText->setPlainText(QString("Score: %1").arg(m_score));

    if (m_health <= 0) {
        // 播放玩家爆炸动画
        if (m_player && m_player->scene()) {
            new PlayerExplosion(m_player->pos(), m_scene);
            m_scene->removeItem(m_player);
            delete m_player;
            m_player = nullptr;
        }

        emit playerExplosionFinished();  // 触发游戏结束
    }
}

void GameView::addScore(int points)
{
    m_score += points;
    m_scoreText->setPlainText(QString("Score: %1").arg(m_score));
}

int GameView::getHealth() const
{
    return m_health;
}

void GameView::gameOver()
{
    // 停止所有定时器
    if (m_spawnTimer) m_spawnTimer->stop();
    if (m_scrollTimer) m_scrollTimer->stop();
    if (m_playerTimer) m_playerTimer->stop();
    if (m_difficultyTimer) m_difficultyTimer->stop();

    // 清除所有敌人和子弹
    QList<QGraphicsItem*> items = m_scene->items();
    for (QGraphicsItem* item : items) {
        if (dynamic_cast<Enemy*>(item) || dynamic_cast<Bullet*>(item)) {
            m_scene->removeItem(item);
            delete item;
        }
    }

    if (!m_gameOverImage) {
        QPixmap gameOverPixmap(":/images/images/gameover.png");
        if (!gameOverPixmap.isNull()) {
            m_gameOverImage = m_scene->addPixmap(gameOverPixmap);
            int sceneWidth = m_scene->width();
            int sceneHeight = m_scene->height();
            int imgWidth = gameOverPixmap.width();
            int imgHeight = gameOverPixmap.height();
            m_gameOverImage->setPos((sceneWidth - imgWidth) / 2, (sceneHeight - imgHeight) / 2 - 50);
            m_gameOverImage->setAcceptHoverEvents(true);
            m_gameOverImage->setData(0, "gameover_image");
        }
    }

    if (!m_restartButton) {
        QPixmap restartPixmap(":/images/images/again.png");
        if (!restartPixmap.isNull()) {
            m_restartButton = m_scene->addPixmap(restartPixmap);
            int sceneWidth = m_scene->width();
            int sceneHeight = m_scene->height();
            int btnWidth = restartPixmap.width();
            int btnHeight = restartPixmap.height();
            m_restartButton->setPos((sceneWidth - btnWidth) / 2, (sceneHeight + btnHeight) / 2);
            m_restartButton->setAcceptHoverEvents(true);
            m_restartButton->setData(0, "restart_button");
        }
    }
}

void GameView::updateLifeIcons()
{
    // 清除旧的生命值图标
    for (QGraphicsPixmapItem *icon : m_lifeIcons) {
        m_scene->removeItem(icon);
        delete icon;
    }
    m_lifeIcons.clear();

    // 重新创建生命值图标
    QPixmap lifePixmap(":/images/images/life.png");
    int m_lifeSize = lifePixmap.width();

    for (int i = 0; i < m_health; ++i) {
        QGraphicsPixmapItem *lifeIcon = m_scene->addPixmap(lifePixmap);
        lifeIcon->setPos(10 + i * (m_lifeSize + 10), 10);
        m_lifeIcons.append(lifeIcon);
    }

    // 更新得分文本位置
    if (m_scoreText) {
        m_scoreText->setFont(m_scoreFont);
        m_scoreText->setPos(10,10 + lifePixmap.height());
    }
}

void GameView::onRestartButtonClicked()
{
    // 移除游戏结束图标
    if (m_gameOverImage) {
        m_scene->removeItem(m_gameOverImage);
        delete m_gameOverImage;
        m_gameOverImage = nullptr;
    }

    // 移除重启图标
    if (m_restartButton) {
        m_scene->removeItem(m_restartButton);
        delete m_restartButton;
        m_restartButton = nullptr;
    }

    // 重置所有游戏状态
    m_score = 0;
    m_health = 5;
    updateLifeIcons();
    m_scoreText->setPlainText("Score: 0");

    // 重新生成玩家飞机
    QPixmap playerPixmap(":/images/images/me.png");
    int playerWidth = playerPixmap.width();
    int playerHeight = playerPixmap.height();
    int backgroundWidth = m_scene->sceneRect().width();
    int backgroundHeight = m_scene->sceneRect().height();
    int centerX = (backgroundWidth - playerWidth) / 2;
    int bottomY = backgroundHeight - playerHeight;

    // 安全释放旧的玩家飞机
    if (m_player) {
        m_scene->removeItem(m_player);
        delete m_player;
    }

    m_player = m_scene->addPixmap(playerPixmap);
    m_player->setPos(centerX, bottomY);
    m_playerX = centerX;
    m_playerY = bottomY;

    // 重新启动定时器
    if (!m_spawnTimer->isActive()) m_spawnTimer->start(1500);
    if (!m_scrollTimer->isActive()) m_scrollTimer->start(30);
    if (!m_playerTimer->isActive()) m_playerTimer->start(30);
}

void GameView::restartGame()
{
    onRestartButtonClicked();  // 调用重启逻辑
}

void GameView::mousePressEvent(QMouseEvent *event)
{
    QPointF clickPos = mapToScene(event->pos());

    // 检查是否点击了暂停按钮
    if (m_pauseButton && m_pauseButton->scene() && m_pauseButton->isVisible()) {
        if (m_pauseButton->contains(m_pauseButton->mapFromScene(clickPos))) {
            pauseGame();  // 触发暂停
            return;
        }
    }

    // 检查是否点击了恢复按钮
    if (m_resumeButton && m_resumeButton->scene() && m_resumeButton->isVisible()) {
        if (m_resumeButton->contains(m_resumeButton->mapFromScene(clickPos))) {
            resumeGame();  // 触发恢复
            return;
        }
    }

    // 检查是否点击了重启按钮
    if (m_restartButton && m_restartButton->scene()) {
        if (m_restartButton->contains(m_restartButton->mapFromScene(clickPos))) {
            restartGame();
            return;
        }
    }

    // 检查是否点击了退出按钮
    if (m_gameOverImage && m_gameOverImage->scene()) {
        if (m_gameOverImage->contains(m_gameOverImage->mapFromScene(clickPos))) {
            QApplication::quit();
            return;
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void GameView::spawnSupply(QPointF position)
{
    // 50% 概率掉落补给
    if (QRandomGenerator::global()->bounded(2) == 0) {
        Supply* supply = new Supply(position, m_scene);
        m_scene->addItem(supply);
    }
}

void GameView::addHealth(int points)
{
    m_health += points;
    if (m_health > 5) m_health = 5;  // 限制最大生命值为5
    updateLifeIcons();
    m_scoreText->setPlainText(QString("Score: %1").arg(m_score));
}

void GameView::pauseGame()
{
    if (m_isPaused) return;

    // 停止所有定时器（包括玩家、敌人、子弹）
    for (QTimer* timer : m_gameTimers) {
        if (timer->isActive()) {
            timer->stop();
        }
    }

    // 停止所有敌人和子弹的定时器
    for (QGraphicsItem* item : m_scene->items()) {
        if (Enemy* enemy = dynamic_cast<Enemy*>(item)) {
            if (enemy->fallingTimer()->isActive()) {
                enemy->fallingTimer()->stop();
            }
            if (enemy->shootingTimer()->isActive()) {
                enemy->shootingTimer()->stop();
            }
        }

        if (Bullet* bullet = dynamic_cast<Bullet*>(item)) {
            if (bullet->moveTimer()->isActive()) {
                bullet->moveTimer()->stop();
            }
        }

        if (Supply* supply = dynamic_cast<Supply*>(item)) {
            if (supply->moveTimer()->isActive()) {
                supply->moveTimer()->stop();
            }
        }
    }

    // 显示恢复按钮，隐藏暂停按钮
    if (m_pauseButton) m_pauseButton->setVisible(false);
    if (m_resumeButton) m_resumeButton->setVisible(true);

    m_isPaused = true;
}

void GameView::resumeGame()
{
    if (!m_isPaused) return;

    // 重启所有主定时器
    for (QTimer* timer : m_gameTimers) {
        if (!timer->isActive()) {
            timer->start();  // 恢复定时器
        }
    }

    // 重启所有敌人和子弹的定时器
    for (QGraphicsItem* item : m_scene->items()) {
        if (Enemy* enemy = dynamic_cast<Enemy*>(item)) {
            if (!enemy->fallingTimer()->isActive()) {
                enemy->fallingTimer()->start(30);
            }
            if (!enemy->shootingTimer()->isActive()) {
                enemy->shootingTimer()->start(enemy->shootInterval());
            }
        }

        if (Bullet* bullet = dynamic_cast<Bullet*>(item)) {
            if (!bullet->moveTimer()->isActive()) {
                bullet->moveTimer()->start(30);
            }
        }

        if (Supply* supply = dynamic_cast<Supply*>(item)) {
            if (!supply->moveTimer()->isActive()) {
                supply->moveTimer()->start(30);
            }
        }
    }

    // 切换按钮显示
    if (m_pauseButton) m_pauseButton->setVisible(true);
    if (m_resumeButton) m_resumeButton->setVisible(false);

    m_isPaused = false;
}

void GameView::increaseDifficulty()
{
    if (m_spawnInterval > 200) {  // 设置最小间隔为200ms
        m_spawnInterval -= 100;   // 每次减少100ms

        m_spawnTimer->stop();
        m_spawnTimer->setInterval(m_spawnInterval);
        m_spawnTimer->start();
    }
}
