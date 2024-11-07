//
// Created by 陈磊 on 24-11-4.
//

#ifndef REVIEW_H
#define REVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "word.h"
# include "vocabulary.h"
# include "QTableWidget"
# include "QFileDialog"
# include  <QMessageBox>
# include <QDebug>
#include  <QFile>
#include <QTextStream>
#include <QInputDialog>

class CustomButton;

class Review : public QWidget {
    Q_OBJECT // 必须添加，否则无法使用信号槽

public:
    explicit Review(const QList<Word> &wrongWords, QWidget *parent = nullptr); // 构造函数
    QList<Word> getWrongWords() const;  // 获取错题列表
    QList<Word> wrongWords; // 错题列表

    void onExportButtonClicked(); // 导出错题
    void onRedoButtonClicked(); // 重新做错题
    signals:
        void backToMainMenu(); // 返回主菜单信号

    private slots:
        void onBackButtonClicked(); // 返回按钮槽函数


private:
    void setupUI(); // 设置界面
    QTableWidget *wrongWordsTable;      // 显示错题列表的表格
    CustomButton *redoButton;            // 重做错题按钮
    CustomButton *exportButton;          // 导出错题按钮

    void loadWrongWords();   // 加载错题数据
    CustomButton *backButton; // 返回按钮
};

#endif // REVIEW_H

