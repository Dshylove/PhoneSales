#include "mainwindow.h"
#include "logindlg.h"
#include <QApplication>
#include <QtSql>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//SQL数据库连接//
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    qDebug()<<"ODBC driver?"<<db.isValid();
    //db.setHostName("127.0.0.1");
    db.setDatabaseName("QTDSN");
    db.setUserName("sa");
    db.setPassword("112233");
    if(!db.open())
    {
        qDebug()<<db.lastError().text();
        QMessageBox::critical(0, QObject::tr("Database error!"), db.lastError().text());
        return false;
    }
    else
        qDebug()<<"database open success!";

    LoginDlg dlg;                       // 建立自己新建的LoginDlg类的实例dlg

    if(dlg.exec() == QDialog::Accepted)// 利用Accepted返回值判断按钮是否被按下
    {
        MainWindow w;//
        w.show();                      // 如果被按下，显示主窗口
        return a.exec();              // 程序一直执行，直到主窗口关闭
    }
    else
        return 0;                   //如果没有被按下，则不会进入主窗口，整个程序结束运行
}
