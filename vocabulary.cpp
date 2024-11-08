//
// Created by 陈磊 on 24-11-4.
//

#include "vocabulary.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>


Vocabulary::Vocabulary() {
    loadDefaultWords("./wordlist.txt"); // 构造函数中加载默认单词
}

bool Vocabulary::modifyWord(const QString &english, const Word &newWord) { // 修改单词
    for (int i = 0; i < words.size(); ++i) { // 遍历单词列表
        if (words[i].getEnglish() == english) { // 找到要修改的单词
            words[i] = newWord; // 替换为新的单诫
            return true; // 返回成功
        }
    }
    return false;   // 未找到单词，返回失败
}

void Vocabulary::addWord(const Word &word) { // 添加单词
    words.append(word); // 添加到单词列表
}


const QVector<Word>& Vocabulary::getWords() const {      // 获取单词列表
    return words; // 返回单词列表
}

bool Vocabulary::removeWord(const QString &english) { // 删除单词
    for (int i = 0; i < words.size(); ++i) {    // 遍历单词列表
        if (words[i].getEnglish() == english) { // 找到要删除的单词
            words.removeAt(i); // 删除单词
            return true;    // 返回成功
        }
    }
    return false;   // 未找到单词，返回失败
}

bool Vocabulary::updateWord(const QString &english, const QString &newPartOfSpeech, const QStringList &newMeanings) { // 更新单词
    for (int i = 0; i < words.size(); ++i) {    // 遍历单词列表
        if (words[i].getEnglish() == english) { // 找到要更新的单词
            words[i].setPartOfSpeech(newPartOfSpeech); // 更新词性
            for (const QString &meaning : newMeanings) {    // 遍历新的释义列表
                words[i].addMeaning(meaning);   // 添加释义
            }
            return true;
        }
    }
    return false;
}

bool Vocabulary::addMeaningToWord(const QString &english, const QString &newMeaning) {  // 添加释义
    for (int i = 0; i < words.size(); ++i) {   // 遍历单词列表
        if (words[i].getEnglish() == english) {   // 找到要添加释义的单词
            words[i].addMeaning(newMeaning); // 添加释义
            return true;
        }
    }
    return false;
}

Word* Vocabulary::findWord(const QString &english) { // 查找单词
    for (int i = 0; i < words.size(); ++i) {  // 遍历单词列表
        if (words[i].getEnglish() == english) { // 找到单词
            return &words[i]; // 返回单词指针
        }
    }
    return nullptr;
}

int Vocabulary::size() const { // 获取单词数量
    return words.size(); // 返回单词列表的大小
}

void Vocabulary::loadDefaultWords(const QString &filename) { // 加载默认单词
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) { // 以只读文本模式打开文件
        QTextStream in(&file);  // 创建文本流对象
        in.setCodec("UTF-8"); // 使用 UTF-8 编码读取文件
        while (!in.atEnd()) {  // 循环读取文件内容
            QString line = in.readLine(); // 读取一行
            QStringList parts = line.split(","); // 以逗号分割
            if (parts.size() >= 3) {   // 至少包含英文、词性和一个释义
                QString english = parts[0].trimmed();   // 去除首尾空格
                QString partOfSpeech = parts[1].trimmed(); // 去除首尾空格
                QStringList meanings;  // 创建释义列表
                for (int i = 2; i < parts.size(); ++i) { // 从第三个字段开始都是释义
                    meanings.append(parts[i].trimmed()); // 去除首尾空格
                }
                addWord(Word(english, partOfSpeech, meanings)); // 添加单词
                qDebug() << "读取成功" << line;
            } else {
                qDebug() << "无效的单词格式：" << line;
            }
        }
        file.close(); // 关闭文件
    } else {
        qDebug() << "无法打开文件：" << filename; // 输出错误信息
    }
}

void Vocabulary::saveToFile(const QString &filename) { // 保存到文件
    QFile file(filename);   // 创建文件对象
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) { // 以只写文本模式打开文件
        QTextStream out(&file); // 创建文本流对象
        out.setCodec("UTF-8");  // 使用 UTF-8 编码写入文件
        for (const Word &word : words) {   // 遍历单词列表
            out << word.getEnglish() << "," << word.getPartOfSpeech(); // 写入英文和词性
            for (const QString &meaning : word.getMeanings()) { // 遍历释义列表
                out << "," << meaning; // 写入释义
            }
            out << "\n";   // 换行
        }
        file.close();
    }
}