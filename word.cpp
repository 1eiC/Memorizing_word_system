//
// Created by 陈磊 on 24-11-4.
//

#include "word.h"

// 构造函数
Word::Word(const QString &english, const QString &partOfSpeech, const QStringList &meanings)
    : english(english), partOfSpeech(partOfSpeech), meanings(meanings),
      occurrenceCount(0), errorCount(0) {} //初始化出现次数和错误次数

// 获取英文单词
QString Word::getEnglish() const {
    return english;
}

// 获取词性
QString Word::getPartOfSpeech() const {
    return partOfSpeech;
}

// 获取所有释义
QStringList Word::getMeanings() const {
    return meanings;
}

// 增加释义
void Word::addMeaning(const QString &meaning) {
    if (!meanings.contains(meaning)) {
        meanings.append(meaning);
    }
}

// 设置词性
void Word::setPartOfSpeech(const QString &partOfSpeech) {
    this->partOfSpeech = partOfSpeech;
}

// 设置所有释义
void Word::setMeanings(const QStringList &meanings) {
    this->meanings = meanings;
}

// 增加出现次数
void Word::incrementOccurrence() {
    occurrenceCount++;
}

// 增加错误次数
void Word::incrementError() {
    errorCount++;
}

// 获取出现次数
int Word::getOccurrenceCount() const {
    return occurrenceCount;
}

// 获取错误次数
int Word::getErrorCount() const {
    return errorCount;
}