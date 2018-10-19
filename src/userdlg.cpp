#include "userdlg.h"
#include "ui_userdlg.h"
#include <QMessageBox>

Userdlg::Userdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Userdlg)
{
    ui->setupUi(this);    
    setFixedSize(400,300);
    setAttribute(Qt::WA_DeleteOnClose,true);  //关闭后释放内存
    setModal(true);  //设置模态，让该窗口一直在最前端
}

Userdlg::~Userdlg()
{
    delete ui;
    qDebug()<<__FUNCTION__;
}

void Userdlg::on_CancelBtn_clicked()
{
    this->close();
}

void Userdlg::AddUserdlg()
{
    this->setWindowTitle(tr("添加管理员"));
    ui->label_4->hide();
    ui->lineEdit_4->hide();
    dlg = 1;
}

void Userdlg::ChangeUserdlg()
{
    this->setWindowTitle(tr("密码修改"));
    ui->label_2->setText(tr("原密码："));
    ui->label_3->setText(tr("新密码："));
    dlg = 2;
}

void Userdlg::on_lineEdit_textChanged(const QString &arg1)
{
    userCnt = false;
    if(arg1.trimmed() == "")
    {
        ui->Userrightlabel->setText("");
        return;
    }

    //query.exec("SELECT username FROM Userlogin");
    query.exec("exec sp_SelectUser");
    while(query.next())
    {
        if(arg1.trimmed() == query.value(0).toString())
            userCnt = true;
    }
    if(dlg == 1)
    {        
        if(userCnt == true)
        {
            ui->Userrightlabel->setStyleSheet("color:red");
            ui->Userrightlabel->setText(tr("用户名已存在！"));
        }
        else
        {
            ui->Userrightlabel->setStyleSheet("color:black");
            ui->Userrightlabel->setText(tr("此用户名可用！"));
        }
    }
    if(dlg == 2)
    {        
        if(userCnt == true)
        {
            ui->Userrightlabel->setStyleSheet("color:black");
            ui->Userrightlabel->setText(tr("用户名正确！"));
        }
        else
        {
            ui->Userrightlabel->setStyleSheet("color:red");
            ui->Userrightlabel->setText(tr("用户名不存在！"));
        }
    }
}

void Userdlg::on_OkBtn_clicked()
{
    qDebug()<<dlg;
    if(dlg == 1)
    {
        if(userCnt == true)
        {
            QMessageBox::warning(this, tr("警告"),tr("该用户名已存在！"),QMessageBox::Ok);
            return;
        }
        if(ui->lineEdit->text().trimmed() == "" || ui->lineEdit_2->text() == "" || ui->lineEdit_3->text() == "")
        {
            QMessageBox::warning(this, tr("警告"),tr("请输入完整！"),QMessageBox::Ok);
            return;
        }
        if(ui->lineEdit_2->text() != ui->lineEdit_3->text())
        {
            QMessageBox::warning(this, tr("警告"),tr("密码不一致！"),QMessageBox::Ok);
            return;
        }
        bool rtn = query.prepare("INSERT INTO Userlogin(username,password) VALUES(:username,:password)");
        query.bindValue(":username",ui->lineEdit->text().trimmed());
        query.bindValue(":password",ui->lineEdit_2->text());
        query.exec();
        if(rtn)
        {
            QMessageBox::about(this,tr("提示"),tr("添加成功！"));
            this->close();
        }
        else        
            QMessageBox::warning(this, tr("警告"),tr("添加失败！"),QMessageBox::Ok);        
    }

    if(dlg == 2)
    {
        if(ui->lineEdit->text().trimmed() == "" || ui->lineEdit_2->text() == "" || ui->lineEdit_3->text() == "" || ui->lineEdit_4->text() == "")
        {
            QMessageBox::warning(this, tr("警告"),tr("请输入完整！"),QMessageBox::Ok);
            return;
        }
        if(userCnt == false)
        {
            QMessageBox::warning(this, tr("警告"),tr("用户名不存在！"),QMessageBox::Ok);
            return;
        }
        if(ui->lineEdit_3->text() != ui->lineEdit_4->text())
        {
            QMessageBox::warning(this, tr("警告"),tr("新密码不一致！"),QMessageBox::Ok);
            return;
        }
        query.prepare("SELECT password FROM Userlogin WHERE username=:username");
        query.bindValue(":username",ui->lineEdit->text().trimmed());
        query.exec();
        query.next();
        if(ui->lineEdit_2->text() != query.value(0).toString())
        {
            QMessageBox::warning(this, tr("警告"),tr("原密码不正确！"),QMessageBox::Ok);
            return;
        }

        bool rtn = query.prepare("UPDATE Userlogin SET password=:password WHERE username=:username");
        query.bindValue(":password",ui->lineEdit_3->text());
        query.bindValue(":username",ui->lineEdit->text().trimmed());
        query.exec();
        if(rtn)
        {
            QMessageBox::about(this,tr("提示"),tr("修改成功！"));
            this->close();
        }
        else        
            QMessageBox::warning(this, tr("警告"),tr("修改失败！"),QMessageBox::Ok);        
    }
}
