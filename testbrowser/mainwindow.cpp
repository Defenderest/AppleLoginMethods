// mainwindow.cpp
#include "mainwindow.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QOAuthHttpServerReplyHandler>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWebEngineProfile>
#include <QWebEnginePage>
#include <QUrl>
#include <QUrlQuery>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    initializeOAuth();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUi() {
    setWindowTitle("Apple Login Example");
    resize(800, 600);

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    m_layout = new QVBoxLayout(m_centralWidget);
    m_layout->setSpacing(20);
    m_layout->setContentsMargins(40, 40, 40, 40);

    // Створюємо статус лейбл
    m_statusLabel = new QLabel("Not logged in", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("QLabel { color: gray; font-size: 14px; }");
    m_layout->addWidget(m_statusLabel);

    // Створюємо WebView
    m_webView = new QWebEngineView(this);
    m_webView->setMinimumSize(720, 400);
    m_webView->hide(); // Спочатку приховуємо
    m_layout->addWidget(m_webView);

    // Створюємо кнопку входу
    m_loginButton = new QPushButton("Sign in with Apple", this);
    m_loginButton->setMinimumHeight(45);
    m_loginButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #000000;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border-radius: 5px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #333333;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #444444;"
        "}"
        );

    m_layout->addWidget(m_loginButton);
    m_layout->addStretch();

    connect(m_loginButton, &QPushButton::clicked, this, &MainWindow::handleLogin);
}

void MainWindow::initializeOAuth() {
    m_networkManager = new QNetworkAccessManager(this);
    m_oauth2 = new QOAuth2AuthorizationCodeFlow(this);
    m_oauth2->setNetworkAccessManager(m_networkManager);


    const QString clientId = "your.client.id"; // Your AppleID
    const auto redirectUri = QUrl("your.redirect.uri");
    const auto authEndpoint = QUrl("https://appleid.apple.com/auth/authorize");
    const auto tokenEndpoint = QUrl("https://appleid.apple.com/auth/token");

    m_oauth2->setClientIdentifier(clientId);
    m_oauth2->setAuthorizationUrl(authEndpoint);
    m_oauth2->setAccessTokenUrl(tokenEndpoint);

    auto replyHandler = new QOAuthHttpServerReplyHandler(1234, this);
    m_oauth2->setReplyHandler(replyHandler);

    connect(m_oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            [this](const QUrl &url) {
                m_webView->show();
                m_webView->load(url);
            });

    connect(m_oauth2, &QOAuth2AuthorizationCodeFlow::granted,
            this, &MainWindow::handleGrant);

    QStringList scopes;
    scopes << "name" << "email";
    m_oauth2->setScope(scopes.join(" "));
}

void MainWindow::handleLogin() {
    m_oauth2->grant();
}

void MainWindow::handleGrant() {
    m_webView->hide();
    updateUserInterface(true);

    QString token = m_oauth2->token();
    m_statusLabel->setText("Successfully logged in!");
    m_statusLabel->setStyleSheet("QLabel { color: green; font-size: 14px; }");
}

void MainWindow::handleAuthenticationComplete() {
    m_webView->hide();
    updateUserInterface(true);
    QMessageBox::information(this, "Success", "Authentication completed successfully!");
}

void MainWindow::handleError(const QString &error) {
    m_webView->hide();
    updateUserInterface(false);
    QMessageBox::warning(this, "Error", "Authentication error: " + error);
}

void MainWindow::updateUserInterface(bool authenticated) {
    if (authenticated) {
        m_loginButton->setEnabled(false);
        m_loginButton->setText("Signed In");
    } else {
        m_loginButton->setEnabled(true);
        m_loginButton->setText("Sign in with Apple");
    }
}
