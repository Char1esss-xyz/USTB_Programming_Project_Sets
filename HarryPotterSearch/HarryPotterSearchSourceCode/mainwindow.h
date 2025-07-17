#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QRegularExpression>
#include <QFutureWatcher>
#include <QString>

// 搜索结果结构体
struct SearchResult {
    int fileIndex;
    int page;
    int chapter;
    QString line;
    QString fileName;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void fileError(const QString& filePath);
    void resultFound(const SearchResult& result);

private slots:
    void on_pushButton_clicked();       // 开始搜索
    void on_pushButton_2_clicked();     // 跳转输入行数
    void on_textBrowser_anchorClicked(const QUrl &url); // 跳转点击行数
    void updateProgress(int value);      // 更新进度条
    void searchFinished();               // 搜索完成处理
    void handleResultFound(const SearchResult& result);

private:
    void processFile(const QString& filePath, const QString& keyword,int fileIndex); //处理文件

    Ui::MainWindow *ui;
    QList<QString> bookFiles_;          // 书籍文件路径列表
    QList<SearchResult> searchResults_; // 搜索结果列表

    QRegularExpression chapterRegex;     // 章节标题匹配
    QFutureWatcher<void> futureWatcher_; // 任务监视器
    QFuture<void> Future; // 任务句柄
};

#endif // MAINWINDOW_H
