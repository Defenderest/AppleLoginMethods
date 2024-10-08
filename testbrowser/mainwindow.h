// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QOAuth2AuthorizationCodeFlow>
#include <QWebEngineView>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLogin();
    void handleAuthenticationComplete();
    void handleError(const QString &error);
    void handleGrant();

private:
    void setupUi();
    void initializeOAuth();
    void updateUserInterface(bool authenticated);

    QPushButton *m_loginButton;
    QLabel *m_statusLabel;
    QOAuth2AuthorizationCodeFlow *m_oauth2;
    QNetworkAccessManager *m_networkManager;
    QWebEngineView *m_webView;
    QWidget *m_centralWidget;
    QVBoxLayout *m_layout;
};

#endif // MAINWINDOW_H
