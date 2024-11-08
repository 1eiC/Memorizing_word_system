//
// Created by 陈磊 on 24-11-4.
//

#include "register.h"

#include "mainwindow.h"
#include "user.h"

Register::Register(QWidget *parent) : QWidget(parent) { // 注册界面
    QVBoxLayout *layout = new QVBoxLayout(this); // 垂直布局

    usernameInput = new QLineEdit(this); // 用户名输入框
    usernameInput->setPlaceholderText("用户名"); // 设置占位符
    layout->addWidget(usernameInput); // 将用户名输入框添加到布局中

    passwordInput = new QLineEdit(this); // 密码输入框
    passwordInput->setPlaceholderText("密码"); // 设置占位符
    passwordInput->setEchoMode(QLineEdit::Password); // 设置显示模式为密码类型
    layout->addWidget(passwordInput); // 将密码输入框添加到布局中

    CustomButton *registerButton = new CustomButton("注册", this); // 注册按钮
    layout->addWidget(registerButton); // 将注册按钮添加到布局中
    connect(registerButton, &CustomButton::clicked, this, &Register::onRegister); // 连接注册按钮的点击信号到 onRegister 槽函数
}

void Register::onRegister() {
    // 这里实现注册逻辑，例如保存用户名和密码
    QString username = usernameInput->text().trimmed(); // 去除两端的空白字符
    QString password = passwordInput->text().trimmed(); // 去除两端的空白字符

    if (username.isEmpty() || password.isEmpty()) { // 如果用户名或密码为空
        QMessageBox::warning(this, "注册失败", "用户名和密码不能为空！"); // 弹出警告框
        return;
    }

    // 调用User类的registerUser方法来注册用户
    if (User::registerUser(username, password)) {
        // 注册成功，将用户数据保存到文件
        User::saveUserDatabase();

        QMessageBox::information(this, "注册成功", "用户注册成功！"); // 弹出注册成功信息框
        emit registerSuccessful(); // 发送注册成功信号
        printf("1");
    } else {
        QMessageBox::warning(this, "注册失败", "用户名已存在，请选择其他用户名。"); // 弹出警告框
    }
    //emit registerSuccessful(); // 注册成功信号
    // printf("1");
}
