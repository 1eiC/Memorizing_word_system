//
// Created by 陈磊 on 24-11-4.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "login.h"
#include "register.h"
#include "vocabulary.h"
#include "word.h"
#include "review.h"
#include <QFileDialog>
#include <QButtonGroup>
#include <QRadioButton>
#include <QTextStream>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
# include <QList>
#include <QMessageBox>
#include  <QDebug>
#include <QInputDialog>
#include <QString>
#include <algorithm> // 引入算法库以使用 std::random_shuffle
#include <random> // 引入随机库
# include <QMap>
# include <QSet>
# include <QFile>
# include <QTextStream>
# include <QDir>
#include <QDate>
# include <QTimer>
#include <QCoreApplication>
# include <QApplication>
#include <QPalette>
#include <QPixmap>
#include <QResizeEvent>

class CustomButton : public QPushButton { // 自定义按钮类
    Q_OBJECT
public:
    // 构造函数
    CustomButton(const QString &text, QWidget *parent = nullptr)
        : QPushButton(text, parent) { // 调用父类构造函数
        // 在这里设置按钮样式表
        setStyleSheet("QPushButton {"
    "    background-color: #4CAF50;"    // 绿色背景
    "    color: white;"                 // 白色文本
    "    border: 2px solid #4CAF50;"    // 边框与背景相同
    "    border-radius: 10px;"          // 圆角半径
    "    font-size: 16px;"              // 字体大小
    "    padding: 15px;"                // 内边距
    "}"
    "QPushButton:hover {"               // 鼠标悬停样式
    "    background-color: #45a049;"    // 鼠标悬停时颜色变化
    "}"
    "QPushButton:pressed {"             // 按下样式
    "    background-color: #3e8e41;"    // 按下时颜色变化
    "}");
    }
};

class Statistics;

class MainWindow : public QMainWindow {
    Q_OBJECT

public: // 公有成员变量
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    private slots: // 槽函数
        void onLoginSuccessful(); // 登录成功的槽函数
        void onSwitchToRegister(); // 切换到注册页面
        void onRegisterSuccessful(); // 注册成功的槽函数
        void MainMenu(); // 主菜单
        void startLearning1(); // 开始拼写背诵
        void startLearning2(); // 开始选择题背诵
        void startReview(); // 开始复习错题
        void performStatistics(); // 执行统计分析
        void import(); //开始导入单词表
        void previewVocabulary(); // 预览题库页面
        void addWord();           // 添加单词
        void modifyWord();        // 修改单词
        void deleteWord();        // 删除单词
        void supplementMeaning(); // 补充单词释义
        void onExportWrongWords(); // 导出错题
        void onRedoWrongWords(); // 重新做错题
        void onCardButtonClicked();//开始打卡
        void onCheckInButtonClicked(); // 打卡按钮点击
        void showCardPage();            // 显示打卡页面
        void onReturnToMainMenuClicked(); // 返回主菜单
        void onUsernameReceived(const QString &username); // 接收用户名


private: // 私有成员函数

    void showMainMenu(); // 显示单词本管理菜单
    void addBackgroundImage(); // 显示学习页面
    void resizeEvent(QResizeEvent *event) override; // 重写 resizeEvent 函数
    void showLearningPage1(); // 显示拼写背诵页面
    void updateWordLabel(const QVector<Word>& words);//随机选择并更新学习页面上显示的单词内容
    void showLearningPage2(); // 显示选择题背诵页面
    void updateMultipleChoiceQuestion();   // 更新选择题问题
    void checkMultipleChoiceAnswer(QButtonGroup *optionsGroup); // 检查用户选择的答案
    void showReviewPage(); // 显示错题本页面
    void showStatisticsPage(); // 显示统计页面
    void ImportPage(); //显示导入单词表页面
    void updateVocabularyFile(); // 更新题库文件
    void loadCheckinData();          // 加载打卡数据
    void saveCheckinData(const QDate &date);        // 保存打卡数据
    void updateCheckinDisplay();     // 更新显示的打卡信息
    QString getUserFilePath() const;        // 获取当前用户的打卡文件路径

    QList<QDate> recentCheckins; // 最近打卡日期列表
    int checkinCount = 0;            // 记录打卡天数
    QString username;  // 用户名
    mutable QString filePath;  // 文件路径缓存
    QWidget *CardPage = nullptr;// 打卡页面
    QLabel *infoLabel = nullptr;// 显示打卡信息
    // QLabel *infoLabel;
    int currentCheckInDays;// 当前打卡天数
    QDate lastCheckInDate;// 上次打卡日期

    Login *loginPage; // 登录页面
    Register *registerPage; // 注册页面
    Review *review;  //用于管理 Review 对象的指针
    Statistics *statistics; // 用于管理 Statistics 对象的指针
    QWidget *learningPage1; // 拼写背诵页面
    QWidget *learningPage2; // 选择题背诵页面
    QButtonGroup *optionsGroup; // 选项按钮组
    QList<QString> generateOptions(const QString &correctAnswer, bool isMeaning); // 生成干扰选项
    QWidget *reviewPage; // 错题本页面
    QWidget *statisticsPage; // 统计分析页面
    QWidget *importPage; //导入单词表页面
    QWidget *vocabularyPage; // 题库预览页面
    Vocabulary vocabulary; // 单词本
    QLabel *wordLabel; // 显示当前单词
    QLineEdit *inputField; // 用户输入框
    QPushButton *submitButton; // 提交按钮
    QTableWidget *vocabularyTable; // 预览题库用的表格
    QList<Word> wrongWords; // 错题本
    QList<Word> learnedWords; // 已学过的单词
    QMap<QString, QPair<int, int>> wordStats; // 记录每个单词的出现次数和错误次数
    QVector<Word> selectedWords; // 存储用户当前学习的单词集合
    QSet<int> usedIndexes; // 记录已出题的单词索引
    QString currentCorrectAnswer;  // 当前题目的正确答案
    int currentIndex; // 当前题目索引
    int totalScore = 0;  // 用于记录用户的总成绩
};

#endif // MAINWINDOW_H

