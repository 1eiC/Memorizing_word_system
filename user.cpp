#include "user.h"
#include <Qfile>
#include <QTextStream>
// 静态成员变量初始化
QMap<QString, QString> User::userDatabase;
const QString User::userDatabaseFile = "user_database.txt"; // 用户数据库文件路径

User::User(const QString &username, const QString &password)
    : username(username), password(password) {}

// 获取用户名的实现
QString User::getUsername() const {
    return username;
}

// 验证密码的实现
bool User::verifyPassword(const QString &password) const {
    return this->password == password;
}

// 注册用户的实现
bool User::registerUser(const QString &username, const QString &password) {
    // 如果数据库为空，先加载数据
    if (userDatabase.isEmpty()) {
        loadUserDatabase();
    }

    if (userExists(username)) {
        return false; // 用户已存在，注册失败
    }
    userDatabase[username] = password; // 保存用户信息
    return true;// 注册成功
}

// 检查用户是否已存在的实现
bool User::userExists(const QString &username) {
    return userDatabase.contains(username); // 返回用户是否存在
}

bool User::authenticate(const QString &username, const QString &password) {
    loadUserDatabase(); // 加载用户数据
    return userDatabase.contains(username) && userDatabase[username] == password; // 检查用户名和密码是否匹配
}

// 从文件加载用户数据
void User::loadUserDatabase() {
    QFile file(userDatabaseFile); // 创建文件对象
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) { // 以只读文本模式打开文件
        QTextStream in(&file); // 创建文本流对象
        while (!in.atEnd()) { // 逐行读取文件内容
            QString line = in.readLine(); // 读取一行
            QStringList parts = line.split(':'); // 以冒号分割
            if (parts.size() == 2) { // 至少包含用户名和密码
                QString username = parts[0].trimmed(); // 去除首尾空格
                QString password = parts[1].trimmed(); // 去除首尾空格
                userDatabase[username] = password; // 保存到用户数据库
            }
        }
        file.close();
    }
}

// 将用户数据保存到文件
void User::saveUserDatabase() {
    QFile file(userDatabaseFile); // 创建文件对象
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) { // 以只写文本模式打开文件
        QTextStream out(&file);
        for (auto it = userDatabase.begin(); it != userDatabase.end(); ++it) { // 遍历用户数据库
            out << it.key() << ":" << it.value() << "\n"; // 写入用户名和密码
        }
        file.close(); // 关闭文件
    }
}