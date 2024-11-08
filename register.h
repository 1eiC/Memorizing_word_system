//
// Created by 陈磊 on 24-11-4.
//

#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
class Register : public QWidget {
    Q_OBJECT

public:
    Register(QWidget *parent = nullptr); // 构造函数

    signals:
        void registerSuccessful(); // 注册成功信号
        void switchToLogin();      // 切换到登录页面信号

    private slots:
        void onRegister();         // 注册槽函数

private:
    QLineEdit *usernameInput; // 用户名输入框
    QLineEdit *passwordInput; // 密码输入框
};

#endif // REGISTER_H


