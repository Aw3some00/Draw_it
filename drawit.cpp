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
#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>
#include <QFontDatabase>
#include <QInputDialog>

static QTranslator *translator = nullptr;

DrawingArea::DrawingArea(QWidget *parent) : QWidget(parent), drawing(false), currentBrushSize(2), currentBrushColor(Qt::black) {
    setFixedSize(1000, 600);
    setStyleSheet("background-color: white; border: 2px solid #4A90E2; border-radius: 10px;");
}

void DrawingArea::addPoint(const QPoint &point) {
    points.append(point);
    brushSizes.append(currentBrushSize);
    brushColors.append(currentBrushColor);
    update();
}

void DrawingArea::clear() {
    points.clear();
    brushSizes.clear();
    brushColors.clear();
    update();
}

void DrawingArea::setBrushSize(int size) {
    currentBrushSize = size;
}

void DrawingArea::setBrushColor(const QColor &color) {
    currentBrushColor = color;
}

void DrawingArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    for (int i = 1; i < points.size(); ++i) {
        painter.setPen(QPen(brushColors[i], brushSizes[i], Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(points[i-1], points[i]);
    }
}

void DrawingArea::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drawing = true;
        points.append(event->pos());
        brushSizes.append(currentBrushSize);
        brushColors.append(currentBrushColor);
        emit pointDrawn(event->pos(), currentBrushSize, currentBrushColor);
        update();
    }
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event) {
    if (drawing) {
        points.append(event->pos());
        brushSizes.append(currentBrushSize);
        brushColors.append(currentBrushColor);
        emit pointDrawn(event->pos(), currentBrushSize, currentBrushColor);
        update();
    }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drawing = false;
    }
}

RoomSettingsDialog::RoomSettingsDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *maxPlayersLabel = new QLabel("Max Players (1-6):", this);
    maxPlayersLabel->setStyleSheet("font-family: 'Roboto'; font-size: 14px; color: #333;");
    layout->addWidget(maxPlayersLabel);

    maxPlayersSpinBox = new QSpinBox(this);
    maxPlayersSpinBox->setRange(1, 6);
    maxPlayersSpinBox->setValue(2);
    maxPlayersSpinBox->setStyleSheet("font-family: 'Roboto'; font-size: 14px; padding: 5px; border: 1px solid #ccc; border-radius: 5px;");
    layout->addWidget(maxPlayersSpinBox);

    confirmButton = new QPushButton("Confirm", this);
    confirmButton->setStyleSheet("background-color: #4A90E2; color: white; font-family: 'Roboto'; font-size: 14px; border-radius: 5px; padding: 8px;");
    layout->addWidget(confirmButton);

    connect(confirmButton, &QPushButton::clicked, this, &RoomSettingsDialog::onConfirmClicked);

    setStyleSheet("background-color: #F5F7FA; border: 1px solid #ccc; border-radius: 10px; padding: 10px;");
}

int RoomSettingsDialog::getMaxPlayers() const {
    return maxPlayersSpinBox->value();
}

void RoomSettingsDialog::onConfirmClicked() {
    emit settingsConfirmed(maxPlayersSpinBox->value());
    accept();
}

ChatWidget::ChatWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);
    chatDisplay->setStyleSheet("background-color: #F5F7FA; border: 1px solid #ccc; border-radius: 10px; padding: 10px; font-family: 'Roboto'; font-size: 14px;");
    layout->addWidget(chatDisplay);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    chatInput = new QLineEdit(this);
    chatInput->setStyleSheet(
        "background-color: white; "
        "color: #000000; "
        "border: 1px solid #ccc; "
        "border-radius: 5px; "
        "padding: 8px; "
        "font-family: 'Roboto'; "
        "font-size: 16px;"
        );
    inputLayout->addWidget(chatInput);

    sendButton = new QPushButton("Send", this);
    sendButton->setStyleSheet("background-color: #4A90E2; color: white; font-family: 'Roboto'; font-size: 14px; border-radius: 5px; padding: 8px;");
    inputLayout->addWidget(sendButton);
    layout->addLayout(inputLayout);

    connect(sendButton, &QPushButton::clicked, this, &ChatWidget::onSendMessage);
}

void ChatWidget::appendMessage(const QString &message) {
    chatDisplay->append("<div style='background-color: #E1F5FE; border-radius: 10px; padding: 8px; margin: 5px;'>" + message + "</div>");
}

void ChatWidget::onSendMessage() {
    QString message = chatInput->text();
    if (!message.isEmpty()) {
        QString formattedMessage = "Player: " + message;
        appendMessage(formattedMessage);
        emit messageSent(formattedMessage);
        chatInput->clear();
    }
}

JoinLobbyDialog::JoinLobbyDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("Enter Server IP:", this);
    label->setStyleSheet("font-family: 'Roboto'; font-size: 14px; color: #333;");
    layout->addWidget(label);

    serverIpInput = new QLineEdit(this);
    serverIpInput->setPlaceholderText("e.g., 192.168.1.100");
    serverIpInput->setStyleSheet(
        "background-color: white; "
        "color: #000000; "
        "border: 1px solid #ccc; "
        "border-radius: 5px; "
        "padding: 8px; "
        "font-family: 'Roboto'; "
        "font-size: 16px;"
        );
    layout->addWidget(serverIpInput);

    joinButton = new QPushButton("Join", this);
    joinButton->setStyleSheet("background-color: #4A90E2; color: white; font-family: 'Roboto'; font-size: 14px; border-radius: 5px; padding: 8px;");
    layout->addWidget(joinButton);

    connect(joinButton, &QPushButton::clicked, this, &JoinLobbyDialog::onJoinClicked);

    setStyleSheet("background-color: #F5F7FA; border: 1px solid #ccc; border-radius: 10px; padding: 10px;");
}

QString JoinLobbyDialog::getServerIp() const {
    return serverIpInput->text();
}

void JoinLobbyDialog::onJoinClicked() {
    emit joinRequested(serverIpInput->text());
    accept();
}

PlayModeWindow::PlayModeWindow(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("Select Mode", this);
    label->setStyleSheet("font-family: 'Roboto'; font-size: 16px; font-weight: bold; color: #333;");
    layout->addWidget(label, 0, Qt::AlignCenter);

    createRoomButton = new QPushButton("Create Room", this);
    createRoomButton->setStyleSheet("background-color: #4A90E2; color: white; font-family: 'Roboto'; font-size: 14px; border-radius: 5px; padding: 8px;");
    layout->addWidget(createRoomButton);

    joinRoomButton = new QPushButton("Join Room", this);
    joinRoomButton->setStyleSheet("background-color: #4A90E2; color: white; font-family: 'Roboto'; font-size: 14px; border-radius: 5px; padding: 8px;");
    layout->addWidget(joinRoomButton);

    connect(createRoomButton, &QPushButton::clicked, this, &PlayModeWindow::onCreateRoomClicked);
    connect(joinRoomButton, &QPushButton::clicked, this, &PlayModeWindow::onJoinRoomClicked);

    setStyleSheet("background-color: #F5F7FA; border: 1px solid #ccc; border-radius: 10px; padding: 10px;");
}

void PlayModeWindow::onCreateRoomClicked() {
    emit createRoomRequested();
    accept();
}

void PlayModeWindow::onJoinRoomClicked() {
    emit joinRoomRequested();
    accept();
}

GameWindow::GameWindow(QWidget *parent, bool isServer, const QString &serverIp) : QDialog(parent), server(nullptr), maxPlayers(2) {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    drawingArea = new DrawingArea(this);
    leftLayout->addWidget(drawingArea);

    QHBoxLayout *toolsLayout = new QHBoxLayout();
    brushSizeCombo = new QComboBox(this);
    brushSizeCombo->addItems({"Small (2px)", "Medium (5px)", "Large (10px)"});
    brushSizeCombo->setStyleSheet("font-family: 'Roboto'; font-size: 14px; padding: 5px; border: 1px solid #ccc; border-radius: 5px;");
    toolsLayout->addWidget(brushSizeCombo);

    brushColorButton = new QPushButton("Pick Color", this);
    brushColorButton->setStyleSheet("background-color: #4A90E2; color: white; font-family: 'Roboto'; font-size: 14px; border-radius: 5px; padding: 8px;");
    toolsLayout->addWidget(brushColorButton);
    leftLayout->addLayout(toolsLayout);
    mainLayout->addLayout(leftLayout, 3);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    playerList = new QListWidget(this);
    playerList->setStyleSheet("background-color: white; border: 1px solid #ccc; border-radius: 10px; padding: 5px; font-family: 'Roboto'; font-size: 14px;");
    playerList->addItem("Player 1 (You)");
    rightLayout->addWidget(playerList, 1);

    chatWidget = new ChatWidget(this);
    rightLayout->addWidget(chatWidget, 2);
    mainLayout->addLayout(rightLayout, 1);

    connect(drawingArea, &DrawingArea::pointDrawn, this, &GameWindow::onPointDrawn);
    connect(chatWidget, &ChatWidget::messageSent, this, &GameWindow::onSendMessage);
    connect(brushSizeCombo, QOverload<int>::of(&QComboBox::activated), this, &GameWindow::onBrushSizeChanged);
    connect(brushColorButton, &QPushButton::clicked, this, &GameWindow::onBrushColorChanged);

    if (isServer) {
        setupServer();
    } else {
        QTcpSocket *socket = new QTcpSocket(this);
        socket->connectToHost(serverIp, 12345);
        if (socket->waitForConnected()) {
            clients.append(socket);
            connect(socket, &QTcpSocket::readyRead, this, &GameWindow::readClientData);
            connect(socket, &QTcpSocket::disconnected, this, &GameWindow::clientDisconnected);
            chatWidget->appendMessage("Connected to server at " + serverIp);
        } else {
            chatWidget->appendMessage("Could not connect to server at " + serverIp);
        }
    }

    setWindowTitle("Draw It - Game");
    setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #A1C4FD, stop:1 #C2E9FB);");
    resize(1200, 800);
}

GameWindow::~GameWindow() {
    for (QTcpSocket *client : clients) {
        client->disconnectFromHost();
    }
    if (server) {
        server->close();
    }
}

void GameWindow::setMaxPlayers(int max) {
    maxPlayers = max;
}

void GameWindow::setupServer() {
    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::Any, 12345)) {
        chatWidget->appendMessage("Server could not start!");
        return;
    }
    connect(server, &QTcpServer::newConnection, this, &GameWindow::handleNewConnection);
    chatWidget->appendMessage("Server started on port 12345");
    chatWidget->appendMessage("Your IP: " + server->serverAddress().toString());
}

void GameWindow::handleNewConnection() {
    if (clients.size() >= maxPlayers) {
        QTcpSocket *client = server->nextPendingConnection();
        client->disconnectFromHost();
        return;
    }
    QTcpSocket *client = server->nextPendingConnection();
    clients.append(client);
    connect(client, &QTcpSocket::readyRead, this, &GameWindow::readClientData);
    connect(client, &QTcpSocket::disconnected, this, &GameWindow::clientDisconnected);
    chatWidget->appendMessage("New player connected");
    playerList->addItem("Player " + QString::number(clients.size() + 1));
}

void GameWindow::readClientData() {
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    QDataStream in(client);
    in.setVersion(QDataStream::Qt_5_15);
    QString message;
    QPoint point;
    int brushSize;
    QColor brushColor;

    while (!in.atEnd()) {
        QString type;
        in >> type;
        if (type == "message") {
            in >> message;
            chatWidget->appendMessage(message);
            broadcastMessage(message);
        } else if (type == "point") {
            in >> point >> brushSize >> brushColor;
            drawingArea->addPoint(point);
            broadcastPoint(point, brushSize, brushColor);
        }
    }
}

void GameWindow::clientDisconnected() {
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;
    clients.removeAll(client);
    chatWidget->appendMessage("Player disconnected");
    playerList->clear();
    playerList->addItem("Player 1 (You)");
    for (int i = 0; i < clients.size(); ++i) {
        playerList->addItem("Player " + QString::number(i + 2));
    }
}

void GameWindow::broadcastMessage(const QString &message) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << QString("message") << message;
    for (QTcpSocket *client : clients) {
        client->write(block);
    }
}

void GameWindow::broadcastPoint(const QPoint &point, int brushSize, const QColor &brushColor) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << QString("point") << point << brushSize << brushColor;
    for (QTcpSocket *client : clients) {
        client->write(block);
    }
}

void GameWindow::onPointDrawn(const QPoint &point, int brushSize, const QColor &brushColor) {
    broadcastPoint(point, brushSize, brushColor);
}

void GameWindow::onSendMessage(const QString &message) {
    broadcastMessage(message);
}

void GameWindow::onBrushSizeChanged(int index) {
    int size = 2;
    if (index == 1) size = 5;
    else if (index == 2) size = 10;
    drawingArea->setBrushSize(size);
}

void GameWindow::onBrushColorChanged() {
    QColor color = QColorDialog::getColor(Qt::black, this, "Select Brush Color");
    if (color.isValid()) {
        drawingArea->setBrushColor(color);
        brushColorButton->setStyleSheet(QString("background-color: %1; color: white; font-family: 'Roboto'; font-size: 14px; border-radius: 5px; padding: 8px;").arg(color.name()));
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QFontDatabase fontDatabase;
    fontDatabase.addApplicationFont(":/fonts/Roboto-Regular.ttf");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #A1C4FD, stop:1 #C2E9FB);");

    profileLabel = new QLabel(centralWidget);
    profileLabel->setStyleSheet("font-family: 'Roboto'; font-size: 16px; font-weight: bold; color: #333;");

    newsLabel = new QLabel(centralWidget);
    newsLabel->setStyleSheet("font-family: 'Roboto'; font-size: 16px; font-weight: bold; color: #333;");

    imageLabel = new QLabel(centralWidget);
    imageLabel->setAlignment(Qt::AlignCenter);
    QPixmap pixmap(":/beach");
    if (!pixmap.isNull()) {
        imageLabel->setPixmap(pixmap.scaled(700, 500, Qt::KeepAspectRatio));
        imageLabel->setScaledContents(false);
    } else {
        imageLabel->setText("Image not found");
        imageLabel->setStyleSheet("font-family: 'Roboto'; font-size: 20px; color: red;");
    }

    playersLabel = new QLabel("Players online: 0", centralWidget);
    playersLabel->setStyleSheet("font-family: 'Roboto'; font-size: 14px; color: #333;");

    versionLabel = new QLabel("v0.0.0.1", centralWidget);
    versionLabel->setStyleSheet("font-family: 'Roboto'; font-size: 12px; color: #333;");

    friendsLabel = new QLabel(centralWidget);
    friendsLabel->setStyleSheet("font-family: 'Roboto'; font-size: 16px; font-weight: bold; color: #333;");

    friendsList = new QListWidget(centralWidget);
    friendsList->setStyleSheet("background-color: white; border: 1px solid #ccc; border-radius: 10px; padding: 5px; font-family: 'Roboto'; font-size: 14px;");

    addFriendButton = new QPushButton(centralWidget);
    addFriendButton->setStyleSheet("background-color: #2ECC71; color: white; font-family: 'Roboto'; font-size: 14px; border-radius: 5px; padding: 8px;");

    removeFriendButton = new QPushButton(centralWidget);
    removeFriendButton->setStyleSheet("background-color: #E74C3C; color: white; font-family: 'Roboto'; font-size: 14px; border-radius: 5px; padding: 8px;");

    joinLobbyButton = new QPushButton(centralWidget);
    joinLobbyButton->setStyleSheet("background-color: #4A90E2; color: white; font-family: 'Roboto'; font-size: 14px; border-radius: 5px; padding: 8px;");

    playButton = new QPushButton(centralWidget);
    playButton->setStyleSheet("background-color: #4A90E2; color: white; font-family: 'Roboto'; font-size: 16px; border-radius: 10px; padding: 10px;");
    playButton->setMinimumWidth(150);
    playButton->setMinimumHeight(40);

    languageCombo = new QComboBox(centralWidget);
    languageCombo->addItems({"English", "Русский"});
    languageCombo->setFixedWidth(150);
    languageCombo->setStyleSheet("font-family: 'Roboto'; font-size: 14px; padding: 5px; border: 1px solid #ccc; border-radius: 5px;");

    QWidget *profileWidget = new QWidget(centralWidget);
    profileWidget->setStyleSheet("background-color: white; border: 1px solid #ccc; border-radius: 10px; padding: 5px;");
    QHBoxLayout *profileLayout = new QHBoxLayout(profileWidget);
    QLabel *avatar = new QLabel("👨‍🦰", centralWidget);
    avatar->setStyleSheet("font-size: 40px;");
    profileLayout->addWidget(avatar);
    QLabel *username = new QLabel("AW3some", centralWidget);
    username->setStyleSheet("font-family: 'Roboto'; font-size: 16px; color: #333;");
    profileLayout->addWidget(username);

    QWidget *popularWidget = new QWidget(centralWidget);
    popularWidget->setStyleSheet("background-color: white; border: 1px solid #ccc; border-radius: 10px; padding: 5px;");
    QHBoxLayout *popularLayout = new QHBoxLayout(popularWidget);
    QLabel *popularImage = new QLabel(centralWidget);
    popularImage->setPixmap(QPixmap(":/popular").scaled(100, 100, Qt::KeepAspectRatio));
    popularLayout->addWidget(popularImage);
    QLabel *popularText = new QLabel("Albedo\nAfro", centralWidget);
    popularText->setStyleSheet("font-family: 'Roboto'; font-size: 14px; color: #333;");
    popularLayout->addWidget(popularText);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(profileLabel);
    leftLayout->addWidget(profileWidget);
    leftLayout->addWidget(newsLabel);
    leftLayout->addWidget(imageLabel, 1);
    leftLayout->addWidget(popularWidget);
    leftLayout->addWidget(playersLabel);
    leftLayout->addWidget(versionLabel);
    mainLayout->addLayout(leftLayout, 3);

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

    QVBoxLayout *outerLayout = new QVBoxLayout();
    outerLayout->addLayout(mainLayout, 1);
    outerLayout->addWidget(playButton, 0, Qt::AlignCenter);

    mainLayout->addWidget(languageCombo, 0, Qt::AlignTop | Qt::AlignRight);

    connect(playButton, &QPushButton::clicked, this, &MainWindow::onPlayClicked);
    connect(addFriendButton, &QPushButton::clicked, this, &MainWindow::onAddFriendClicked);
    connect(removeFriendButton, &QPushButton::clicked, this, &MainWindow::onRemoveFriendClicked);
    connect(joinLobbyButton, &QPushButton::clicked, this, &MainWindow::onJoinLobbyClicked);
    connect(languageCombo, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::onLanguageChanged);

    translator = new QTranslator(this);
    retranslateUi();

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
    PlayModeWindow *playModeWindow = new PlayModeWindow(this);
    connect(playModeWindow, &PlayModeWindow::createRoomRequested, this, &MainWindow::onCreateRoomRequested);
    connect(playModeWindow, &PlayModeWindow::joinRoomRequested, this, &MainWindow::onJoinRoomRequested);
    playModeWindow->exec();
}

void MainWindow::onAddFriendClicked() {
    bool ok;
    QString friendName = QInputDialog::getText(this, tr("Add Friend"), tr("Enter friend's name:"), QLineEdit::Normal, "", &ok);
    if (ok && !friendName.isEmpty()) {
        friendsList->addItem(friendName + " - Offline");
    }
}

void MainWindow::onRemoveFriendClicked() {
    if (friendsList->currentRow() >= 0) {
        delete friendsList->takeItem(friendsList->currentRow());
    }
}

void MainWindow::onJoinLobbyClicked() {
    JoinLobbyDialog *joinDialog = new JoinLobbyDialog(this);
    connect(joinDialog, &JoinLobbyDialog::joinRequested, this, &MainWindow::onJoinLobbyRequested);
    joinDialog->exec();
}

void MainWindow::onLanguageChanged(int index) {
    QString lang = index == 0 ? "en" : "ru";
    translator->load("drawit_" + lang, "/Users/fuad/qt_projects/drawit/");
    qApp->installTranslator(translator);
    retranslateUi();
}

void MainWindow::onCreateRoomRequested() {
    RoomSettingsDialog *settingsDialog = new RoomSettingsDialog(this);
    connect(settingsDialog, &RoomSettingsDialog::settingsConfirmed, this, &MainWindow::onRoomSettingsConfirmed);
    settingsDialog->exec();
}

void MainWindow::onJoinRoomRequested() {
    JoinLobbyDialog *joinDialog = new JoinLobbyDialog(this);
    connect(joinDialog, &JoinLobbyDialog::joinRequested, this, &MainWindow::onJoinLobbyRequested);
    joinDialog->exec();
}

void MainWindow::onRoomSettingsConfirmed(int maxPlayers) {
    GameWindow *gameWindow = new GameWindow(this, true);
    gameWindow->setMaxPlayers(maxPlayers);
    gameWindow->exec();
}

void MainWindow::onJoinLobbyRequested(const QString &ip) {
    GameWindow *gameWindow = new GameWindow(this, false, ip);
    gameWindow->exec();
}
