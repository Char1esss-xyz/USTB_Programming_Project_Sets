#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

class GameView : public QGraphicsView
{
    Q_OBJECT

public:
    GameView(QWidget *parent = nullptr);
    void addScore(int points);
    void decreaseHealth(int damage);
    int getHealth() const;
    void restartGame();
    void pauseGame();   // 暂停函数
    void resumeGame();
    bool isPaused() const { return m_isPaused; }

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QGraphicsScene *m_scene;
    QGraphicsPixmapItem *m_player;        // 玩家图标
    QGraphicsPixmapItem *m_background1;  // 背景图1
    QGraphicsPixmapItem *m_background2;  // 背景图2
    int m_playerHorizSpeed = 0;   // 水平速度（左右）
    int m_playerVertSpeed = 0;    // 垂直速度（上下）
    qreal m_playerX = 0;          // 当前X坐标
    qreal m_playerY = 0;          // 当前Y坐标
    int m_backgroundHeight = 0;   // 背景高度
    void updatePlayerPosition();  // 更新飞机位置
    QTimer *m_spawnTimer;         // 敌人生成定时器
    QTimer *m_playerTimer;        // 计时器
    QTimer *m_scrollTimer;       // 地图滚动定时器
    QTimer *m_difficultyTimer;   // 游戏难度定时器
    int m_spawnInterval;        // 敌人生成间隔（毫秒）
    int m_score = 0;   // 分数
    int m_health = 5;  // 初始生命值设为5
    QList<QGraphicsPixmapItem*> m_lifeIcons;  // 存储生命值图标
    int m_lifeSize = 0; // 存储生命图标大小
    QGraphicsTextItem *m_scoreText;
    QFont m_scoreFont;
    void spawnEnemy();           // 生成敌人
    void shootBullet();         //发射子弹
    void updateScroll();        // 地图滚动逻辑
    void checkCollisions();  // 添加这一行
    void checkPlayerEnemyCollision();  // 检测玩家与敌人碰撞
    void checkBulletEnemyCollisions();  // 检测子弹与敌人碰撞
    void checkEnemyBulletPlayerCollisions();  // 检测敌人子弹与玩家碰撞
    void gameOver();
    void updateLifeIcons();
    void spawnSupply(QPointF position);
    void checkPlayerSupplyCollision();
    void addHealth(int points);
    void increaseDifficulty();
    QGraphicsPixmapItem *m_gameOverImage = nullptr;  // 游戏结束图片
    QGraphicsPixmapItem *m_restartButton = nullptr;  // 重新开始按钮
    bool m_isPaused = false;  // 暂停状态标志
    QGraphicsPixmapItem *m_pauseButton = nullptr;   // 暂停按钮
    QGraphicsPixmapItem *m_resumeButton = nullptr; // 恢复按钮
    QList<QTimer*> m_gameTimers;  // 游戏中所有定时器（用于暂停）

private slots:
    void onRestartButtonClicked();  // 按钮点击槽函数

signals:
    void playerExplosionFinished();
};


#endif // GAMEVIEW_H
