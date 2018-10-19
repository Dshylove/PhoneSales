#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "orderdlg.h"
#include "phonedlg.h"
#include "stockdlg.h"
#include "userdlg.h"
#include <QMessageBox>
#include <QInputDialog>

extern QString usr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(920,685);
    //statusBar()->showMessage(tr("登录用户："),0);
    label = new QLabel(this);
    label->setText(QString(tr(" 登录用户：%1")).arg(usr));
    label->setMinimumSize(label->sizeHint());
    statusBar()->addWidget(label);
    statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));

    model = new QSqlRelationalTableModel(this);
    model->setTable("Orders");
    model->setSort(0, Qt::DescendingOrder); //按降序排列，AscendingOrder为升序
    model->setRelation(1,QSqlRelation("Phone","tradecode","fullname"));
    model->setHeaderData(0,Qt::Horizontal,tr("订单号"));  //设置表头显示内容
    model->setHeaderData(1,Qt::Horizontal,tr("商品名称"));
    model->setHeaderData(2,Qt::Horizontal,tr("单价"));
    model->setHeaderData(3,Qt::Horizontal,tr("数量"));
    model->setHeaderData(4,Qt::Horizontal,tr("总额"));
    model->setHeaderData(5,Qt::Horizontal,tr("状态"));
    model->setHeaderData(6,Qt::Horizontal,tr("姓名"));
    model->setHeaderData(7,Qt::Horizontal,tr("客户编号"));
    model->setHeaderData(8,Qt::Horizontal,tr("手机号"));
    model->setHeaderData(9,Qt::Horizontal,tr("收货地址"));
    model->setHeaderData(10,Qt::Horizontal,tr("下单时间"));
    model->setHeaderData(11,Qt::Horizontal,tr("发货时间"));
    //model->removeColumn(2);   //不显示第2属性列,如果这时添加记录，则该属性的值添加不上
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);  //编辑策略，OnManualSubmit指需调用函数才改变
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();  //每列宽度适配其内容
    //ui->tableView->setColumnHidden(7,true);  //隐藏列
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);  //不允许多选
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  //选中时为整行选中
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  //使其不可编辑
    //ui->tableView->verticalHeader()->setVisible(false);  //禁止显示垂直方向表头
    //ui->tableView->setColumnWidth(0,145);  //设置列宽度，1个字符宽度为10

    model2 = new QSqlRelationalTableModel(this);
    model2->setTable("Phone");
    model2->setHeaderData(0,Qt::Horizontal,tr("商品编号"));
    model2->setHeaderData(1,Qt::Horizontal,tr("商品名称"));
    model2->setHeaderData(2,Qt::Horizontal,tr("颜色"));
    model2->setHeaderData(3,Qt::Horizontal,tr("价格"));
    model2->setHeaderData(4,Qt::Horizontal,tr("库存"));
    model2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model2->select();
    ui->tableView_2->setModel(model2);
    ui->tableView_2->setColumnWidth(1,420);
    ui->tableView_2->setColumnWidth(4,100);
    ui->tableView_2->setSelectionMode(QAbstractItemView::SingleSelection);  //不允许多选
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);  //选中时为整行选中
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);  //使其不可编辑
    ui->tableView_2->horizontalHeader()->setStretchLastSection(true); //最后一列充满
    ui->tableView_2->verticalHeader()->setVisible(false);  //禁止显示垂直方向表头

    model3 = new QSqlRelationalTableModel(this);
    model3->setTable("Customer");
    model3->setHeaderData(0,Qt::Horizontal,tr("客户编号"));
    model3->setHeaderData(1,Qt::Horizontal,tr("姓名"));
    model3->setHeaderData(2,Qt::Horizontal,tr("手机号"));
    model3->setHeaderData(3,Qt::Horizontal,tr("地址"));
    model3->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model3->select();
    ui->tableView_3->setModel(model3);
    ui->tableView_3->setSelectionMode(QAbstractItemView::SingleSelection);  //不允许多选
    ui->tableView_3->setSelectionBehavior(QAbstractItemView::SelectRows);  //选中时为整行选中
    ui->tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);  //使其不可编辑
    ui->tableView_3->horizontalHeader()->setStretchLastSection(true); //最后一列充满
    ui->tableView_3->verticalHeader()->setVisible(false);  //禁止显示垂直方向表头


//建立菜单以及信号与槽
    QAction *exit=new QAction(tr("退出"),this);
    menuBar()->addAction(exit);
    connect(exit,SIGNAL(triggered()),this,SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
//根据姓名进行筛选查询
    QString name = ui->lineEdit->text();
    if(name == "")
        QMessageBox::information(this,tr("提示"),tr("请输入姓名！"));
    else
    {
        model->setFilter(QString("name='%1'").arg(name));
        model->select();
    }
}

void MainWindow::on_selectorder_action_triggered()
{
    bool ok;
    QString orderID = QInputDialog::getText(this,tr("订单查询"),tr("请输入订单号："),QLineEdit::Normal,"",&ok);
    if(ok && orderID.trimmed().isEmpty())
        QMessageBox::warning(this, tr("警告"),tr("订单号不能为空！"));
    if(ok && !orderID.trimmed().isEmpty())
    {
        model->setFilter(QString("orderID='%1'").arg(orderID));
        model->select();
    }
}

void MainWindow::on_pushButton_3_clicked()  //显示所有订单
{
    model->setTable("Orders");
    model->setSort(0, Qt::DescendingOrder);
    model->setRelation(1,QSqlRelation("Phone","tradecode","fullname"));
    model->setHeaderData(0,Qt::Horizontal,tr("订单号"));
    model->setHeaderData(1,Qt::Horizontal,tr("商品名称"));
    model->setHeaderData(2,Qt::Horizontal,tr("单价"));
    model->setHeaderData(3,Qt::Horizontal,tr("数量"));
    model->setHeaderData(4,Qt::Horizontal,tr("总额"));
    model->setHeaderData(5,Qt::Horizontal,tr("状态"));
    model->setHeaderData(6,Qt::Horizontal,tr("姓名"));
    model->setHeaderData(7,Qt::Horizontal,tr("客户编号"));
    model->setHeaderData(8,Qt::Horizontal,tr("手机号"));
    model->setHeaderData(9,Qt::Horizontal,tr("收货地址"));
    model->setHeaderData(10,Qt::Horizontal,tr("下单时间"));
    model->setHeaderData(11,Qt::Horizontal,tr("发货时间"));
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_pushButton_2_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();  //判断是否选择了待删除订单
    if(!index.isValid())
    {
        QMessageBox::information(this,tr("提示"),tr("请选择要删除的订单！"));
        return;  //结束当前函数
    }
    int curRow = ui->tableView->currentIndex().row();  //获取选中行
//    model->removeRow(curRow);
//    int ok = QMessageBox::warning(this,tr("删除当前订单！"),tr("确定要删除当前订单吗？"),QMessageBox::Yes,QMessageBox::No);
//    if(ok == QMessageBox::No)
//        model->revertAll();
//    else
//        model->submitAll();

    QSqlRecord record = model->record(curRow);
    QString orderID = record.value("orderID").toString();
    qDebug()<<orderID;
    int ok = QMessageBox::warning(this,tr("删除当前订单！"),tr("确定要删除当前订单吗？"),QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::Yes)
    {
        query.exec("DELETE FROM Orders WHERE orderID='"+orderID+"'");
        on_pushButton_3_clicked();
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();  //判断是否选择了订单
    if(!index.isValid())
    {
        QMessageBox::information(this,tr("提示"),tr("请选择要发货的订单！"));
        return;  //结束当前函数
    }
    int ok = QMessageBox::warning(this,tr("发货"),tr("确定要发货吗？"),QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::Yes)
    {
        int curRow = ui->tableView->currentIndex().row();
        QSqlRecord record = model->record(curRow);
        QString orderID = record.value(0).toString();
        qDebug()<<orderID;//
        QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        //query.exec("UPDATE Orders SET status='已发货',deliverytime='"+date+"' WHERE orderID='"+orderID+"'");
        query.exec("exec sp_UpdateStatus '"+date+"','"+orderID+"'");
        QMessageBox::about(this,tr("提示"),tr("已发货！"));
        MainWindow::on_pushButton_3_clicked();
    }
}

void MainWindow::on_RefreshBtn_clicked()
{
    model2->setTable("Phone");
    model2->setHeaderData(0,Qt::Horizontal,tr("商品编号"));
    model2->setHeaderData(1,Qt::Horizontal,tr("商品名称"));
    model2->setHeaderData(2,Qt::Horizontal,tr("颜色"));
    model2->setHeaderData(3,Qt::Horizontal,tr("价格"));
    model2->setHeaderData(4,Qt::Horizontal,tr("库存"));
    model2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model2->select();    
    ui->tableView_2->setColumnWidth(1,420);
    ui->tableView_2->setColumnWidth(4,100);
}

void MainWindow::on_pushButton_5_clicked()
{
    QString name = ui->lineEdit_2->text();
    if(name == "")
        QMessageBox::information(this,tr("提示"),tr("请输入姓名！"));
    else
    {
        model3->setFilter(QString("name='%1'").arg(name));
        model3->select();
    }
}

void MainWindow::on_ShowCustomerBtn_clicked()
{
    model3->setTable("Customer");
    model3->setHeaderData(0,Qt::Horizontal,tr("客户编号"));
    model3->setHeaderData(1,Qt::Horizontal,tr("姓名"));
    model3->setHeaderData(2,Qt::Horizontal,tr("手机号"));
    model3->setHeaderData(3,Qt::Horizontal,tr("地址"));
    model3->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model3->select();
}

void MainWindow::on_neworder_action_triggered()
{
    Orderdlg *orderdlg = new Orderdlg(this);
    QObject::connect(this, SIGNAL(NewOrder()), orderdlg, SLOT(NewOrderdlg()));
    emit NewOrder();  //发射一个信号
    orderdlg->show();
    QObject::connect(orderdlg, SIGNAL(Refresh()), this, SLOT(on_pushButton_3_clicked()));
    QObject::connect(orderdlg, SIGNAL(Refresh()), this, SLOT(on_RefreshBtn_clicked()));
    QObject::connect(orderdlg, SIGNAL(Refresh()), this, SLOT(on_ShowCustomerBtn_clicked()));
}

void MainWindow::on_changeorder_action_triggered()
{
    Orderdlg *orderdlg2 = new Orderdlg(this);
    QObject::connect(this, SIGNAL(ChangeOrder()), orderdlg2, SLOT(ChangeOrderdlg()));
    emit ChangeOrder();
    orderdlg2->show();
    QObject::connect(orderdlg2, SIGNAL(Refresh()), this, SLOT(on_pushButton_3_clicked()));
    QObject::connect(orderdlg2, SIGNAL(Refresh()), this, SLOT(on_ShowCustomerBtn_clicked()));
}

void MainWindow::on_newphone_action_triggered()
{
    Phonedlg *phonedlg = new Phonedlg(this);
    QObject::connect(this, SIGNAL(NewPhone()), phonedlg, SLOT(NewPhonedlg()));
    emit NewPhone();
    phonedlg->show();
    QObject::connect(phonedlg, SIGNAL(Refresh()), this, SLOT(on_RefreshBtn_clicked()));
}

void MainWindow::on_changephone_action_triggered()
{
    Phonedlg *phonedlg2 = new Phonedlg(this);
    QObject::connect(this, SIGNAL(ChangePhone()), phonedlg2, SLOT(ChangePhonedlg()));
    emit ChangePhone();
    phonedlg2->show();
    QObject::connect(phonedlg2, SIGNAL(Refresh()), this, SLOT(on_RefreshBtn_clicked()));
    QObject::connect(phonedlg2, SIGNAL(Refresh()), this, SLOT(on_pushButton_3_clicked()));
}

void MainWindow::on_addstock_action_triggered()
{
    Stockdlg *stockdlg = new Stockdlg(this);
    QObject::connect(this, SIGNAL(AddStock()), stockdlg, SLOT(AddStockdlg()));
    emit AddStock();
    stockdlg->show();
    QObject::connect(stockdlg, SIGNAL(Refresh()), this, SLOT(on_RefreshBtn_clicked()));
}

void MainWindow::on_changestock_action_triggered()
{
    Stockdlg *stockdlg2 = new Stockdlg(this);
    QObject::connect(this, SIGNAL(ChangeStock()), stockdlg2, SLOT(ChangeStockdlg()));
    emit ChangeStock();
    stockdlg2->show();
    QObject::connect(stockdlg2, SIGNAL(Refresh()), this, SLOT(on_RefreshBtn_clicked()));
}

void MainWindow::on_adduser_action_triggered()
{
    Userdlg *userdlg = new Userdlg(this);
    QObject::connect(this, SIGNAL(AddUser()), userdlg, SLOT(AddUserdlg()));
    emit AddUser();
    userdlg->show();
}

void MainWindow::on_changeuser_action_triggered()
{
    Userdlg *userdlg2 = new Userdlg(this);
    QObject::connect(this, SIGNAL(ChangeUser()), userdlg2, SLOT(ChangeUserdlg()));
    emit ChangeUser();
    userdlg2->show();
}
