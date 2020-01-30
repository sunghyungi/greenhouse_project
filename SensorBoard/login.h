#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr, bool dbcon=true);
    ~Login();

private slots:
    void on_btn_ExitLogin_clicked();

    void on_btn_Login_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
