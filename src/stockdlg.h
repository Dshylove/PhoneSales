#ifndef STOCKDLG_H
#define STOCKDLG_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class Stockdlg;
}

class Stockdlg : public QDialog
{
    Q_OBJECT

public:
    explicit Stockdlg(QWidget *parent = 0);
    ~Stockdlg();

signals:
    void Refresh();

private slots:
    void AddStockdlg();

    void ChangeStockdlg();

    void on_CancelBtn_clicked();

    void on_tradecodelineEdit_textChanged(const QString &arg1);

    void on_OkBtn_clicked();

private:
    Ui::Stockdlg *ui;
    QSqlQuery query;
    int dlg,qty;
    bool tradecodeCnt = false;
};

#endif // STOCKDLG_H
