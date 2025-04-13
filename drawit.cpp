#include "drawit.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QTranslator>
#include <QApplication>
#include <QListWidget>
#include <QFile>
#include <QStatusBar>

static QTranslator *translator = nullptr;

// Реализация MainWindow
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Центральный виджет
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setStyleSheet("background-color: #FFC107;");


    profileLabel = new QLabel(centralWidget);
    profileLabel->setStyleSheet("font-size: 16px; font-weight: bold;");

    newsLabel = new QLabel(centralWidget);
    newsLabel->setStyleSheet("font-size: 16px; font-weight: bold;");

    // Инициализация imageLabel
    imageLabel = new QLabel(centralWidget);
    imageLabel->setAlignment(Qt::AlignCenter);
    QString imagePath = "/Users/fuad/qt_projects/drawit/build/Qt_5_16_0-Debug/beach.png";
    if (QFile::exists(imagePath)) {
        QPixmap pixmap(imagePath);
        imageLabel->setPixmap(pixmap.scaled(700, 500, Qt::KeepAspectRatio));
        imageLabel->setScaledContents(false);
    } else {
        imageLabel->setText("Image not found");
        imageLabel->setStyleSheet("font-size: 20px; color: red;");
    }


    playersLabel = new QLabel(centralWidget);
    playersLabel->setStyleSheet("font-size: 14px;");

    versionLabel = new QLabel("v0.0.0.1", centralWidget);
    versionLabel->setStyleSheet("font-size: 12px;");

    friendsLabel = new QLabel(centralWidget);
    friendsLabel->setStyleSheet("font-size: 16px; font-weight: bold;");

    friendsList = new QListWidget(centralWidget);
    friendsList->setStyleSheet("background-color: white; border: 1px solid black;");

    addFriendButton = new QPushButton(centralWidget);
    addFriendButton->setStyleSheet("background-color: green; color: white; border-radius: 5px; padding: 5px;");

    removeFriendButton = new QPushButton(centralWidget);
    removeFriendButton->setStyleSheet("background-color: red; color: white; border-radius: 5px; padding: 5px;");

    joinLobbyButton = new QPushButton(centralWidget);
    joinLobbyButton->setStyleSheet("background-color: #2196F3; color: white; border-radius: 5px; padding: 5px;");

    playButton = new QPushButton(centralWidget);
    playButton->setStyleSheet("background-color: #2196F3; color: white; border-radius: 10px; padding: 10px; font-size: 16px;");
    playButton->setMinimumWidth(150);
    playButton->setMinimumHeight(40);

    languageCombo = new QComboBox(centralWidget);
    languageCombo->addItems({"English", "Русский"});
    languageCombo->setFixedWidth(150);

    // Профиль
    QWidget *profileWidget = new QWidget(centralWidget);
    profileWidget->setStyleSheet("background-color: white; border: 1px solid black;");
    QHBoxLayout *profileLayout = new QHBoxLayout(profileWidget);
    QLabel *avatar = new QLabel("👨‍🦰", centralWidget);
    avatar->setStyleSheet("font-size: 40px;");
    profileLayout->addWidget(avatar);
    QLabel *username = new QLabel("AW3some", centralWidget);
    username->setStyleSheet("font-size: 16px;");
    profileLayout->addWidget(username);

    // Популярные рисунки
    QWidget *popularWidget = new QWidget(centralWidget);
    popularWidget->setStyleSheet("background-color: white; border: 1px solid black;");
    QHBoxLayout *popularLayout = new QHBoxLayout(popularWidget);
    QLabel *popularImage = new QLabel(centralWidget);
    popularImage->setPixmap(QPixmap(":/popular.png").scaled(100, 100, Qt::KeepAspectRatio));
    popularLayout->addWidget(popularImage);
    QLabel *popularText = new QLabel("Albedo\nAfro", centralWidget);
    popularText->setStyleSheet("font-size: 14px;");
    popularLayout->addWidget(popularText);

    // Сборка макетов
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Левый макет
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(profileLabel);
    leftLayout->addWidget(profileWidget);
    leftLayout->addWidget(newsLabel);
    leftLayout->addWidget(imageLabel, 1);
    leftLayout->addWidget(popularWidget);
    leftLayout->addWidget(playersLabel);
    leftLayout->addWidget(versionLabel);
    mainLayout->addLayout(leftLayout, 3);

    // Правый макет (друзья)
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(friendsLabel);
    friendsList->addItem("Z01Ro - He в сети");
    friendsList->addItem("Obos - He в сети");
    rightLayout->addWidget(friendsList, 1);

    QHBoxLayout *friendButtonsLayout = new QHBoxLayout();
    friendButtonsLayout->addWidget(addFriendButton);
    friendButtonsLayout->addWidget(removeFriendButton);
    rightLayout->addLayout(friendButtonsLayout);
    rightLayout->addWidget(joinLobbyButton);
    mainLayout->addLayout(rightLayout, 1);

    // Кнопка "Играть"
    QVBoxLayout *outerLayout = new QVBoxLayout();
    outerLayout->addLayout(mainLayout, 1);
    outerLayout->addWidget(playButton, 0, Qt::AlignCenter);

    // Выбор языка
    mainLayout->addWidget(languageCombo, 0, Qt::AlignTop | Qt::AlignRight);

    // Подключение сигналов
    connect(playButton, &QPushButton::clicked, this, &MainWindow::onPlayClicked);
    connect(addFriendButton, &QPushButton::clicked, this, &MainWindow::onAddFriendClicked);
    connect(removeFriendButton, &QPushButton::clicked, this, &MainWindow::onRemoveFriendClicked);
    connect(joinLobbyButton, &QPushButton::clicked, this, &MainWindow::onJoinLobbyClicked);
    connect(languageCombo, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::onLanguageChanged);

    // Инициализация переводчика
    translator = new QTranslator(this);
    retranslateUi();

    // Полноэкранный режим
    setWindowState(Qt::WindowMaximized);
}

void MainWindow::retranslateUi() {
    setWindowTitle(tr("Draw It"));
    profileLabel->setText(tr("Your Profile"));
    newsLabel->setText(tr("News"));
    friendsLabel->setText(tr("Friends"));
    playButton->setText(tr("Play"));
    addFriendButton->setText(tr("Add Friend"));
    removeFriendButton->setText(tr("Remove"));
    joinLobbyButton->setText(tr("Join Lobby"));
}

void MainWindow::onPlayClicked() {
    statusBar()->showMessage(tr("Play clicked!"));
}

void MainWindow::onAddFriendClicked() {
    friendsList->addItem("New Friend - Offline");
}

void MainWindow::onRemoveFriendClicked() {
    if (friendsList->currentRow() >= 0) {
        delete friendsList->takeItem(friendsList->currentRow());
    }
}

void MainWindow::onJoinLobbyClicked() {
    statusBar()->showMessage(tr("Join Lobby clicked!"));
}

void MainWindow::onLanguageChanged(int index) {
    QString lang = index == 0 ? "en" : "ru";
    translator->load("drawit_" + lang, "/Users/fuad/qt_projects/drawit/");
    qApp->installTranslator(translator);
    retranslateUi();
}
