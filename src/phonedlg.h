#ifndef PHONEDLG_H
#define PHONEDLG_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class Phonedlg;
}

class Phonedlg : public QDialog
{
    Q_OBJECT

public:
    explicit Phonedlg(QWidget *parent = 0);
    ~Phonedlg();

signals:
    void Refresh();

private slots:
    void NewPhonedlg();

    void ChangePhonedlg();

    void on_CancelBtn_clicked();

    void on_OkBtn_clicked();

    void on_tradecodelineEdit_textChanged(const QString &arg1);

private:
    Ui::Phonedlg *ui;
    QSqlQuery query;
    int dlg;
    bool tradecodeCnt = false;
};

#endif // PHONEDLG_H
