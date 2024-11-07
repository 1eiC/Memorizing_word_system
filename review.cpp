//
// Created by 陈磊 on 24-11-4.
//

#include "review.h"

#include "mainwindow.h"

Review::Review(const QList<Word> &wrongWords, QWidget *parent)
    : QWidget(parent), wrongWords(wrongWords) {
    setupUI();
    qDebug() << "Review 中的错题数量:" << wrongWords.size();
}

void Review::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("错题复习", this);
    titleLabel->setStyleSheet("QLabel {"
                         "    font-size: 30px;"        // 设置字体大小
                         "    color: #333;"            // 设置文本颜色
                         "    font-weight: bold;"      // 加粗文本
                         "    margin-bottom: 15px;"    // 控件底部的间距
                         "}");
    layout->addWidget(titleLabel);
    wrongWordsTable = new QTableWidget(this);
    layout->addWidget(wrongWordsTable);

    redoButton = new CustomButton("重做错题", this);
    exportButton = new CustomButton("导出错题", this);
    layout->addWidget(redoButton);
    layout->addWidget(exportButton);
    loadWrongWords();  // 加载错题数据到表格
    if (wrongWords.isEmpty()) {
        QLabel *noMistakesLabel = new QLabel("没有错题，继续保持！", this);
        noMistakesLabel->setStyleSheet("QLabel {"
                         "    font-size: 30px;"        // 设置字体大小
                         "    color: #333;"            // 设置文本颜色
                         "    font-weight: bold;"      // 加粗文本
                         "    margin-bottom: 15px;"    // 控件底部的间距
                         "}");
        layout->addWidget(noMistakesLabel);
    } else {
        for (const Word &word : wrongWords) {
            // 使用 join 方法将多个释义组合成一个字符串
            QString meaningsString = word.getMeanings().join(", "); // 使用逗号分隔多个释义
            QString wordInfo = word.getEnglish() + " (" + word.getPartOfSpeech() + ") - " + meaningsString; // 使用空格分隔单词和词性
            QLabel *wordLabel = new QLabel(wordInfo, this);// 创建一个标签来显示单词信息
            wordLabel->setStyleSheet("QLabel {"
                         "    font-size: 30px;"        // 设置字体大小
                         "    color: #333;"            // 设置文本颜色
                         "    font-weight: bold;"      // 加粗文本
                         "    margin-bottom: 15px;"    // 控件底部的间距
                         "}"); // 设置样式
            layout->addWidget(wordLabel);
        }
    }

    backButton = new CustomButton("返回主菜单", this);
    layout->addWidget(backButton);

    connect(backButton, &CustomButton::clicked, this, &Review::onBackButtonClicked);
    connect(redoButton, &CustomButton::clicked, this, &Review::onRedoButtonClicked);
    connect(exportButton, &CustomButton::clicked, this, &Review::onExportButtonClicked);
}

void Review::onBackButtonClicked() {
    emit backToMainMenu(); // 发射返回主菜单信号
}

void Review::loadWrongWords() {
    // 从文件中加载错题
    //wrongWords.clear();  // 清空现有数据
    QFile file("wrong_words.txt");  // 存储错题数据的文件名为 wrong_words.txt

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) { // 以只读文本模式打开文件
        QMessageBox::warning(this, "加载错题失败", "无法打开文件：wrong_words.txt"); // 弹出警告对话框
        return; // 如果文件无法打开，直接返回
    }

    QTextStream in(&file); // 创建文本流对象
    while (!in.atEnd()) { // 循环读取文件内容
        QString line = in.readLine(); // 读取一行
        QStringList parts = line.split(","); // 以逗号分割

        if (parts.size() < 3) { // 至少包含英文、词性和一个释义
            // 如果格式错误，继续处理下一行
            continue; // 跳过当前循环
        }

        QString english = parts[0].trimmed(); // 去除首尾空格
        QString partOfSpeech = parts[1].trimmed(); // 去除首尾空格
        QStringList meanings; // 创建释义列表

        // 从第三个字段开始都是释义
        for (int i = 2; i < parts.size(); ++i) { // 遍历释义
            meanings.append(parts[i].trimmed()); // 去除首尾空格
        }

        wrongWords.append(Word(english, partOfSpeech, meanings)); // 添加单词
    }

    file.close();

    // 更新 QTableWidget
    wrongWordsTable->setRowCount(wrongWords.size()); // 设置行数为错题数量
    wrongWordsTable->setColumnCount(3); // 设置列数为3
    wrongWordsTable->setHorizontalHeaderLabels({"单词", "词性", "释义"}); // 设置表头

    for (int i = 0; i < wrongWords.size(); ++i) { // 遍历所有行
        const Word &word = wrongWords[i]; // 获取当前行的单词对象
        wrongWordsTable->setItem(i, 0, new QTableWidgetItem(word.getEnglish())); // 设置单元格内容
        wrongWordsTable->setItem(i, 1, new QTableWidgetItem(word.getPartOfSpeech())); // 设置单元格内容
        wrongWordsTable->setItem(i, 2, new QTableWidgetItem(word.getMeanings().join(", "))); // 设置单元格内容
    }
}

QList<Word> Review::getWrongWords() const { // 返回所有错题
    return wrongWords;
}

void Review::onRedoButtonClicked() {
    // 复习错题逻辑
    for (int i = 0; i < wrongWords.size(); ++i) {
        const Word &word = wrongWords[i]; // 获取当前错题单词
        bool ok; // 用于判断用户是否输入了内容
        QString userInput = QInputDialog::getText(this, "重做错题", "请输入 " + word.getEnglish() + " 的释义:", QLineEdit::Normal, "", &ok); // 弹出输入对话框
        if (!ok) { // 用户取消输入
            return; // 用户取消输入，退出函数
        }
        if (word.getMeanings().contains(userInput.trimmed())) { // 判断用户输入是否正确
            QMessageBox::information(this, "正确", "回答正确！"); // 弹出提示框
            wrongWords.removeAt(i); // 移除答对的单词
            --i;  // 调整索引以考虑移除的元素
        } else {
            QMessageBox::warning(this, "错误", "回答错误，请再试一次。"); // 弹出提示框
        }
    }

    QMessageBox::information(this, "完成", "已完成所有错题的重做。"); // 弹出提示框
    loadWrongWords(); // 更新显示
}


void Review::onExportButtonClicked() { // 导出错题
    qDebug() << "导出时错题数量:" << wrongWords.size(); // 调试输出
    if (wrongWords.isEmpty()) { // 如果没有错题
        QMessageBox::information(this, "没有错题", "没有错题可供导出。");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "导出错题", "", "文本文件 (*.txt);;所有文件 (*)"); // 弹出文件保存对话框
    if (fileName.isEmpty()) { // 如果用户取消操作
        return;
    }

    QFile file(fileName); // 创建文件对象
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) { // 以只写文本模式打开文件
        QTextStream out(&file);
        for (const Word &word : wrongWords) { // 遍历所有单词
            out << word.getEnglish() << "," << word.getPartOfSpeech() << "," << word.getMeanings().join(", ") << "\n"; // 输出单词信息
        } // 输出单词信息
        file.close();   // 关闭文件
        QMessageBox::information(this, "导出成功", "错题已成功导出。"); // 弹出提示框
    } else { // 如果无法打开文件
        QMessageBox::warning(this, "导出失败", "无法打开文件进行写入。"); // 弹出警告对话框
    }
}
