#ifndef DRAWIT_H
#define DRAWIT_H

#include <QMainWindow>
#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QPoint>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QColor>
#include <QDialog>
#include <QTextEdit>

class QPushButton;
class QLabel;
class QComboBox;
class QListWidget;
class QTextEdit;

class DrawingArea : public QWidget {
    Q_OBJECT
public:
    explicit DrawingArea(QWidget *parent = nullptr);
    void addPoint(const QPoint &point);
    void clear();
    void setBrushSize(int size);
    void setBrushColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void pointDrawn(const QPoint &point, int brushSize, const QColor &brushColor);

private:
    QVector<QPoint> points;
    QVector<int> brushSizes;
    QVector<QColor> brushColors;
    bool drawing;
    int currentBrushSize;
    QColor currentBrushColor;
};

class RoomSettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit RoomSettingsDialog(QWidget *parent = nullptr);
    int getMaxPlayers() const;

signals:
    void settingsConfirmed(int maxPlayers);

private slots:
    void onConfirmClicked();

private:
    QSpinBox *maxPlayersSpinBox;
    QPushButton *confirmButton;
};

class ChatWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChatWidget(QWidget *parent = nullptr);
    void appendMessage(const QString &message);

signals:
    void messageSent(const QString &message);

private slots:
    void onSendMessage();

private:
    QTextEdit *chatDisplay;
    QLineEdit *chatInput;
    QPushButton *sendButton;
};

class JoinLobbyDialog : public QDialog {
    Q_OBJECT
public:
    explicit JoinLobbyDialog(QWidget *parent = nullptr);
    QString getServerIp() const;

signals:
    void joinRequested(const QString &ip);

private slots:
    void onJoinClicked();

private:
    QLineEdit *serverIpInput;
    QPushButton *joinButton;
};

class PlayModeWindow : public QDialog {
    Q_OBJECT
public:
    explicit PlayModeWindow(QWidget *parent = nullptr);

signals:
    void createRoomRequested();
    void joinRoomRequested();

private slots:
    void onCreateRoomClicked();
    void onJoinRoomClicked();

private:
    QPushButton *createRoomButton;
    QPushButton *joinRoomButton;
};

class GameWindow : public QDialog {
    Q_OBJECT
public:
    explicit GameWindow(QWidget *parent = nullptr, bool isServer = false, const QString &serverIp = "");
    ~GameWindow();
    void setMaxPlayers(int max);

private slots:
    void onPointDrawn(const QPoint &point, int brushSize, const QColor &brushColor);
    void handleNewConnection();
    void readClientData();
    void clientDisconnected();
    void onSendMessage(const QString &message);
    void onBrushSizeChanged(int index);
    void onBrushColorChanged();

private:
    void setupServer();
    void broadcastMessage(const QString &message);
    void broadcastPoint(const QPoint &point, int brushSize, const QColor &brushColor);

    QTcpServer *server;
    QVector<QTcpSocket*> clients;
    int maxPlayers;

    DrawingArea *drawingArea;
    ChatWidget *chatWidget;
    QListWidget *playerList;
    QComboBox *brushSizeCombo;
    QPushButton *brushColorButton;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onPlayClicked();
    void onAddFriendClicked();
    void onRemoveFriendClicked();
    void onJoinLobbyClicked();
    void onLanguageChanged(int index);
    void onCreateRoomRequested();
    void onJoinRoomRequested();
    void onRoomSettingsConfirmed(int maxPlayers);
    void onJoinLobbyRequested(const QString &ip);

private:
    void retranslateUi();

    QLabel *profileLabel;
    QLabel *newsLabel;
    QLabel *imageLabel;
    QLabel *playersLabel;
    QLabel *versionLabel;
    QLabel *friendsLabel;
    QListWidget *friendsList;
    QPushButton *addFriendButton;
    QPushButton *removeFriendButton;
    QPushButton *joinLobbyButton;
    QPushButton *playButton;
    QComboBox *languageCombo;
};

#endif
