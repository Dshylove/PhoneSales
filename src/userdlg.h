#ifndef USERDLG_H
#define USERDLG_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class Userdlg;
}

class Userdlg : public QDialog
{
    Q_OBJECT

public:
    explicit Userdlg(QWidget *parent = 0);
    ~Userdlg();

private slots:
    void AddUserdlg();

    void ChangeUserdlg();

    void on_CancelBtn_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_OkBtn_clicked();

private:
    Ui::Userdlg *ui;
    QSqlQuery query;
    int dlg;
    bool userCnt = false;
};

#endif // USERDLG_H
