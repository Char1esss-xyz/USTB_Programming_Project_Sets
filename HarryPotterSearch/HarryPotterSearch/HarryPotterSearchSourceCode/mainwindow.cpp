#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QUrl>
#include <QRegularExpression>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include <QFutureWatcher>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //ui初始化
    ui->setupUi(this);
    ui->textBrowser->setOpenExternalLinks(false);
    ui->textBrowser->setOpenLinks(false);

    bookFiles_ << "Quidditch_Through_the_Ages.txt"
               << "HP6_Harry_Potter_and_the_Half_Blood_Prince.txt"
               << "HP4_Harry_Potter_and_the_Goblet_of_Fire.txt"
               << "HP3_Harry_Potter_and_the_Prisoner_of_Azkaban.txt"
               << "HP0_Harry_Potter_Prequel.txt"
               << "HP7_Harry_Potter_and_the_Deathly_Hallows_Book_7.txt"
               << "HP2_Harry_Potter_and_the_Chamber_of_Secrets_Book_2.txt"
               <<  "The_Tales_of_Beedle_the_Bard.txt";

    // 章节检测正则表达式
    chapterRegex = QRegularExpression("^\\s*(?i:chapter)\\s+([a-zA-Z]+)",
                                      QRegularExpression::CaseInsensitiveOption);

    // 连接线程信号槽
    connect(&futureWatcher_, &QFutureWatcher<QList<SearchResult>>::progressValueChanged,
            this, &MainWindow::updateProgress);
    connect(&futureWatcher_, &QFutureWatcher<QList<SearchResult>>::finished,
            this, &MainWindow::searchFinished);

    // 连接错误信号
    connect(this, &MainWindow::fileError, this, [](const QString& filePath) {
        qWarning() << "Failed to open file:" << filePath;
    });

    // 连接查找结果信号
    connect(this, &MainWindow::resultFound, this, &MainWindow::handleResultFound);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // 清空结果并检查输入
    searchResults_.clear();
    ui->textBrowser->clear();
    QString searchText = ui->lineEdit->text().trimmed();
    if (searchText.isEmpty()) return;

    // 显示进度条
    ui->progressBar->setRange(0, bookFiles_.size());
    ui->progressBar->setValue(0);

    // 启动搜索
    Future = QtConcurrent::run([=]() {
        for (int i = 0; i < bookFiles_.size(); ++i) {
            if (futureWatcher_.isCanceled()) break;
            processFile(bookFiles_[i], searchText, i);
            emit futureWatcher_.progressValueChanged(i + 1);
        }
    });

    futureWatcher_.setFuture(Future);
}

void MainWindow::processFile(const QString& filePath, const QString& keyword,int fileIndex)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit fileError(filePath);
        return;
    }

    QTextStream in(&file);
    int currentPage = 0;
    int currentChapter = 0;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        // 正则表达式章节检测
        if (chapterRegex.match(line).hasMatch()) {
            currentChapter++;
            continue;
        }

        // 页码检测
        bool ok;
        int num = line.toInt(&ok);
        if (ok && num > 0 && line == QString::number(num)) {
            currentPage = num;
            continue;
        }

        // 关键字匹配
        if (line.contains(keyword, Qt::CaseInsensitive)) {
            emit resultFound({
                fileIndex,
                currentPage,
                currentChapter,
                line,
                QFileInfo(filePath).fileName()
            });
        }
    }
    file.close();
}

void MainWindow::updateProgress(int value)
{
    ui->progressBar->setValue(value);
}

void MainWindow::searchFinished()
{
    if (!searchResults_.isEmpty()) {
        ui->statusbar->showMessage(QString("Found %1 results").arg(searchResults_.size()));
    } else {
        ui->statusbar->showMessage("No results found.");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    bool ok;
    int input = ui->lineEdit_2->text().toInt(&ok);
    if (!ok || input < 1 || input > searchResults_.size()) {
        QMessageBox::warning(this, "Error", "Invalid entry number");
        return;
    }

    ui->textBrowser_2->setText(searchResults_[input - 1].line);
}

void MainWindow::on_textBrowser_anchorClicked(const QUrl &url)
{
    if (url.scheme() == "item") {
        bool ok;
        int index = url.path().toInt(&ok);
        if (ok && index >= 1 && index <= searchResults_.size()) {
            ui->textBrowser_2->setText(searchResults_[index - 1].line);
        }
    }
}

void MainWindow::handleResultFound(const SearchResult& result)
{
    searchResults_.append(result);
    int index = searchResults_.size();

    QString displayText = QString("<a href='item:%1'>%2</a> &nbsp;&nbsp; %3 &nbsp;&nbsp; Page %4 &nbsp;&nbsp; Chapter %5 &nbsp;&nbsp; %6")
                              .arg(index)
                              .arg(index)
                              .arg(result.fileName)
                              .arg(result.page)
                              .arg(result.chapter)
                              .arg(result.line.left(30) + "...");

    ui->textBrowser->append(displayText);
}
