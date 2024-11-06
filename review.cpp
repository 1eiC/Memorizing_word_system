//
// Created by 陈磊 on 24-11-4.
//

#include "review.h"

Review::Review(const QList<Word> &wrongWords, QWidget *parent)
    : QWidget(parent), wrongWords(wrongWords) {
    setupUI();
    qDebug() << "Review 中的错题数量:" << wrongWords.size();
}

void Review::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("错题复习", this);
    layout->addWidget(titleLabel);
    wrongWordsTable = new QTableWidget(this);
    layout->addWidget(wrongWordsTable);

    redoButton = new QPushButton("重做错题", this);
    exportButton = new QPushButton("导出错题", this);
    layout->addWidget(redoButton);
    layout->addWidget(exportButton);
    loadWrongWords();  // 加载错题数据到表格
    if (wrongWords.isEmpty()) {
        QLabel *noMistakesLabel = new QLabel("没有错题，继续保持！", this);
        layout->addWidget(noMistakesLabel);
    } else {
        for (const Word &word : wrongWords) {
            // 使用 join 方法将多个释义组合成一个字符串
            QString meaningsString = word.getMeanings().join(", "); // 使用逗号分隔多个释义
            QString wordInfo = word.getEnglish() + " (" + word.getPartOfSpeech() + ") - " + meaningsString; // 使用空格分隔单词和词性
            QLabel *wordLabel = new QLabel(wordInfo, this);// 创建一个标签来显示单词信息
            layout->addWidget(wordLabel);
        }
    }

    backButton = new QPushButton("返回主菜单", this);
    layout->addWidget(backButton);

    connect(backButton, &QPushButton::clicked, this, &Review::onBackButtonClicked);
    connect(redoButton, &QPushButton::clicked, this, &Review::onRedoButtonClicked);
    connect(exportButton, &QPushButton::clicked, this, &Review::onExportButtonClicked);
}

void Review::onBackButtonClicked() {
    emit backToMainMenu(); // 发射返回主菜单信号
}

void Review::loadWrongWords() {
    // 从文件中加载错题
    //wrongWords.clear();  // 清空现有数据
    QFile file("wrong_words.txt");  // 存储错题数据的文件名为 wrong_words.txt

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "加载错题失败", "无法打开文件：wrong_words.txt");
        return; // 如果文件无法打开，直接返回
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() < 3) {
            // 如果格式错误，继续处理下一行
            continue;
        }

        QString english = parts[0].trimmed();
        QString partOfSpeech = parts[1].trimmed();
        QStringList meanings;

        // 从第三个字段开始都是释义
        for (int i = 2; i < parts.size(); ++i) {
            meanings.append(parts[i].trimmed());
        }

        wrongWords.append(Word(english, partOfSpeech, meanings));
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
        const Word &word = wrongWords[i];
        bool ok;
        QString userInput = QInputDialog::getText(this, "重做错题", "请输入 " + word.getEnglish() + " 的释义:", QLineEdit::Normal, "", &ok);
        if (!ok) {
            return;
        }
        if (word.getMeanings().contains(userInput.trimmed())) {
            QMessageBox::information(this, "正确", "回答正确！");
            wrongWords.removeAt(i);
            --i;  // 调整索引以考虑移除的元素
        } else {
            QMessageBox::warning(this, "错误", "回答错误，请再试一次。");
        }
    }

    QMessageBox::information(this, "完成", "已完成所有错题的重做。");
    loadWrongWords(); // 更新显示
}

void Review::onExportButtonClicked() {
    qDebug() << "导出时错题数量:" << wrongWords.size(); // 调试输出
    if (wrongWords.isEmpty()) {
        QMessageBox::information(this, "没有错题", "没有错题可供导出。");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "导出错题", "", "文本文件 (*.txt);;所有文件 (*)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName); // 创建文件对象
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const Word &word : wrongWords) { // 遍历所有单词
            out << word.getEnglish() << "," << word.getPartOfSpeech() << "," << word.getMeanings().join(", ") << "\n";
        } // 输出单词信息
        file.close();
        QMessageBox::information(this, "导出成功", "错题已成功导出。");
    } else {
        QMessageBox::warning(this, "导出失败", "无法打开文件进行写入。");
    }
}
