//
// Created by 陈磊 on 24-11-4.
//

#include "mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QRandomGenerator>

#include "statistics.h"

MainWindow::MainWindow(QWidget *parent) // 构造函数
    : QMainWindow(parent), currentIndex(0) {    // 初始化成员变量
    loginPage = new Login(this); // 登录页面
    registerPage = new Register(this); // 注册页面

    setCentralWidget(loginPage); // 初始显示登录页面

    connect(loginPage, &Login::loginSuccessful, this, &MainWindow::onLoginSuccessful); // 登录成功后，触发该信号
    connect(loginPage, &Login::switchToRegister, this, &MainWindow::onSwitchToRegister); // 点击注册后，触发该信号
    connect(registerPage, &Register::registerSuccessful, this, &MainWindow::onRegisterSuccessful); // 注册成功后，触发该信号
    connect(loginPage, &Login::usernameSent, this, &MainWindow::onUsernameReceived); // 登录成功后，触发该信号
}

MainWindow::~MainWindow() {
}

void MainWindow::onLoginSuccessful() {
    showMainMenu(); // 登录成功后，显示单词本管理菜单
}

void MainWindow::onSwitchToRegister() {
    setCentralWidget(registerPage); // 切换到注册页面
}

void MainWindow::onRegisterSuccessful() {
    showMainMenu(); // 登录成功后，显示单词本管理菜单
}

void MainWindow::MainMenu() {
    showMainMenu(); //显示单词本管理菜单
}

void MainWindow::onUsernameReceived(const QString &username) // 接收用户名
{
    this->username = username; // 更新MainWindow中的username变量
    // 计算距离上次打卡的天数
    QDate currentDate = QDate::currentDate();
    loadCheckinData();
    qDebug() << "上次打卡日期：" << lastCheckInDate.toString(Qt::ISODate);
    int daysSinceLastCheckIn = lastCheckInDate.isValid() ? lastCheckInDate.daysTo(currentDate) : 0;
    qDebug() << "距离上次打卡的天数：" << daysSinceLastCheckIn;
    // 显示提示框
    QMessageBox::information(this, "打卡提示", QString("您已经 %1 天没有打卡了！").arg(daysSinceLastCheckIn));
}

void MainWindow::showMainMenu() {
    // 主菜单页面布局
    QWidget *mainMenu = new QWidget(this);
    QVBoxLayout *menuLayout = new QVBoxLayout(mainMenu);
    CustomButton *preview = new CustomButton("预览", this);
    CustomButton *learnButton1 = new CustomButton("拼写背诵", this);
    CustomButton *learnButton2 = new CustomButton("选择题背诵", this);
    CustomButton *reviewButton = new CustomButton("复习错题", this);
    CustomButton *statisticsButton = new CustomButton("统计分析", this);
    CustomButton *cardButton = new CustomButton("打卡", this);
    CustomButton *importButton = new CustomButton("导入单词表",this);
    CustomButton *logoutButton = new CustomButton("退出", this);

    // 添加按钮
    menuLayout->addWidget(preview);
    menuLayout->addWidget(learnButton1);
    menuLayout->addWidget(learnButton2);
    menuLayout->addWidget(reviewButton);
    menuLayout->addWidget(statisticsButton);
    menuLayout->addWidget(cardButton);
    menuLayout->addWidget(importButton);
    menuLayout->addWidget(logoutButton);
    menuLayout->setSpacing(70);  // 控件之间的间距
    menuLayout->setContentsMargins(200, 160, 200, 160);  // 界面四周的边距
    mainMenu->setLayout(menuLayout); // 设置布局
    addBackgroundImage(); // 添加背景图片
    setCentralWidget(mainMenu); // 显示主菜单

    connect(preview, &CustomButton::clicked, this, &MainWindow::previewVocabulary);
    connect(learnButton1, &CustomButton::clicked, this, &MainWindow::startLearning1);
    connect(learnButton2, &CustomButton::clicked, this, &MainWindow::startLearning2);
    connect(reviewButton, &CustomButton::clicked, this, &MainWindow::startReview);
    connect(statisticsButton, &CustomButton::clicked, this, &MainWindow::performStatistics);
    connect(importButton, &CustomButton::clicked, this, &MainWindow::import);
    connect(cardButton, &CustomButton::clicked, this, &MainWindow::onCardButtonClicked);
    connect(logoutButton, &CustomButton::clicked, this, [this]() {
        QApplication::quit(); // 退出程序
    });
}

void MainWindow::addBackgroundImage() {
    QPalette palette = this->palette(); // 获取当前窗口的调色板
    QPixmap pixmap("./OIP.png"); // 加载背景图片

    // 确保图片被拉伸到整个窗口大小
    pixmap = pixmap.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation); // 按比例拉伸图片

    palette.setBrush(QPalette::Window, QBrush(pixmap)); // 设置背景图片
    this->setPalette(palette); // 设置调色板
}

void MainWindow::resizeEvent(QResizeEvent *event) { // 窗口大小改变事件
    addBackgroundImage(); // 窗口大小改变时，重新设置背景图片
    QMainWindow::resizeEvent(event); // 调用基类的 resizeEvent 函数
}


void MainWindow::startLearning1() {
    showLearningPage1(); // 切换到拼写背诵
}

void MainWindow::startLearning2() {
    showLearningPage2(); // 切换到选择题背诵
}

void MainWindow::startReview() {
    showReviewPage(); // 切换到复习错题页面
}

void MainWindow::performStatistics() {
    showStatisticsPage(); // 切换到统计页面
}

void MainWindow::import() {
    ImportPage(); //切换到导入单词表页面
}

void MainWindow::onCardButtonClicked() {
    showCardPage();
}

void MainWindow::previewVocabulary() {
    // 创建词汇页面和布局
    vocabularyPage = new QWidget(this);
    QVBoxLayout *vocabularyLayout = new QVBoxLayout(vocabularyPage);

    // 初始化词汇表，设置列数和表头
    vocabularyTable = new QTableWidget(this);
    vocabularyTable->setColumnCount(3);
    QStringList headers;
    headers << "单词" << "词性" << "释义";
    vocabularyTable->setHorizontalHeaderLabels(headers);

    // 获取词汇数据并填充到词汇表中
    const QVector<Word> &words = vocabulary.getWords();
    vocabularyTable->setRowCount(words.size());
    for (int i = 0; i < words.size(); ++i) {
        vocabularyTable->setItem(i, 0, new QTableWidgetItem(words[i].getEnglish()));
        vocabularyTable->setItem(i, 1, new QTableWidgetItem(words[i].getPartOfSpeech()));
        vocabularyTable->setItem(i, 2, new QTableWidgetItem(words[i].getMeanings().join("; ")));
    }

    // 创建操作按钮：添加、修改、删除单词及返回主菜单
    CustomButton *addButton = new CustomButton("添加单词", this);
    CustomButton *modifyButton = new CustomButton("修改单词", this);
    CustomButton *deleteButton = new CustomButton("删除单词", this);
    CustomButton *backButton = new CustomButton("返回主菜单", this);

    // 将词汇表和按钮添加到布局中
    vocabularyLayout->addWidget(vocabularyTable);
    vocabularyLayout->addWidget(addButton);
    vocabularyLayout->addWidget(modifyButton);
    vocabularyLayout->addWidget(deleteButton);
    vocabularyLayout->addWidget(backButton);

    // 设置中央部件为词汇页面
    vocabularyPage->setLayout(vocabularyLayout);
    setCentralWidget(vocabularyPage);

    // 连接按钮点击信号到对应的槽函数
    connect(addButton, &CustomButton::clicked, this, &MainWindow::addWord);
    connect(modifyButton, &CustomButton::clicked, this, &MainWindow::modifyWord);
    connect(deleteButton, &CustomButton::clicked, this, &MainWindow::deleteWord);
    connect(backButton, &CustomButton::clicked, this, &MainWindow::showMainMenu);
}


void MainWindow::addWord() {
    // 添加单词逻辑
    QString english = QInputDialog::getText(this, "添加单词", "请输入单词：");
    if (english.isEmpty()) {
        return; // 用户取消输入
    }

    QString partOfSpeech = QInputDialog::getText(this, "添加词性", "请输入词性：");
    if (partOfSpeech.isEmpty()) {
        return; // 用户取消输入
    }

    QString meaningsStr = QInputDialog::getText(this, "添加释义", "请输入释义（多个释义用逗号分隔）：");
    if (meaningsStr.isEmpty()) {
        return; // 用户取消输入
    }
    QStringList meanings = meaningsStr.split(",", Qt::SkipEmptyParts); // 获取释义列表

    vocabulary.addWord(Word(english, partOfSpeech, meanings));
    updateVocabularyFile();
    previewVocabulary(); // 更新预览
}

void MainWindow::modifyWord() {
    // 修改单词逻辑
    int currentRow = vocabularyTable->currentRow(); // 获取当前选中的行
    if (currentRow >= 0) {
        QString english = vocabularyTable->item(currentRow, 0)->text(); // 获取当前行的英文单词
        QString newPartOfSpeech = QInputDialog::getText(this, "修改词性", "请输入新的词性：");
        if (newPartOfSpeech.isEmpty()) {
            return;
        }

        QString newMeaningsStr = QInputDialog::getText(this, "修改释义", "请输入新的释义（多个释义用逗号分隔）：");
        if (newMeaningsStr.isEmpty()) {
            return;
        }
        QStringList newMeanings = newMeaningsStr.split(",", Qt::SkipEmptyParts); // 获取释义列表

        Word newWord(english, newPartOfSpeech, newMeanings); // 创建新的 Word 对象
        vocabulary.modifyWord(english, newWord); // 修改单词
        updateVocabularyFile(); // 保存到文件
        previewVocabulary(); // 更新预览
    }
}

void MainWindow::deleteWord() {
    // 删除单词逻辑
    int currentRow = vocabularyTable->currentRow(); // 获取当前选中的行
    if (currentRow >= 0) {
        QString english = vocabularyTable->item(currentRow, 0)->text(); // 获取当前行的英文单词
        vocabulary.removeWord(english); // 删除单词
        updateVocabularyFile(); // 保存到文件
        previewVocabulary(); // 更新预览
    }
}


void MainWindow::supplementMeaning() {
    // 补充单词释义的逻辑
    int currentRow = vocabularyTable->currentRow(); // 获取当前选中的行
    if (currentRow >= 0) {
        QString english = vocabularyTable->item(currentRow, 0)->text(); // 获取当前行的英文单词
        QString newMeaning = QInputDialog::getText(this, "补充释义", "请输入新的释义：");
        if (!newMeaning.isEmpty()) {
            vocabulary.addMeaningToWord(english, newMeaning); // 添加释义
            updateVocabularyFile(); // 保存到
            previewVocabulary(); // 更新预览
        }
    } else {
        QMessageBox::warning(this, "未选择单词", "请先选择一个单词以补充释义。"); // 提示用户未选择单词
    }
}


void MainWindow::updateVocabularyFile() {
    vocabulary.saveToFile("wordlist.txt"); // 保存到文件
}


void MainWindow::showLearningPage1() {
    learningPage1 = new QWidget(this);
    QVBoxLayout *learningLayout = new QVBoxLayout(learningPage1);

    wordLabel = new QLabel(this);
    wordLabel->setStyleSheet("QLabel {"
                             "    font-size: 30px;"       // 字体大小
                             "    font-weight: bold;"     // 字体加粗
                             "    color: #333;"           // 文本颜色
                             "}");

    inputField = new QLineEdit(this);
    inputField->setPlaceholderText("请输入你的答案...");
    inputField->setStyleSheet("QLineEdit {"
                              "    font-size: 16px;"       // 字体大小
                              "    color: #333;"           // 文本颜色
                              "    padding: 8px;"          // 内边距
                              "    border: 2px solid #4CAF50;" // 边框颜色
                              "    border-radius: 8px;"    // 圆角边框
                              "    background-color: #ffffff;" // 背景色
                              "}"
                              "QLineEdit:focus {"
                              "    border: 2px solid #3e8e41;" // 聚焦时边框颜色
                              "}");
    submitButton = new CustomButton("提交", this);
    CustomButton *returnToMenuButton = new CustomButton("返回主菜单", this); // 返回主菜单按钮
    learningLayout->addWidget(wordLabel);
    learningLayout->addWidget(inputField);
    learningLayout->addWidget(submitButton);
    learningLayout->addWidget(returnToMenuButton);
    learningLayout->setSpacing(80); // 控件之间的间距

    learningPage1->setLayout(learningLayout);
    setCentralWidget(learningPage1);

    currentIndex = 0; // 初始化学习索引
    size_t maxQuestions = 10; // 设置题目上限为10个

    // 获取词汇表并随机打乱
    QVector<Word> allWords = vocabulary.getWords(); // 假设这个方法返回 QVector<Word>
    std::shuffle(allWords.begin(), allWords.end(), std::mt19937(std::random_device()())); // 打乱顺序

    // 只选择前 maxQuestions 个单词进行学习
    selectedWords = allWords.mid(0, std::min(static_cast<size_t>(allWords.size()), maxQuestions));

    if (!selectedWords.isEmpty()) {
        updateWordLabel(selectedWords); // 更新单词标签
    }

    // 连接信号和槽
    connect(returnToMenuButton, &CustomButton::clicked, this, &MainWindow::showMainMenu);
    connect(submitButton, &CustomButton::clicked, this, [this]() {
        QString userInput = inputField->text().trimmed();
        const Word& currentWord = selectedWords[currentIndex];

        // 增加单词出现次数
        wordStats[currentWord.getEnglish()].first++;

        // 根据显示的语言来检查用户的输入
        bool showEnglish = wordLabel->text().startsWith(currentWord.getEnglish());
        QStringList correctAnswers;

        if (showEnglish) {
            // 用户看到的是英文，检查输入的中文释义
            correctAnswers = currentWord.getMeanings();
            if (correctAnswers.contains(userInput)) {
                QMessageBox::information(this, "正确", "回答正确！", QMessageBox::Ok);
                currentIndex++;
                if (currentIndex < selectedWords.size()) { // 限制题目数量
                    updateWordLabel(selectedWords); // 更新单词标签
                    inputField->clear();
                } else {
                    QMessageBox::information(this, "完成", "已完成所有单词学习！", QMessageBox::Ok);
                    showMainMenu();
                }
            } else {
                QMessageBox::warning(this, "错误", "回答错误，请再试一次。", QMessageBox::Ok);
                // 增加错误次数
                wordStats[currentWord.getEnglish()].second++;
                wrongWords.append(currentWord); // 将错误的单词加入错题本
                inputField->clear();
            }
        } else {
            // 用户看到的是中文，检查输入的英文单词
            if (userInput == currentWord.getEnglish()) {
                QMessageBox::information(this, "正确", "回答正确！", QMessageBox::Ok);
                totalScore += 10;  // 答对加10分
                currentIndex++;
                if (currentIndex < selectedWords.size()) { // 限制题目数量
                    updateWordLabel(selectedWords); // 更新单词标签
                    inputField->clear();
                } else {
                    QMessageBox::information(this, "完成", "已完成所有单词学习！", QMessageBox::Ok);
                    showMainMenu();
                }
            } else {
                QMessageBox::warning(this, "错误", "回答错误，请再试一次。", QMessageBox::Ok);
                // 增加错误次数
                wordStats[currentWord.getEnglish()].second++;
                totalScore -= 10;  // 答错扣10分
                wrongWords.append(currentWord); // 将错误的单词加入错题本
                inputField->clear();
            }
        }
    });
}
// 更新单词标签的方法
void MainWindow::updateWordLabel(const QVector<Word>& words) {
    // 随机选择显示中文或英文
    bool showEnglish = QRandomGenerator::global()->bounded(2) == 0; // 50% 概率
    const Word& currentWord = words[currentIndex];

    if (showEnglish) {
        wordLabel->setText(currentWord.getEnglish() + " (" + currentWord.getPartOfSpeech() + ")");
    } else {
        wordLabel->setText(currentWord.getMeanings().join(", ") + " (" + currentWord.getPartOfSpeech() + ")");
    }
}

void MainWindow::showLearningPage2() {
    learningPage2 = new QWidget(this);
    QVBoxLayout *learningLayout = new QVBoxLayout(learningPage2);

    wordLabel = new QLabel(this);
    wordLabel->setStyleSheet("QLabel {"
                         "    font-size: 30px;"        // 设置字体大小
                         "    color: #333;"            // 设置文本颜色
                         "    font-weight: bold;"      // 加粗文本
                         "    margin-bottom: 15px;"    // 控件底部的间距
                         "}");

    submitButton = new CustomButton("提交", this);

    // 生成四个选项按钮
    optionsGroup = new QButtonGroup(this);
    QRadioButton *optionA = new QRadioButton(this);
    QRadioButton *optionB = new QRadioButton(this);
    QRadioButton *optionC = new QRadioButton(this);
    QRadioButton *optionD = new QRadioButton(this);
    CustomButton *returnToMenuButton = new CustomButton("返回主菜单", this); // 返回主菜单按钮
    QString radioButtonStyle = "QRadioButton {"
                           "    font-size: 20px;"              // 设置字体大小
                           "    color: #333;"                  // 设置文本颜色
                           "    font-weight: bold;"            // 加粗文本
                           "    padding: 8px;"                 // 设置控件内边距，增加视觉效果
                           "    background-color: #f0f0f0;"    // 设置背景颜色
                           "    border: 2px solid #d3d3d3;"    // 设置边框
                           "    border-radius: 5px;"           // 设置圆角边框
                           "    margin: 5px;"                  // 设置控件之间的距离
                           "}"
                           "QRadioButton:hover {"
                           "    background-color: #e8e8e8;"    // 鼠标悬停时改变背景颜色
                           "    border: 2px solid #b0b0b0;"    // 悬停时边框颜色
                           "}"
                           "QRadioButton:checked {"
                           "    font-weight: bold;"            // 选中状态下加粗字体
                           "    background-color: #d6eaf8;"    // 设置选中状态的背景颜色
                           "}";

    optionA->setStyleSheet(radioButtonStyle);
    optionB->setStyleSheet(radioButtonStyle);
    optionC->setStyleSheet(radioButtonStyle);
    optionD->setStyleSheet(radioButtonStyle);


    optionsGroup->addButton(optionA, 0);
    optionsGroup->addButton(optionB, 1);
    optionsGroup->addButton(optionC, 2);
    optionsGroup->addButton(optionD, 3);

    // 添加控件到布局
    learningLayout->addWidget(wordLabel);
    learningLayout->addWidget(optionA);
    learningLayout->addWidget(optionB);
    learningLayout->addWidget(optionC);
    learningLayout->addWidget(optionD);
    learningLayout->addWidget(submitButton);
    learningLayout->addWidget(returnToMenuButton);
    learningLayout->setSpacing(20);                // 设置控件之间的间距为 20 像素
    learningLayout->setContentsMargins(20, 20, 20, 20); // 设置布局的内边距（左、上、右、下）

    learningPage2->setLayout(learningLayout);
    setCentralWidget(learningPage2);

    // 初始化
    currentIndex = 0;  // 当前题目索引
    usedIndexes.clear(); // 清空已出题的索引集合

    // 获取词汇表并随机打乱
    QVector<Word> allWords = vocabulary.getWords();
    std::shuffle(allWords.begin(), allWords.end(), std::mt19937(std::random_device()()));

    // 从打乱后的词汇表中选择前10个单词作为题目
    selectedWords.clear();
    size_t maxQuestions = 10; // 假设题目上限为10

    // 手动计算最小值，确保索引不超过可用词汇数
    size_t numQuestions = (allWords.size() < static_cast<size_t>(maxQuestions)) ? allWords.size() : static_cast<size_t>(maxQuestions);

    for (size_t i = 0; i < numQuestions; ++i) {
        selectedWords.append(allWords[i]);
    }

    // 如果有题目，更新界面显示
    if (!selectedWords.isEmpty()) {
        updateMultipleChoiceQuestion(); // 初始化第一道选择题
    }

    // 连接信号和槽
    connect(returnToMenuButton, &CustomButton::clicked, this, &MainWindow::showMainMenu);
    connect(submitButton, &CustomButton::clicked, this, [this]() {
        checkMultipleChoiceAnswer(optionsGroup); // 检查答案
    });
}

// 更新选择题问题的方法
void MainWindow::updateMultipleChoiceQuestion() {
    if (currentIndex >= selectedWords.size()) {
        QMessageBox::information(this, "完成", "已达到题目上限！", QMessageBox::Ok);
        showMainMenu();
        return;
    }

    const Word &currentWord = selectedWords[currentIndex];
    bool showEnglish = QRandomGenerator::global()->bounded(2) == 0;

    // 设置题干
    if (showEnglish) {
        wordLabel->setText(currentWord.getEnglish() + " (" + currentWord.getPartOfSpeech() + ")");
    } else {
        wordLabel->setText(currentWord.getMeanings().join(", ") + " (" + currentWord.getPartOfSpeech() + ")");
    }

    // 随机生成选项
    QList<QString> options;
    if (showEnglish) {
        options = generateOptions(currentWord.getMeanings().first(), true);  // 生成中文释义的选项
    } else {
        options = generateOptions(currentWord.getEnglish(), false);  // 生成英文翻译的选项
    }

    // 分配选项到按钮
    QRadioButton *optionA = qobject_cast<QRadioButton *>(optionsGroup->button(0));
    QRadioButton *optionB = qobject_cast<QRadioButton *>(optionsGroup->button(1));
    QRadioButton *optionC = qobject_cast<QRadioButton *>(optionsGroup->button(2));
    QRadioButton *optionD = qobject_cast<QRadioButton *>(optionsGroup->button(3));

    // 设置选项文本
    optionA->setText(options[0]);
    optionB->setText(options[1]);
    optionC->setText(options[2]);
    optionD->setText(options[3]);

    qDebug() << "生成的选项：" << options;
    qDebug() << "正确答案：" << options[0]; // 假设正确答案是第一个选项
}

// 生成干扰选项的方法
QList<QString> MainWindow::generateOptions(const QString &correctAnswer, bool isMeaning) {
    QList<QString> options = {correctAnswer};  // 添加正确答案
    int wordsCount = vocabulary.getWords().size();

    while (options.size() < 4) {
        int randIndex = QRandomGenerator::global()->bounded(wordsCount);
        const Word &randomWord = vocabulary.getWords()[randIndex];
        QString option = isMeaning ? randomWord.getMeanings().first() : randomWord.getEnglish();

        if (!options.contains(option)) {
            options.append(option);
        }
    }

    // 打乱选项顺序
    std::shuffle(options.begin(), options.end(), std::default_random_engine(QRandomGenerator::global()->generate()));
    return options;
}

// 检查用户选择的答案
void MainWindow::checkMultipleChoiceAnswer(QButtonGroup *optionsGroup) {
    QAbstractButton *selectedButton = optionsGroup->checkedButton();
    if (!selectedButton) {
        QMessageBox::warning(this, "错误", "请选择一个答案！", QMessageBox::Ok);
        return;
    }

    const Word &currentWord = selectedWords[currentIndex];
    QString correctAnswer = wordLabel->text().startsWith(currentWord.getEnglish())
                                ? currentWord.getMeanings().first()
                                : currentWord.getEnglish();

    qDebug() << "用户选择的答案：" << selectedButton->text();
    qDebug() << "正确答案：" << correctAnswer;

    // 增加单词出现次数
    wordStats[currentWord.getEnglish()].first++;
    learnedWords.append(currentWord);  // 将该单词加入已学习的集合

    // 比较答案
    if (selectedButton->text().trimmed() == correctAnswer.trimmed()) {
        QMessageBox::information(this, "正确", "回答正确！", QMessageBox::Ok);
        totalScore += 10;  // 答对加10分
        currentIndex++;
        if (currentIndex < selectedWords.size()) {
            updateMultipleChoiceQuestion();
        } else {
            QMessageBox::information(this, "完成", "已完成所有单词学习或达到题目上限！", QMessageBox::Ok);
            showMainMenu();
        }
    } else {
        QMessageBox::warning(this, "错误", "回答错误，请再试一次。", QMessageBox::Ok);
        // 增加错误次数
        wordStats[currentWord.getEnglish()].second++;
        totalScore -= 10;  // 答错扣10分
        qDebug() << "当前错题数量:" << wrongWords.size();  // 调试输出
        wrongWords.append(currentWord); // 将错误的单词加入错题本
    }
}

void MainWindow::showReviewPage() {
    review = new Review(wrongWords, this);
    reviewPage = new QWidget(this); // 创建复习页面
    QVBoxLayout *reviewLayout = new QVBoxLayout(reviewPage); // 创建布局

    QLabel *reviewLabel = new QLabel("错题复习", this); // 创建标题标签
    reviewLabel->setStyleSheet("QLabel {"
                         "    font-size: 30px;"        // 设置字体大小
                         "    color: #333;"            // 设置文本颜色
                         "    font-weight: bold;"      // 加粗文本
                         "    margin-bottom: 15px;"    // 控件底部的间距
                         "}"); // 设置样式
    reviewLayout->addWidget(reviewLabel); // 添加标题标签

    if (wrongWords.isEmpty()) { // 如果没有错题
        reviewLabel->setText("没有错题，继续保持！"); // 设置提示
    } else {
        for (const Word &word : wrongWords) { // 遍历错题
            QString meaningsString = word.getMeanings().join(", "); // 使用逗号分隔多个释义
            QLabel *wordItem = new QLabel(word.getEnglish() + " (" + word.getPartOfSpeech() + ") - " + meaningsString, this); // 创建标签
            wordItem->setStyleSheet("QLabel {"
                         "    font-size: 30px;"        // 设置字体大小
                         "    color: #333;"            // 设置文本颜色
                         "    font-weight: bold;"      // 加粗文本
                         "    margin-bottom: 15px;"    // 控件底部的间距
                         "}"); // 设置样式
            reviewLayout->addWidget(wordItem); // 添加标签
        }
    }

    CustomButton *exportButton = new CustomButton("导出错题", this);
    CustomButton *redoButton = new CustomButton("重新做错题", this);
    CustomButton *backButton = new CustomButton("返回主菜单", this); // 返回按钮
    reviewLayout->addWidget(exportButton); // 添加导出按钮
    reviewLayout->addWidget(redoButton); // 添加重新做错题按钮
    reviewLayout->addWidget(backButton); // 添加返回按钮

    reviewPage->setLayout(reviewLayout); // 设置布局
    setCentralWidget(reviewPage); // 显示页面

    connect(backButton, &CustomButton::clicked, this, &MainWindow::showMainMenu); // 返回主菜单
    connect(exportButton, &CustomButton::clicked, this, &MainWindow::onExportWrongWords);
    connect(redoButton, &CustomButton::clicked, this, &MainWindow::onRedoWrongWords);
}


void MainWindow::onExportWrongWords() {
    review -> onExportButtonClicked(); // 导出错题
}

void MainWindow::onRedoWrongWords() {
    review -> onRedoButtonClicked(); // 重新做错题
}

void MainWindow::showStatisticsPage() {
    statisticsPage = new Statistics(vocabulary, wrongWords, learnedWords, wordStats, totalScore, this);
    connect(dynamic_cast<const QtPrivate::FunctionPointer<void(Statistics::*)()>::Object *>(statisticsPage), &Statistics::backToMainMenu, this, &MainWindow::MainMenu);
    // 设置中央窗口小部件为统计页面
    setCentralWidget(statisticsPage);

}

void MainWindow::ImportPage() {
    // 创建导入单词表页面
    QWidget *importPage = new QWidget(this);
    QVBoxLayout *importLayout = new QVBoxLayout(importPage);

    CustomButton *selectFileButton = new CustomButton("选择单词表文件", this);
    QLabel *fileContentLabel = new QLabel("请选择一个文件来导入单词表。", this);
    fileContentLabel->setStyleSheet("QLabel {"
                         "    font-size: 30px;"        // 设置字体大小
                         "    color: #333;"            // 设置文本颜色
                         "    font-weight: bold;"      // 加粗文本
                         "    margin-bottom: 15px;"    // 控件底部的间距
                         "}"); // 设置样式
    CustomButton *backButton = new CustomButton("返回主菜单", this);

    importLayout->addWidget(selectFileButton);
    importLayout->addWidget(fileContentLabel);
    importLayout->addWidget(backButton);

    importPage->setLayout(importLayout);
    setCentralWidget(importPage); // 设置导入页面为当前中心窗口

    connect(selectFileButton, &CustomButton::clicked, this, [this, fileContentLabel]() {
        QString fileName = QFileDialog::getOpenFileName(this, "选择单词表文件", "", "文本文件 (*.txt)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                in.setCodec("UTF-8"); // 确保使用 UTF-8 编码读取文件
                QString content;
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
                        vocabulary.addWord(Word(english, partOfSpeech, meanings));
                        content += line + "\n";
                    } else {
                        content += "无效的单词格式: " + line + "\n";
                    }
                }
                fileContentLabel->setText(content);
                file.close();
            } else {
                QMessageBox::warning(this, "文件错误", "无法打开所选文件！");
            }
        }
    });

    connect(backButton, &CustomButton::clicked, this, &MainWindow::showMainMenu); // 返回主菜单
}

void MainWindow::showCardPage() {
    try {
        if (CardPage == nullptr) {
            CardPage = new QWidget(this);
            QVBoxLayout *cardLayout = new QVBoxLayout(CardPage);
            infoLabel = new QLabel(this);
            infoLabel->setStyleSheet("QLabel {"
                         "    font-size: 30px;"        // 设置字体大小
                         "    color: #333;"            // 设置文本颜色
                         "    font-weight: bold;"      // 加粗文本
                         "    margin-bottom: 15px;"    // 控件底部的间距
                         "}"); // 设置样式
            CustomButton *checkInButton = new CustomButton("打卡", this);
            CustomButton *returnToMenuButton = new CustomButton("返回主菜单", this); // 返回主菜单按钮

            // 将控件添加到布局中
            cardLayout->addWidget(infoLabel);
            cardLayout->addWidget(checkInButton);
            cardLayout->addWidget(returnToMenuButton); // 添加返回主菜单按钮

            // 设置 CardPage 的布局
            CardPage->setLayout(cardLayout);

            // 连接按钮的点击信号到槽函数
            connect(checkInButton, &CustomButton::clicked, this, &MainWindow::onCheckInButtonClicked);
            connect(returnToMenuButton, &CustomButton::clicked, this, &MainWindow::onReturnToMainMenuClicked);
        }

        // 加载打卡数据到recentCheckins列表
        loadCheckinData();
        // 更新显示的打卡信息
        updateCheckinDisplay();

        // 将 CardPage 设置为主窗口的中央部件
        setCentralWidget(CardPage);
    } catch (const std::bad_alloc& e) {
        QMessageBox::critical(this, "内存分配错误", "无法分配足够的内存来创建界面！");
    }
}

void MainWindow::onCheckInButtonClicked() {
    QDate currentDate = QDate::currentDate();

    // 判断是否已打卡
    if (lastCheckInDate == currentDate) {
        QMessageBox::information(this, "提示", "今天已经打过卡了！");
        return;
    }

    // 更新打卡数据
    lastCheckInDate = currentDate;
    checkinCount++;
    saveCheckinData(currentDate);
    updateCheckinDisplay();

    QMessageBox::information(this, "打卡成功", "打卡成功！");
}

void MainWindow::onReturnToMainMenuClicked() {
    // 处理返回主菜单的逻辑
    showMainMenu();
}

void MainWindow::loadCheckinData() {
    QString filePath = getUserFilePath();

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return; // 如果文件无法打开，直接返回
    }

    QTextStream in(&file);

    bool ok;
    QString checkinCountStr = in.readLine(); // 读取打卡次数
    checkinCount = checkinCountStr.toInt(&ok);
    if (!ok) {
        checkinCount = 0; // 如果读取失败，设置为 0
    }

    QString lastCheckInDateStr = in.readLine(); // 读取上次打卡日期
    lastCheckInDate = QDate::fromString(lastCheckInDateStr, Qt::ISODate);
    if (!lastCheckInDate.isValid()) {
        lastCheckInDate = QDate(); // 如果日期无效，重置为空
    }

    // 清空现有的打卡记录
    recentCheckins.clear();

    // 读取历史打卡记录
    while (!in.atEnd()) {
        QString dateString = in.readLine();
        QDate checkinDate = QDate::fromString(dateString, Qt::ISODate);
        if (checkinDate.isValid()) {
            recentCheckins.append(checkinDate);
        }
    }

    file.close();
}
void MainWindow::saveCheckinData(const QDate &date)
{
    QString filePath = getUserFilePath();

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "无法保存打卡数据！");
        return;
    }

    QTextStream out(&file);
    out << checkinCount << "\n";
    out << lastCheckInDate.toString(Qt::ISODate) << "\n";
    out << date.toString(Qt::ISODate) << "\n";

    for (const QDate &checkin : recentCheckins) {
        out << checkin.toString(Qt::ISODate) << "\n";
    }
    file.close();
}

QString MainWindow::getUserFilePath() const// 获取用户文件路径
{
    QString dataDir = QCoreApplication::applicationDirPath() + "/checkin_data/";
    QDir dir(dataDir);// 创建目录对象
    if (!dir.exists())
        dir.mkpath(dataDir);// 创建数据目录
    return dataDir + username + ".txt";
}

void MainWindow::updateCheckinDisplay(){
    QString displayText = QString("当前打卡天数：%1\n上次打卡日期：%2") // 显示打卡信息
                              .arg(checkinCount)
                              .arg(lastCheckInDate.isValid() ? lastCheckInDate.toString(Qt::ISODate) : "无");

    infoLabel->setText(displayText);
}
//打卡

