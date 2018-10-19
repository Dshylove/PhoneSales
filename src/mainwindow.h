#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);  //explicit构造函数只能被显式调用
    ~MainWindow();

signals:
    void NewOrder();

    void ChangeOrder();

    void NewPhone();

    void ChangePhone();

    void AddStock();

    void ChangeStock();

    void AddUser();

    void ChangeUser();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_neworder_action_triggered();

    void on_changeorder_action_triggered();

    void on_newphone_action_triggered();

    void on_changephone_action_triggered();

    void on_RefreshBtn_clicked();

    void on_addstock_action_triggered();

    void on_changestock_action_triggered();

    void on_pushButton_5_clicked();

    void on_ShowCustomerBtn_clicked();

    void on_adduser_action_triggered();

    void on_changeuser_action_triggered();

    void on_selectorder_action_triggered();

private:
    Ui::MainWindow *ui;
    QSqlRelationalTableModel *model,*model2,*model3;
    QSqlQuery query;
    QLabel *label;
};

#endif // MAINWINDOW_H
