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

bool Vocabulary::modifyWord(const QString &english, const Word &newWord) {
    for (int i = 0; i < words.size(); ++i) {
        if (words[i].getEnglish() == english) {
            words[i] = newWord;
            return true;
        }
    }
    return false;
}

void Vocabulary::addWord(const Word &word) {
    words.append(word);
}


const QVector<Word>& Vocabulary::getWords() const {
    return words;
}

bool Vocabulary::removeWord(const QString &english) {
    for (int i = 0; i < words.size(); ++i) {
        if (words[i].getEnglish() == english) {
            words.removeAt(i);
            return true;
        }
    }
    return false;
}

bool Vocabulary::updateWord(const QString &english, const QString &newPartOfSpeech, const QStringList &newMeanings) {
    for (int i = 0; i < words.size(); ++i) {
        if (words[i].getEnglish() == english) {
            words[i].setPartOfSpeech(newPartOfSpeech);
            for (const QString &meaning : newMeanings) {
                words[i].addMeaning(meaning);
            }
            return true;
        }
    }
    return false;
}

bool Vocabulary::addMeaningToWord(const QString &english, const QString &newMeaning) {
    for (int i = 0; i < words.size(); ++i) {
        if (words[i].getEnglish() == english) {
            words[i].addMeaning(newMeaning);
            return true;
        }
    }
    return false;
}

Word* Vocabulary::findWord(const QString &english) {
    for (int i = 0; i < words.size(); ++i) {
        if (words[i].getEnglish() == english) {
            return &words[i];
        }
    }
    return nullptr;
}

int Vocabulary::size() const {
    return words.size();
}

void Vocabulary::loadDefaultWords(const QString &filename) {
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8"); // 使用 UTF-8 编码读取文件
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() >= 3) {
                QString english = parts[0].trimmed();
                QString partOfSpeech = parts[1].trimmed();
                QStringList meanings;
                for (int i = 2; i < parts.size(); ++i) {
                    meanings.append(parts[i].trimmed());
                }
                addWord(Word(english, partOfSpeech, meanings));
                qDebug() << "读取成功" << line;
            } else {
                qDebug() << "无效的单词格式：" << line;
            }
        }
        file.close();
    } else {
        qDebug() << "无法打开文件：" << filename;
    }
}

void Vocabulary::saveToFile(const QString &filename) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        for (const Word &word : words) {
            out << word.getEnglish() << "," << word.getPartOfSpeech();
            for (const QString &meaning : word.getMeanings()) {
                out << "," << meaning;
            }
            out << "\n";
        }
        file.close();
    }
}