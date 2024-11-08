//
// Created by 陈磊 on 24-11-4.
//

#include "statistics.h"
#include <QMessageBox>

Statistics::Statistics(const Vocabulary &vocabulary, const QList<Word> &wrongWords, const QList<Word> &learnedWords,
                       const QMap<QString, QPair<int, int>> &wordStats, int score, QWidget *parent)
    : QWidget(parent), vocabulary(vocabulary), wrongWords(wrongWords), learnedWords(learnedWords),
      wordStats(wordStats), score(score) {
    setupUI();
}

// 设置用户界面：初始化和布局UI组件
void Statistics::setupUI() {
    // 创建垂直布局对象
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 创建并添加标题标签
    QLabel *titleLabel = new QLabel("统计分析", this);
    titleLabel->setStyleSheet("QLabel {"
                         "    font-size: 30px;"        // 设置字体大小
                         "    color: #333;"            // 设置文本颜色
                         "    font-weight: bold;"      // 加粗文本
                         "    margin-bottom: 15px;"    // 控件底部的间距
                         "}");
    layout->addWidget(titleLabel);

    // 计算单词总数、已学习单词数和错误单词数
    int totalWords = vocabulary.getWords().size();

    int learnedWordsCount= learnedWords.size();
    int wrongWordsCount = wrongWords.size();

    // 创建并添加统计信息标签
    QLabel *statsLabel = new QLabel(QString("总单词数：%1\n已学习单词数：%2\n错误单词数：%3\n总分数：%4")
                                    .arg(totalWords)
                                    .arg(learnedWordsCount)
                                    .arg(wrongWordsCount)
                                    .arg(score), this);
    statsLabel->setStyleSheet("QLabel {"
                         "    font-size: 30px;"        // 设置字体大小
                         "    color: #333;"            // 设置文本颜色
                         "    font-weight: bold;"      // 加粗文本
                         "    margin-bottom: 15px;"    // 控件底部的间距
                         "}");
    layout->addWidget(statsLabel);

    // 创建导出按钮和返回按钮
    exportButton = new CustomButton("导出统计数据", this);
    backButton = new CustomButton("返回主菜单", this);

    // 将按钮添加到布局中
    layout->addWidget(exportButton);
    layout->addWidget(backButton);

    // 连接按钮点击事件到对应的槽函数
    connect(exportButton, &CustomButton::clicked, this, &Statistics::onExportButtonClicked);
    connect(backButton, &CustomButton::clicked, this, &Statistics::onBackButtonClicked);
}


void Statistics::onExportButtonClicked() {
    // 弹出文件保存对话框，让用户选择保存路径和文件名
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出统计数据"), "", tr("文本文件 (*.txt);;所有文件 (*)"));
    if (fileName.isEmpty()) {
        return; // 如果用户取消操作，则返回
    }

    // 创建文件对象
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) { // 以只写文本模式打开文件
        QMessageBox::warning(this, "错误", "无法创建文件，请检查文件路径和权限。", QMessageBox::Ok); // 弹出警告对话框
        return;
    }
    // 调试输出：检查 wordStats 的大小
    qDebug() << "导出时 wordStats 的大小:" << wordStats.size();

    // 将统计数据写入文件
    QTextStream out(&file);
    out << "Statistics\n";
    out << "-------------------\n";

    for (auto it = wordStats.constBegin(); it != wordStats.constEnd(); ++it) { // 遍历所有单词
        QString word = it.key();
        int attempts = it.value().first;  // 出现次数
        int errors = it.value().second;   // 错误次数

        out << "Word: " << word << "\n"; // 输出单词
        out << "Number_of_occurrences: " << attempts << "\n"; // 输出出现次数
        out << "Number_of_errors: " << errors << "\n"; // 输出错误次数
        out << "Correctness: " << ((attempts > 0) ? (1.0 - static_cast<double>(errors) / attempts) * 100 : 100) << "%\n"; // 输出正确率
        out << "-------------------\n";     // 分隔符
    }

    // 完成写入，关闭文件
    file.close();

    // 弹出成功提示框
    QMessageBox::information(this, "导出", "统计数据已成功导出到文件。", QMessageBox::Ok);

    // 发射导出统计数据信号
    emit exportStatistics();
}

void Statistics::onBackButtonClicked() { // 返回按钮槽函数
    emit backToMainMenu(); // 发射返回主菜单信号

}