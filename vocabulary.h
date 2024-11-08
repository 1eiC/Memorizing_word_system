//
// Created by 陈磊 on 24-11-4.
//

#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <QList>
#include "word.h"

class Vocabulary {
public:
    Vocabulary(); // 构造函数

    void addWord(const Word &word); // 添加新单词
    const QVector<Word>& getWords() const; // 获取单词列表
    bool removeWord(const QString &english); // 删除单词
    bool modifyWord(const QString &english, const Word &newWord); // 修改单词
    bool updateWord(const QString &english, const QString &newPartOfSpeech, const QStringList &newMeanings); // 更新单词信息
    bool addMeaningToWord(const QString &english, const QString &newMeaning); // 补充新的释义
    Word* findWord(const QString &english); // 查找单词
    int size() const; // 获取单词本的大小

    void loadDefaultWords(const QString &filename); // 加载默认的单词表
    void saveToFile(const QString &filename); // 保存单词表到文件
private:
    QVector<Word> words; // 存储单词的列表
};

#endif // VOCABULARY_H


