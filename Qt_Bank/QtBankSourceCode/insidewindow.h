#ifndef INSIDEWINDOW_H
#define INSIDEWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class InsideWindow;
}

class InsideWindow : public MainWindow
{
    Q_OBJECT

public:
    explicit InsideWindow(QWidget *parent = nullptr);
    ~InsideWindow();

private slots:
    void switchInsideWindow();

    void on_PushButtonCreateAccount_clicked();        // 创建账户按钮
    void on_PushButtonDepositMoney_clicked();         // 存款按钮
    void on_PushButtonWithdrawMoney_clicked();        // 取款按钮
    void on_PushButtonConfirmDate_clicked();          // 确认日期按钮
    void on_PushButtonNextMonth_clicked();            // 进入下个月按钮
    void on_PushButtonQueryAccount_clicked();         // 查询账户信息按钮
    void on_PushButtonQueryTransaction_clicked();     // 查询时间段账目按钮

private:
    Ui::InsideWindow *ui;
    static InsideWindow *iw;
    static void backMain(char cmd, bool is_now_reading_a_file);
    static void updateFinancialSummary();
};

#endif // INSIDEWINDOW_H
