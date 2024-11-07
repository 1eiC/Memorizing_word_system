//
// Created by 陈磊 on 24-11-4.
//

#include "login.h"

#include "mainwindow.h"
#include "user.h"

// Login 类构造函数，初始化登录界面
Login::Login(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this); // 创建垂直布局

    // 创建用户名输入框，设置占位符
    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("用户名");
    // 创建密码输入框，设置占位符，并将显示模式设置为密码类型
    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("密码");
    passwordInput->setEchoMode(QLineEdit::Password);
    QString lineEditStyle = "QLineEdit {"
                        "    font-size: 16px;"               // 设置字体大小
                        "    padding: 8px;"                  // 设置内边距，让控件显得更大
                        "    border: 2px solid #d3d3d3;"     // 设置边框
                        "    border-radius: 5px;"            // 圆角边框
                        "    background-color: #f9f9f9;"     // 设置背景颜色
                        "    color: #333;"                   // 设置文本颜色
                        "}"
                        "QLineEdit:focus {"
                        "    border: 2px solid #4CAF50;"     // 聚焦时改变边框颜色
                        "}";

    // 应用样式到用户名输入框
    usernameInput->setStyleSheet(lineEditStyle);
    // 应用样式到密码输入框
    passwordInput->setStyleSheet(lineEditStyle);
    layout->addWidget(usernameInput); // 将用户名输入框添加到布局中
    layout->addWidget(passwordInput); // 将密码输入框添加到布局中

    // 创建登录按钮，并将其添加到布局中
    CustomButton *loginButton = new CustomButton("登录", this);
    layout->addWidget(loginButton);
    connect(loginButton, &CustomButton::clicked, this, &Login::onLogin); // 连接登录按钮的点击信号到 onLogin 槽函数

    // 创建注册按钮，并将其添加到布局中
    CustomButton *registerButton = new CustomButton("注册", this);
    layout->addWidget(registerButton);
    connect(registerButton, &CustomButton::clicked, this, &Login::onRegister); // 连接注册按钮的点击信号到 onRegister 槽函数
}

// 登录按钮的槽函数
void Login::onLogin() {
    QString username = usernameInput->text().trimmed(); // 获取并去除用户名输入框中的空格
    QString password = passwordInput->text().trimmed(); // 获取并去除密码输入框中的空格

    // 使用 User 类的 authenticate 方法验证用户名和密码
    if (User::authenticate(username, password)) {
        QMessageBox::information(this, "登录成功", "欢迎，" + username + "！"); // 弹出登录成功信息框
        emit loginSuccessful(); // 发送登录成功信号
        emit usernameSent(username);
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！"); // 弹出登录失败警告框
    }
}

// 注册按钮的槽函数
void Login::onRegister() {
    emit switchToRegister(); // 发送切换到注册页面的信号
}
