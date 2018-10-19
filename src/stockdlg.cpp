#include "stockdlg.h"
#include "ui_stockdlg.h"
#include <QMessageBox>

Stockdlg::Stockdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Stockdlg)
{
    ui->setupUi(this);
    setFixedSize(400,260);
    setAttribute(Qt::WA_DeleteOnClose,true);  //关闭后释放内存
    setModal(true);  //设置模态，让该窗口一直在最前端
    ui->tradecodelineEdit->setValidator(new QIntValidator());
    ui->fullnamelineEdit->setReadOnly(true);
}

Stockdlg::~Stockdlg()
{
    delete ui;
    qDebug()<<__FUNCTION__;
}

void Stockdlg::on_CancelBtn_clicked()
{
    this->close();
}

void Stockdlg::AddStockdlg()
{
    this->setWindowTitle(tr("添加库存"));
    dlg = 1;
}

void Stockdlg::ChangeStockdlg()
{
    this->setWindowTitle(tr("修改库存"));
    ui->qtylabel->setText("库存数量：");
    dlg = 2;
}

void Stockdlg::on_tradecodelineEdit_textChanged(const QString &arg1)
{
    query.exec("SELECT fullname,qty FROM Phone WHERE tradecode='"+arg1+"'");
    if(query.next())
    {
        ui->fullnamelineEdit->setText(query.value(0).toString());
        qty = query.value(1).toInt();
        if(dlg == 2)
            ui->qtyspinBox->setValue(qty);
        tradecodeCnt = true;
    }
    else
    {
        ui->fullnamelineEdit->setText("");
        if(dlg == 2)
            ui->qtyspinBox->setValue(0);
        tradecodeCnt = false;
    }
}

void Stockdlg::on_OkBtn_clicked()
{   qDebug()<<dlg;
    if(tradecodeCnt == false)
    {
        QMessageBox::information(this,tr("提示"),tr("编号不正确！"));
        ui->tradecodelineEdit->setFocus();
        return;
    }
    if(dlg == 1)
    {   qDebug()<<qty+ui->qtyspinBox->value();//
        bool rtn = query.prepare("UPDATE Phone SET qty=:qty WHERE tradecode=:tradecode");
        query.bindValue(":qty",qty+ui->qtyspinBox->value());
        query.bindValue(":tradecode",ui->tradecodelineEdit->text());
        query.exec();
        if(rtn)
        {
            QMessageBox::about(this,tr("提示"),tr("添加成功！"));
            emit Refresh();  //发信号
            this->close();
        }
        else
            QMessageBox::warning(this, tr("警告"),tr("添加失败！"),QMessageBox::Ok);
    }

    if(dlg == 2)
    {
        bool rtn = query.prepare("UPDATE Phone SET qty=:qty WHERE tradecode=:tradecode");
        query.bindValue(":qty",ui->qtyspinBox->value());
        query.bindValue(":tradecode",ui->tradecodelineEdit->text());
        query.exec();
        if(rtn)
        {
            QMessageBox::about(this,tr("提示"),tr("修改成功！"));
            emit Refresh();  //发信号
            this->close();
        }
        else
            QMessageBox::warning(this, tr("警告"),tr("修改失败！"),QMessageBox::Ok);
    }
}
