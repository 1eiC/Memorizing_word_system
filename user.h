//
// Created by 陈磊 on 24-11-4.
//

#ifndef USER_H
#define USER_H

#include <QString>
#include <QMap>

class User {
public:
    User(const QString &username, const QString &password);

    QString getUsername() const; // 获取用户名
    bool verifyPassword(const QString &password) const; // 验证密码

    static bool registerUser(const QString &username, const QString &password); // 注册用户
    static bool userExists(const QString &username); // 检查用户是否已存在
    static bool authenticate(const QString &username, const QString &password);
    static void loadUserDatabase();//从文件加载用户数据
    static void saveUserDatabase(); // 保存用户数据到文件

private:
    QString username; // 用户名
    QString password; // 密码

    static QMap<QString, QString> userDatabase; // 存储用户信息的静态数据库
    static const QString userDatabaseFile;//用户数据文件路径
};

#endif // USER_H
