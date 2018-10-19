#include "phonedlg.h"
#include "ui_phonedlg.h"
#include <QMessageBox>

Phonedlg::Phonedlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Phonedlg)
{
    ui->setupUi(this);
    setFixedSize(480,290);
    setAttribute(Qt::WA_DeleteOnClose,true);  //关闭后释放内存
    setModal(true);  //设置模态，让该窗口一直在最前端
    ui->tradecodelineEdit->setValidator(new QIntValidator());
    ui->pricelineEdit->setValidator(new QDoubleValidator());  //输入double类型
}

Phonedlg::~Phonedlg()
{
    delete ui;
    qDebug()<<__FUNCTION__;
}

void Phonedlg::on_CancelBtn_clicked()
{
    this->close();
}

void Phonedlg::NewPhonedlg()
{
    this->setWindowTitle(tr("添加商品"));
    ui->label->hide();
    ui->tradecodelineEdit->hide();
    dlg = 1;
}

void Phonedlg::ChangePhonedlg()
{
    this->setWindowTitle(tr("修改商品"));
    dlg = 2;
}

void Phonedlg::on_tradecodelineEdit_textChanged(const QString &arg1)
{
    query.exec("SELECT fullname,color,price FROM Phone WHERE tradecode='"+arg1+"'");
    if(query.next())
    {
        ui->fullnamelineEdit->setText(query.value(0).toString());
        ui->colorlineEdit->setText(query.value(1).toString());
        ui->pricelineEdit->setText(query.value(2).toString());
        tradecodeCnt = true;
    }
    else
    {
        ui->fullnamelineEdit->setText("");
        ui->colorlineEdit->setText("");
        ui->pricelineEdit->setText("");
        tradecodeCnt = false;
    }
}

void Phonedlg::on_OkBtn_clicked()
{   qDebug()<<dlg;
    if(dlg == 1)
    {
        if(ui->fullnamelineEdit->text() == "" || ui->colorlineEdit->text() == "" || ui->pricelineEdit->text() == "")
        {
            QMessageBox::information(this,tr("提示"),tr("请输入完整！"));
            return;
        }
        bool rtn = query.prepare("INSERT INTO Phone (fullname,color,price) VALUES (:fullname,:color,:price)");
        query.bindValue(":fullname",ui->fullnamelineEdit->text());
        query.bindValue(":color",ui->colorlineEdit->text());
        query.bindValue(":price",ui->pricelineEdit->text());
        query.exec();
        if(rtn)
        {
            QMessageBox::about(this,tr("提示"),tr("添加成功！"));
            emit Refresh();  //发信号
        }
        else
            QMessageBox::warning(this, tr("警告"),tr("添加失败！"),QMessageBox::Ok);
    }

    if(dlg == 2)
    {
        if(tradecodeCnt == false)
        {
            QMessageBox::information(this,tr("提示"),tr("编号不正确！"));
            ui->tradecodelineEdit->setFocus();
            return;
        }
        if(ui->fullnamelineEdit->text() == "" || ui->colorlineEdit->text() == "" || ui->pricelineEdit->text() == "")
        {
            QMessageBox::information(this,tr("提示"),tr("请输入完整！"));
            return;
        }
        bool rtn = query.prepare("UPDATE Phone SET fullname=:fullname,color=:color,price=:price WHERE tradecode=:tradecode");
        query.bindValue(":fullname",ui->fullnamelineEdit->text());
        query.bindValue(":color",ui->colorlineEdit->text());
        query.bindValue(":price",ui->pricelineEdit->text());
        query.bindValue(":tradecode",ui->tradecodelineEdit->text());
        query.exec();
        if(rtn)
        {
            QMessageBox::about(this,tr("提示"),tr("修改成功！"));
            emit Refresh();  //发信号
        }
        else
            QMessageBox::warning(this, tr("警告"),tr("修改失败！"),QMessageBox::Ok);
    }
}
