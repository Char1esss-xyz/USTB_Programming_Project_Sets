#include "insidewindow.h"
#include "ui_insidewindow.h"
#include "QString"
#include "date.h"
#include "vector"
#include "Account.h"
#include "fstream"
#include "SavingsAccount.h"
#include "CreditAccount.h"
#include "stdexcept"
#include "iostream"
#include "QMessageBox"
using namespace std;

struct deleter {
    template <class T> void operator () (T* p) { delete p; }
};
InsideWindow *InsideWindow::iw = nullptr;

Date currentDate(2025, 2, 25); // 系统起始日期
fstream commands; // 命令文件流
vector<Account *> accounts ;// 账户容器
QString accountParams[5]; // 账户参数缓存（0:账户类型 1:账户名 2:信用额度 3:利率 4:年费）
QString transactionData[3]; // 交易参数缓存（0:账户索引 1:金额 2:原因）
int targetDay; // 目标日期
Date queryStartDate, endDate; // 查询日期范围

InsideWindow::InsideWindow(QWidget *parent) :
    MainWindow(parent),
    ui(new Ui::InsideWindow)
{
    // 初始化UI
    ui->setupUi(this);
    iw = this;

    // 生成欢迎信息
    QString Greeting = "用户: "+ QString::fromStdString(this->MainWindow::Log_on_User) + " 欢迎使用BOLC系统";
    ui->label->setText(Greeting);

    // 命令文件操作
    char cmd;
    commands.open("commands.txt", ios_base::in | ios_base::out);

    // 文件不存在则创建
    if (!commands.is_open()) {
        commands.open("commands.txt", ios_base::out | ios_base::app);
    } else {
        commands.seekg(0);
        // 读取历史命令
        while (commands >> cmd) {
            if (cmd == 'a' || cmd == 'd' || cmd == 'w' || cmd == 'c' ||
                cmd == 'n' || cmd == 'q' || cmd == 's') {
                InsideWindow::backMain(cmd, true);
            } else {
                // 跳过非法命令行
                cerr << "[ERROR] Invalid command in file: " << cmd << endl;
                commands.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        commands.clear();
    }

    // 初始财务摘要更新
    InsideWindow::updateFinancialSummary();
}

InsideWindow::~InsideWindow()
{
    // 释放账户资源
    for_each(accounts.begin(), accounts.end(), deleter());
    // 关闭文件流
    commands.close();
    // 释放UI资源
    delete ui;
}

// 显示内部窗口
void InsideWindow::switchInsideWindow(){
    this->show();
}

// 主命令处理函数
void InsideWindow::backMain(char cmd, bool is_now_reading_a_file){
    char type;
    unsigned int index;
    int day;
    double amount, credit, rate, fee;
    string user, id, desc;
    Account* account;
    Date date1{}, date2{};

    switch (cmd) {
    case 'a': // 创建账户
        try{
            // 文件读取或用户输入处理
            if(is_now_reading_a_file)
                commands >> type >> user >> id;
            else {
                try{
                    type = accountParams[0].toStdString().c_str()[0];
                    user = MainWindow::Log_on_User;
                    id = accountParams[1].toStdString();
                    commands << endl << cmd << " ";
                    commands << type << " " << user << " " << id << " ";
                }
                catch (runtime_error& err) {
                    cerr << err.what() << endl;
                    break;
                }
            }

            // 创建储蓄账户
            if (type == 's') {
                if(is_now_reading_a_file) commands >> rate;
                else {
                    rate = accountParams[3].toDouble();
                    commands << rate;
                }
                account = new SavingsAccount(currentDate, user , id, rate);
                if(user == MainWindow::Log_on_User)
                    iw->ui->textBrowser1->append(QString::fromStdString(user+"创建了 "+id+"账户。"));
                accounts.push_back(account);
            }
            // 创建信用卡账户
            else if(type == 'c'){
                if(is_now_reading_a_file) commands >> credit >> rate >> fee;
                else {
                    credit = accountParams[2].toDouble();
                    rate = accountParams[3].toDouble();
                    fee = accountParams[4].toDouble();
                    commands << credit << " " << rate << " " << fee;
                }
                account = new CreditAccount(currentDate, user, id, credit, rate, fee);
                if(user == MainWindow::Log_on_User)
                    iw->ui->textBrowser1->append(QString::fromStdString(user+"创建了 "+id+"账户。"));
                accounts.push_back(account);
            }
            else{
                throw std::runtime_error("Not a saving or credit account.");
            }
        }catch (runtime_error& err) {
            cerr << err.what() << endl;
            break;
        }
        break;

    case 'd': // 存款操作
        try{
            if(is_now_reading_a_file){
                commands >> index >> amount;
                getline(commands, desc);
            }
            else{
                index = transactionData[0].toUInt();
                amount = transactionData[1].toDouble();
                desc = transactionData[2].toStdString();
                commands << endl << cmd << " ";
                commands << index << " " << amount << " " << desc;
                commands.flush();
            }

            // 账户有效性检查
            if (index >= accounts.size() || accounts[index] == nullptr) {
                throw runtime_error("The index you input is not exist.");
            }

            // 执行存款操作
            accounts[index]->deposit(currentDate, amount, desc);

            // UI日志更新
            if (accounts[index]->getUser() == MainWindow::Log_on_User) {
                QString log = QString("%1年%2月%3日%4存了%5元,原因为：%6")
                                  .arg(currentDate.getYear())
                                  .arg(currentDate.getMonth())
                                  .arg(currentDate.getDay())
                                  .arg(QString::fromStdString(accounts[index]->getId()))
                                  .arg(amount, 0, 'f', 2)
                                  .arg(QString::fromStdString(desc));
                iw->ui->textBrowser1->append(log);
            }

            // 财务摘要更新
            InsideWindow::updateFinancialSummary();
        }
        catch (runtime_error& err) {
            cerr << err.what() << endl;
        }
        break;

    case 'w': // 取款操作
        try {
            // 参数获取
            if (is_now_reading_a_file) {
                commands >> index >> amount;
                getline(commands, desc);
            } else {
                index = transactionData[0].toUInt();
                amount = transactionData[1].toDouble();
                desc = transactionData[2].toStdString();
                if(index >= accounts.size()){
                    throw runtime_error("The index you input is not exist.");
                }
                commands << endl << cmd << " ";
                commands << index << " " << amount << " " << desc;
                commands.flush();
            }

            // 账户有效性检查
            if (index >= accounts.size() || accounts[index] == nullptr) {
                cerr << "[ERROR] Invalid account index: " << index << " (size: " << accounts.size() << ")" << endl;
            }

            // 执行取款操作
            accounts[index]->withdraw(currentDate, amount, desc);

            // UI日志更新
            if (accounts[index]->getUser() == MainWindow::Log_on_User) {
                QString log = QString("%1年%2月%3日%4取了%5元,原因为：%6")
                                  .arg(currentDate.getYear())
                                  .arg(currentDate.getMonth())
                                  .arg(currentDate.getDay())
                                  .arg(QString::fromStdString(accounts[index]->getId()))
                                  .arg(amount, 0, 'f', 2)
                                  .arg(QString::fromStdString(desc));
                iw->ui->textBrowser1->append(log);
                InsideWindow::updateFinancialSummary();
            }
        } catch (AccountException& err) {
            cerr << err.what() << endl;
            QMessageBox::warning(iw, tr("警告信息"), tr("余额不足"));
        }
        break;

    case 's': // 账户查询
        static unsigned int num;
        num = 1;
        // 遍历显示所有账户信息
        for (size_t i = 0; i < accounts.size(); i++) {
            if(accounts[i]->getUser() == MainWindow::Log_on_User)
                iw->ui->textBrowser1->append(QString::fromStdString("[" + to_string(num++) + "] " + accounts[i]->show()));
        }
        break;

    case 'c': // 日期修改
        if(is_now_reading_a_file) commands >> day;
        else {
            day = targetDay;
        }

        // 日期有效性校验
        if (day < currentDate.getDay()){
            cerr << "Invalid Date" << endl;
            QMessageBox::warning(iw,tr("警告信息"),tr("日期无效"));
        }
        else if (day > currentDate.getMaxDay()){
            cerr << "There's not a such day" << endl;
            QMessageBox::warning(iw,tr("警告信息"),tr("这个月没有这一天"));
        }
        else {
            // 更新当前日期
            currentDate = Date(currentDate.getYear(), currentDate.getMonth(), day);

            // 非文件操作时更新日志
            if(!is_now_reading_a_file) {
                commands << endl << cmd << " ";
                commands << day;
                QString log = QString("更改成功，今天是%1年%2月%3日")
                                  .arg(currentDate.getYear())
                                  .arg(currentDate.getMonth())
                                  .arg(currentDate.getDay());
                iw->ui->textBrowser1->append(log);
            }

            // 更新财务摘要
            InsideWindow::updateFinancialSummary(); // 成功后更新
        }
        break;

    case 'n': // 进入下个月
        // 日期更新逻辑
        if (currentDate.getMonth() == 12)
            currentDate = Date(currentDate.getYear() + 1, 1, 1);
        else
            currentDate = Date(currentDate.getYear(), currentDate.getMonth() + 1, 1);

        // 账户结算
        for (auto & iter : accounts)
            iter->settle(currentDate);

        // 非文件操作时更新日志
        if(!is_now_reading_a_file)
        {
            commands << endl << cmd << " ";
            QString log = QString("更改成功，今天是%1年%2月%3日")
                              .arg(currentDate.getYear())
                              .arg(currentDate.getMonth())
                              .arg(currentDate.getDay());
            iw->ui->textBrowser1->append(log);
        }

        // 更新财务摘要
        InsideWindow::updateFinancialSummary();
        break;

    case 'q': // 交易记录查询
        try {
            // 获取查询日期范围
            date1 = queryStartDate;
            date2 = endDate;

            // 显示查询结果
            iw->ui->textBrowser1->append(QString::fromStdString(Account::query(date1, date2, MainWindow::Log_on_User)));
        }
        catch (DateException& err){
            cerr << err.what() << endl;
            if(cin.fail()) cin.clear();
        }
        break;
    default:
        cerr << "Not a right command." <<endl;
        break;
    }

    // 显示日期和总金额
    if(!is_now_reading_a_file){
        currentDate.show();
    }
}

// 创建账户按钮点击事件
void InsideWindow::on_PushButtonCreateAccount_clicked()
{
    accountParams[1] = ui->id->text();

    // 账号唯一性检查
    for(auto &iter : accounts){
        if(iter->getUser() == MainWindow::Log_on_User && iter->getId() == accountParams[1].toStdString()){
            QMessageBox::warning(this,tr("错误"),tr("该账号已被注册"));
            return;
        }
    }

    // 账户类型设置
    if (ui->checkBox->checkState() == Qt::Checked) {
        accountParams[0] = "c"; // 信用卡
        accountParams[2] = ui->credit->text(); // 信用额度
    }
    else accountParams[0] = "s";

    // 参数收集
    accountParams[2] = ui->credit->text();
    accountParams[3] = ui->rate->text();
    accountParams[4] = ui->fare->text();

    // 执行创建操作
    InsideWindow::backMain('a', false);
}

// 存款按钮点击事件
void InsideWindow::on_PushButtonDepositMoney_clicked()
{
    transactionData[0] = ui->id_cy->text();

    // 账户有效性检查
    for (unsigned int i = 0; i < accounts.size(); i++) {
        if (accounts[i]->getId() == transactionData[0].toStdString()) {
            if (accounts[i]->getUser() != MainWindow::Log_on_User) {
                QMessageBox::warning(this, tr("警告信息"), tr("这不是你的账号"));
                return;
            }
            // 索引转换
            transactionData[0] = QString::number(i);
            transactionData[1] = ui->amount_cy->text();
            transactionData[2] = ui->desc_cy->text();

            // 执行存款操作
            InsideWindow::backMain('d',false);
            return;
        }
    }
    QMessageBox::warning(this, tr("警告信息"), tr("该账号未被注册"));
}

// 取款按钮点击事件
void InsideWindow::on_PushButtonWithdrawMoney_clicked()
{
    transactionData[0] = ui->id_qu->text();

    // 账户有效性检查
    for (unsigned int i = 0; i < accounts.size(); i++) {
        if (accounts[i]->getId() == transactionData[0].toStdString()) {
            if (accounts[i]->getUser() != MainWindow::Log_on_User) {
                QMessageBox::warning(this, tr("警告信息"), tr("这不是你的账号"));
                return;
            }

            // 金额有效性检查
            double amount = ui->amount_qu->text().toDouble();
            if (amount <= 0) {
                QMessageBox::warning(this, tr("警告信息"), tr("请输入有效金额"));
                return;
            }

            // 参数设置
            transactionData[0] = QString::number(i);
            transactionData[1] = QString::number(amount);
            transactionData[2] = ui->desc_qu->text();

            // 执行取款操作
            InsideWindow::backMain('w', false);
            return;
        }
    }
    QMessageBox::warning(this, tr("警告信息"), tr("该账号未被注册"));
}

// 日期确认按钮点击事件
void InsideWindow::on_PushButtonConfirmDate_clicked()
{
    targetDay = ui->day_change->text().toInt();
    InsideWindow::backMain('c',false);
}

// 下个月按钮点击事件
void InsideWindow::on_PushButtonNextMonth_clicked()
{
    InsideWindow::backMain('n',false);
}

// 查询账户按钮点击事件
void InsideWindow::on_PushButtonQueryAccount_clicked()
{
    InsideWindow::backMain('s',false);
}

// 查询交易记录按钮点击事件
void InsideWindow::on_PushButtonQueryTransaction_clicked()
{
    // 设置查询日期范围
    queryStartDate = Date(ui->dateEdit1->date().year(), ui->dateEdit1->date().month(), ui->dateEdit1->date().day());
    endDate = Date(ui->dateEdit2->date().year(), ui->dateEdit2->date().month(), ui->dateEdit2->date().day());

    // 执行查询操作
    InsideWindow::backMain('q',false);
}

// 财务摘要更新函数
void InsideWindow::updateFinancialSummary() {
    double totalDebt = 0;
    double totalIncome = 0, totalExpense = 0;

    // 获取当前月份日期范围
    Date startDate(currentDate.getYear(), currentDate.getMonth(), 1);
    Date endDate(currentDate.getYear(), currentDate.getMonth(), currentDate.getMaxDay());

    // 统计信用卡债务
    for (Account* account : accounts) {
        if (account->getUser() == MainWindow::Log_on_User && account->isCreditAccount()) {
            totalDebt = account->getDebt();
        }
    }

    // 计算收支情况
    totalIncome = Account::calculateTotalIncomeBetweenDates(startDate, endDate, MainWindow::Log_on_User);
    totalExpense = Account::calculateTotalExpenseBetweenDates(startDate, endDate, MainWindow::Log_on_User);

    // UI更新
    iw->ui->textBrowser2->clear();
    iw->ui->textBrowser2->append(QString("负债：%1 收入：%2 支出：%3")
                                     .arg(QString::number(totalDebt, 'f', 2))
                                     .arg(QString::number(totalIncome, 'f', 2))
                                     .arg(QString::number(totalExpense, 'f', 2))
                                 );
}
