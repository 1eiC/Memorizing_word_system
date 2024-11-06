//
// Created by 陈磊 on 24-11-4.
//

#include "register.h"
#include "user.h"

Register::Register(QWidget *parent) : QWidget(parent) { // 注册界面
    QVBoxLayout *layout = new QVBoxLayout(this); // 垂直布局

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("用户名");
    layout->addWidget(usernameInput);

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("密码");
    passwordInput->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordInput);

    QPushButton *registerButton = new QPushButton("注册", this);
    layout->addWidget(registerButton);
    connect(registerButton, &QPushButton::clicked, this, &Register::onRegister);
}

void Register::onRegister() {
    // 这里实现注册逻辑，例如保存用户名和密码
    QString username = usernameInput->text().trimmed(); // 去除两端的空白字符
    QString password = passwordInput->text().trimmed(); // 去除两端的空白字符

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "用户名和密码不能为空！");
        return;
    }

    // 调用User类的registerUser方法来注册用户
    if (User::registerUser(username, password)) {
        // 注册成功，将用户数据保存到文件
        User::saveUserDatabase();

        QMessageBox::information(this, "注册成功", "用户注册成功！");
        emit registerSuccessful(); // 发送注册成功信号
        printf("1");
    } else {
        QMessageBox::warning(this, "注册失败", "用户名已存在，请选择其他用户名。");
    }
    //emit registerSuccessful(); // 注册成功信号
    // printf("1");
}
