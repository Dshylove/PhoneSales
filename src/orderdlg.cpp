#include "orderdlg.h"
#include "ui_orderdlg.h"
#include <QMessageBox>

Orderdlg::Orderdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Orderdlg)
{
    ui->setupUi(this);
    setFixedSize(490,320);
    setAttribute(Qt::WA_DeleteOnClose,true);  //关闭后释放内存
    setModal(true);  //设置模态，让该窗口一直在最前端

    ui->pricelineEdit->setValidator(new QDoubleValidator());  //输入double类型
    ui->totalpricelineEdit->setReadOnly(true);
    ui->tellineEdit->setMaxLength(12);  //限制输入长度为12
}

Orderdlg::~Orderdlg()
{
    delete ui;
    qDebug()<<__FUNCTION__;
}

void Orderdlg::on_CancelBtn_clicked()
{
    this->close();
}

void Orderdlg::NewOrderdlg()
{
    this->setWindowTitle(tr("新建订单"));
    ui->label->hide();
    ui->orderIDlineEdit->hide();
    ui->pricelineEdit->setReadOnly(true);
    model = new QSqlQueryModel(this);  //加载商品
    model->setQuery("SELECT fullname FROM Phone");
    ui->fullnamecomboBox->setModel(model);
    dlg = 1;  //dlg为1表示加载新建订单窗口
}

void Orderdlg::ChangeOrderdlg()
{
    this->setWindowTitle(tr("修改订单"));
    ui->fullnamecomboBox->setDisabled(true);
    ui->numspinBox->setDisabled(true);
    dlg = 2;
}

void Orderdlg::on_fullnamecomboBox_currentIndexChanged(const QString &arg1)
{
    query.exec(QString("SELECT price,qty,tradecode FROM Phone WHERE fullname='%1'").arg(arg1));//ui->fullnamecomboBox->currentText()
    query.next();
    ui->pricelineEdit->setText(query.value(0).toString());  //加载价格
    stock = query.value(1).toInt();  //取库存数量
    tradecode = query.value(2).toInt();  //取商品编号
    ui->numspinBox->setMaximum(stock);  //数量最大值
    ui->stocklabel->setText(tr("库存：%1件").arg(stock));  //加载库存
    qreal sum = ui->numspinBox->value()*ui->pricelineEdit->text().toFloat();
    ui->totalpricelineEdit->setText(QString::number(sum));
}

void Orderdlg::on_numspinBox_valueChanged(int arg1)
{
    qreal sum = arg1*ui->pricelineEdit->text().toFloat();
    ui->totalpricelineEdit->setText(QString::number(sum));
}

void Orderdlg::on_pricelineEdit_textChanged(const QString &arg1)
{
    qreal sum = arg1.toFloat()*ui->numspinBox->value();
    ui->totalpricelineEdit->setText(QString::number(sum));
}

void Orderdlg::on_orderIDlineEdit_textChanged(const QString &arg1)
{
    query.exec("SELECT tradecode,amount,name,Tel,address FROM Orders WHERE orderID='"+arg1+"'");
    if(query.next())
    {
        tradecode = query.value(0).toInt();
        ui->numspinBox->setValue(query.value(1).toInt());
        ui->namelineEdit->setText(query.value(2).toString());
        ui->tellineEdit->setText(query.value(3).toString());
        ui->addresslineEdit->setText(query.value(4).toString());
        model = new QSqlQueryModel(this);  //加载商品
        model->setQuery(QString("SELECT fullname FROM Phone WHERE tradecode='%1'").arg(tradecode));
        ui->fullnamecomboBox->setModel(model);
        orderIDCnt = true;
    }
    else
    {
        ui->pricelineEdit->setText("");
        ui->numspinBox->setValue(0);
        ui->namelineEdit->setText("");
        ui->tellineEdit->setText("");
        ui->addresslineEdit->setText("");
        orderIDCnt = false;
    }
}

void Orderdlg::on_OkBtn_clicked()
{   qDebug()<<dlg;
    if(dlg == 1)
    {
        if(ui->numspinBox->value() == 0 || ui->namelineEdit->text() == "" || ui->tellineEdit->text() == "" || ui->addresslineEdit->text() == "")
        {
            QMessageBox::information(this,tr("提示"),tr("请输入完整！"));
            return;
        }
        QString datetime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss"); //获取当前时间
        QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        //bool rtn = query.prepare("INSERT INTO Orders(orderID,tradecode,price,amount,totalprice,status,name,Tel,address,date) VALUES(:orderID,:tradecode,:price,:amount,:totalprice,:status,:name,:Tel,:address,:date)");
        bool rtn = query.prepare("exec sp_InsertOrders :orderID,:tradecode,:price,:amount,:totalprice,:name,:Tel,:address,:date");
        query.bindValue(":orderID",datetime);        
        query.bindValue(":tradecode",tradecode);
        query.bindValue(":price",ui->pricelineEdit->text());
        query.bindValue(":amount",ui->numspinBox->value());//??
        query.bindValue(":totalprice",ui->totalpricelineEdit->text());
        //query.bindValue(":status","未发货");
        query.bindValue(":name",ui->namelineEdit->text());
        query.bindValue(":Tel",ui->tellineEdit->text());
        query.bindValue(":address",ui->addresslineEdit->text());
        query.bindValue(":date",date);
        //query.bindValue(":deliverytime",QVariant(QVariant::String));  //输入空值NULL，对应string类型
        query.exec();
        if(rtn)
        {
            int amount = ui->numspinBox->value();  //更新库存
            stock = stock-amount;
            query.exec(QString("UPDATE Phone SET qty=%1 WHERE tradecode='%2'").arg(stock).arg(tradecode));
//SQL触发器insert新ID值更新到Orders表中
            query.exec("SELECT ID FROM Customer");
            query.last();
            int ID = query.value(0).toInt();
            query.prepare("UPDATE Orders SET ID=:ID WHERE orderID=:orderID");
            query.bindValue(":ID",ID);
            query.bindValue(":orderID",datetime);
            query.exec();

            QMessageBox::about(this,tr("提示"),tr("下单成功！"));
            emit Refresh();  //发信号
            this->close();
        }
        else
            QMessageBox::warning(this, tr("警告"),tr("下单失败！"),QMessageBox::Ok);
    }

    if(dlg == 2)
    {
        if(orderIDCnt == false)
        {
            QMessageBox::information(this,tr("提示"),tr("订单号不正确！"));
            ui->orderIDlineEdit->setFocus();
            return;
        }
        if(ui->pricelineEdit->text() == "" || ui->namelineEdit->text() == "" || ui->tellineEdit->text() == "" || ui->addresslineEdit->text() == "")
        {
            QMessageBox::information(this,tr("提示"),tr("请输入完整！"));
            return;
        }
        bool rtn = query.prepare("UPDATE Orders SET price=:price,totalprice=:totalprice,name=:name,Tel=:Tel,address=:address WHERE orderID=:orderID");
        query.bindValue(":price",ui->pricelineEdit->text());
        query.bindValue(":totalprice",ui->totalpricelineEdit->text());
        query.bindValue(":name",ui->namelineEdit->text());
        query.bindValue(":Tel",ui->tellineEdit->text());
        query.bindValue(":address",ui->addresslineEdit->text());
        query.bindValue(":orderID",ui->orderIDlineEdit->text());
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
