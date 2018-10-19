#include "logindlg.h"
#include "ui_logindlg.h"
#include <QMessageBox>
#include <QtSql>

QString usr;

LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDlg)
{
    ui->setupUi(this);
    ui->usrLineEdit->setFocus();  //定位光标到用户名编辑框
    setFixedSize(400,280);  //设置窗口大小
}

LoginDlg::~LoginDlg()
{
    delete ui;
}

void LoginDlg::on_loginBtn_clicked()
{
    usr = ui->usrLineEdit->text().trimmed();  //QString类的trimmed()函数实现去除字符串前后的空白字符
    QString pwd = ui->pwdLineEdit->text();
    QSqlQuery query;
    //调用存储过程sp_Userlogin_IsRight
    query.prepare("exec sp_Userlogin_IsRight '"+usr+"','"+pwd+"',? output"); //"+usr+"为在SQL语句中引用变量usr
    query.bindValue(0,0,QSql::Out);
    query.exec();
    query.next();
    qDebug()<<query.value(0).toString();
    int isright = query.boundValue(0).toInt();
    qDebug()<<isright;//
    if(isright == 0)
    {
        QMessageBox::warning(this, tr("警告"),tr("该用户名不存在！"),QMessageBox::Ok);
        // 清空内容并定位光标
        ui->usrLineEdit->clear();
        ui->pwdLineEdit->clear();
        ui->usrLineEdit->setFocus();
    }
    else
    {
        if(isright == 2)
        {
            QMessageBox::about(this,tr("提示"),tr("登录成功！"));
            accept();
        }
        else
        {
            QMessageBox::warning(this, tr("警告"),tr("密码错误！"),QMessageBox::Ok);
            ui->pwdLineEdit->clear();
            ui->pwdLineEdit->setFocus();
        }
    }
}
