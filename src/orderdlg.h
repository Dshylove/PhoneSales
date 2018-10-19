#ifndef ORDERDLG_H
#define ORDERDLG_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class Orderdlg;
}

class Orderdlg : public QDialog
{
    Q_OBJECT

public:
    explicit Orderdlg(QWidget *parent = 0);
    ~Orderdlg();

signals:
    void Refresh();

private slots:
    void NewOrderdlg();

    void ChangeOrderdlg();

    void on_CancelBtn_clicked();

    void on_OkBtn_clicked();

    void on_fullnamecomboBox_currentIndexChanged(const QString &arg1);

    void on_numspinBox_valueChanged(int arg1);

    void on_pricelineEdit_textChanged(const QString &arg1);

    void on_orderIDlineEdit_textChanged(const QString &arg1);

private:
    Ui::Orderdlg *ui;
    QSqlQueryModel *model;
    QSqlQuery query;
    int dlg,stock,tradecode;
    bool orderIDCnt = false;
};

#endif // ORDERDLG_H
