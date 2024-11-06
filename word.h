//
// Created by 陈磊 on 24-11-4.
//

#ifndef WORD_H
#define WORD_H

#include <QString>
#include <QStringList>

class Word {
public:
    // 构造函数
    Word(const QString &english, const QString &partOfSpeech, const QStringList &meanings); // 构造函数

    QString getEnglish() const; // 获取英文单词
    QString getPartOfSpeech() const; // 获取词性
    QStringList getMeanings() const; // 获取所有释义
    void addMeaning(const QString &meaning); // 增加释义
    void setPartOfSpeech(const QString &partOfSpeech); // 设置词性
    void setMeanings(const QStringList &meanings); // 设置所有释义
    // 增加出现次数和错误次数的方法
    void incrementOccurrence(); // 增加出现次数
    void incrementError(); // 增加错误次数
    int getOccurrenceCount() const; // 获取出现次数
    int getErrorCount() const; // 获取错误次数

private:
    QString english; // 英文单词
    QString partOfSpeech; // 词性
    QStringList meanings; // 中文释义列表
    int occurrenceCount; // 出现次数
    int errorCount; // 错误次数
};

#endif // WORD_H

