//
// Created by 陈磊 on 24-11-4.
//

#ifndef STATISTICS_H
#define STATISTICS_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "vocabulary.h"
#include "user.h"
#include <QTextStream>
#include <QInputDialog>
#include <QFile>
#include  <QFileDialog>
#include  <QDebug>
#include "mainwindow.h"


class Statistics : public QWidget {
    Q_OBJECT

public:
    explicit Statistics(const Vocabulary &vocabulary, const QList<Word> &wrongWords, const QList<Word> &learnedWords,
                        const QMap<QString, QPair<int, int>> &wordStats, int score, QWidget *parent = nullptr); // 构造函数

    signals:
        void backToMainMenu(); // 返回主菜单信号
        void exportStatistics(); // 导出统计数据信号

    private slots:
        void onExportButtonClicked(); // 导出按钮槽函数
        void onBackButtonClicked(); // 返回按钮槽函数

private:
    //Statistics(const Vocabulary &vocabulary, const QList<Word> &wrongWords, const QList<Word> &learnedWords,
               //const QMap<QString, QPair<int, int>> &wordStats, QWidget *parent);

    void setupUI(); // 设置界面

    const Vocabulary &vocabulary; // 单词本
    const QList<Word> &wrongWords; // 错题本
    const QList<Word> &learnedWords; // 已学过的单词
    CustomButton *exportButton; // 导出按钮
    CustomButton *backButton; // 返回按钮
    QMap<QString, QPair<int, int>> wordStats; // 记录每个单词的出现次数和错误次数
    QMap<QString, QPair<int, int>> wordStat; // 记录每个单词的出现次数和错误次数
    int score;  // 用于记录成绩

};

#endif // STATISTICS_H

