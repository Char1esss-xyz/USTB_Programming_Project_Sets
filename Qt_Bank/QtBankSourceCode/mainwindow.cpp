#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vector"
#include "bankusers.h"
#include "QLineEdit"
#include "fstream"
#include "string"
#include "iostream"
#include "QMessageBox"
#include "QDir"
#include "insidewindow.h"

string MainWindow::Log_on_User;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fstream users;
    users.open("users.txt", ios_base::in);
    while(!users.eof() && users.is_open()){
        string id,password;
        users >> id >> password;
        bankUsers.push_back(new BankUsers(id, password));
    }
    users.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_registerID_clicked()
{
    fstream users;
    users.open("users.txt", ios_base::out | ios_base::app);

    QString id = ui->inputID->text();
    QString password = ui->inputPassword->text();

    for(auto &iter : bankUsers){
        if(iter->getID() == id.toStdString()){
            QMessageBox::warning(this,tr("警告信息"),tr("该用户名已被注册"));
            return;
        }
    }

    users << id.toStdString() << " " << password.toStdString() << endl;
    bankUsers.push_back(new BankUsers(id.toStdString(), password.toStdString()));
    users.close();
}

void MainWindow::on_log_onID_clicked()
{
    QString id = ui->inputID->text();
    QString password = ui->inputPassword->text();

    for(auto &iter : bankUsers){
        if(iter->getID() == id.toStdString()){
            if(iter->getPassword() == password.toStdString()){
                Log_on_User = iter->getID();
                static InsideWindow w1;
                QObject::connect(this,SIGNAL(showInsideWindow()),&w1,SLOT(switchInsideWindow()));
                this->hide();
                emit showInsideWindow();
            }
            else {
                QMessageBox::warning(this,tr("警告信息"),tr("密码错误"));
            }
            return;
        }
    }
    QMessageBox::warning(this,tr("警告信息"),tr("该用户名尚未注册"));
}
