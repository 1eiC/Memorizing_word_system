//
// Created by 陈磊 on 24-11-4.
//

#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

class CustomButton;

class Login : public QWidget {
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);

    signals:
        void loginSuccessful();  // 登录成功信号
        void switchToRegister(); // 切换到注册页面信号
    void usernameSent(const QString &username);// 登录成功后发送用户名

    private slots:
        void onLogin();          // 登录槽函数
        void onRegister();       // 注册槽函数

private:
    QLineEdit *usernameInput; // 用户名输入框
    QLineEdit *passwordInput; // 密码输入框
};

#endif // LOGIN_H
