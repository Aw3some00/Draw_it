#ifndef DRAWIT_H
#define DRAWIT_H

#include <QMainWindow>

class QPushButton;
class QLabel;
class QComboBox;
class QListWidget;

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

private:
    void retranslateUi();
    QLabel *imageLabel; // Для фоновой картинки
    QPushButton *playButton;
    QPushButton *addFriendButton;
    QPushButton *removeFriendButton;
    QPushButton *joinLobbyButton;
    QComboBox *languageCombo;
    QListWidget *friendsList;
    QLabel *profileLabel;
    QLabel *friendsLabel;
    QLabel *playersLabel;
    QLabel *versionLabel;
    QLabel *newsLabel;
};

#endif
