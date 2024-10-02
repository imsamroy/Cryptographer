#include "mainwindow.h"
#include "githubapi.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QPalette>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QMenuBar>
#include <QDialog>
#include <QDesktopServices>
#include <QObject>
#include <QUrl>
#include <QAction>
#include <QColor>
#include <QStyle>
#include <QMessageBox>
#include <QFontMetrics>
#include <QClipboard>
#include <QFontComboBox>
#include <QFontDialog>
#include <QSizePolicy>
#include <QSysInfo>
#include <QInputDialog>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QChar>
#include <QProcess>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStyleHints>
#include <QJsonParseError>
#include <QByteArray>
#include <QRadioButton>
#include <QRandomGenerator>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Cryptographer");
    resize(1100, 550);

    appVersion = "v0.0.2";

    QDir dir(QDir::toNativeSeparators(QDir::homePath()+"/CryptographerData"));
    themeSettingsFile = new QFile(QDir::toNativeSeparators(QDir::homePath()+"/CryptographerData/themeSettings.json"));
    fontSettingsFile = new QFile(QDir::toNativeSeparators(QDir::homePath()+"/CryptographerData/fontSettings.json"));
    if (dir.exists()) {
        if (themeSettingsFile->exists() && themeSettingsFile->size()!=0) {
            if (themeSettingsFile->open(QIODevice::ReadOnly)) {
                QByteArray jsonBytes = themeSettingsFile->readAll();
                themeSettingsFile->close();

                QJsonParseError parseError;
                QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonBytes, &parseError);
                if (parseError.error == QJsonParseError::NoError) {
                    if (jsonDoc.isObject()) {
                        QJsonObject jsonObj = jsonDoc.object();

                        if (jsonObj.value("theme").toString() == "dark") {
                            setDarkMode();
                        }
                        else if (jsonObj.value("theme").toString() == "light") {
                            setLightMode();
                        }
                        else {
                            QMessageBox msgBox;
                            msgBox.warning(this, "Failed to load theme from settings", "No valid theme specified in settings. Click OK to continue anyway.");
                            msgBox.show();
                        }
                    }
                }
                else {
                    QMessageBox msgBox;
                    msgBox.warning(this, "Theme settings file: Parse Error", "Failed to parse JSON data: "+parseError.errorString()+"Click OK to continue anyway.");
                    msgBox.show();
                }
            }
        }
        else {
            if (themeSettingsFile->open(QIODevice::ReadWrite | QIODevice::Truncate)) {
                QJsonObject jsonObj;
                QStyleHints *styleHints = QGuiApplication::styleHints();
                if (styleHints->colorScheme() == Qt::ColorScheme::Dark) {
                    jsonObj["theme"] = "dark";
                    setDarkMode();
                }
                else if (styleHints->colorScheme() == Qt::ColorScheme::Light) {
                    jsonObj["theme"] = "light";
                    setLightMode();
                }
                else {
                    jsonObj["theme"] = "light";
                    setLightMode();
                }
                QJsonDocument jsonDoc;
                jsonDoc.setObject(jsonObj);
                themeSettingsFile->write(jsonDoc.toJson(QJsonDocument::JsonFormat::Indented));
                themeSettingsFile->encodeName("UTF-8");
                themeSettingsFile->close();
            }
        }
        if (fontSettingsFile->exists() && fontSettingsFile->size()!=0) {
            if (fontSettingsFile->open(QIODevice::ReadOnly)) {
                QByteArray bytes = fontSettingsFile->readAll();
                fontSettingsFile->close();
                
                QJsonParseError parseError2;
                QJsonDocument doc = QJsonDocument::fromJson(bytes, &parseError2);
                if (parseError2.error == QJsonParseError::NoError) {
                    if (doc.isObject()) {
                        QJsonObject obj = doc.object();
                        QFont font;
                        font.setFamily(obj.value("font").toString());
                        QApplication::setFont(font);                        
                    }
                }
                else {
                    QMessageBox msgBox2;
                    msgBox2.warning(this, "Font settings file: Parse error", "Failed to parse JSON data: "+parseError2.errorString()+"Click OK to continue anyway.");
                    msgBox2.show();
                }
            }
        }
        else {
            if (fontSettingsFile->open(QIODevice::ReadWrite | QIODevice::Truncate)) {
                QJsonObject jsonObj2;
                QFont defaultFont;
                defaultFont.setFamily(defaultFont.defaultFamily());
                jsonObj2["font"] = defaultFont.family();
                QApplication::setFont(defaultFont);
                QJsonDocument jsonDoc2;
                jsonDoc2.setObject(jsonObj2);
                fontSettingsFile->write(jsonDoc2.toJson(QJsonDocument::JsonFormat::Indented));
                fontSettingsFile->encodeName("UTF-8");
                fontSettingsFile->close();
            }
        }
    }
    else {
        QDir::home().mkdir("CryptographerData");
        if (themeSettingsFile->open(QIODevice::ReadWrite | QIODevice::Truncate)) {
            QJsonObject jsonObj;
            QStyleHints *styleHints = QGuiApplication::styleHints();
            if (styleHints->colorScheme() == Qt::ColorScheme::Dark) {
                jsonObj["theme"] = "dark";
                setDarkMode();
            }
            else if (styleHints->colorScheme() == Qt::ColorScheme::Light) {
                jsonObj["theme"] = "light";
                setLightMode();
            }
            else {
                jsonObj["theme"] = "light";
                setLightMode();
            }
            QJsonDocument jsonDoc;
            jsonDoc.setObject(jsonObj);
            themeSettingsFile->write(jsonDoc.toJson(QJsonDocument::JsonFormat::Indented));
            themeSettingsFile->encodeName("UTF-8");
            themeSettingsFile->close();
        }
        if (fontSettingsFile->open(QIODevice::ReadWrite | QIODevice::Truncate)) {
            QJsonObject jsonObj2;
            QFont defaultFont;
            defaultFont.setFamily(defaultFont.defaultFamily());
            jsonObj2["font"] = defaultFont.family();
            QApplication::setFont(defaultFont);
            QJsonDocument jsonDoc2;
            jsonDoc2.setObject(jsonObj2);
            fontSettingsFile->write(jsonDoc2.toJson(QJsonDocument::JsonFormat::Indented));
            fontSettingsFile->encodeName("UTF-8");
            fontSettingsFile->close();
        }
    }

    menuBar = new QMenuBar(this);
    menuBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMenuBar(menuBar);

    helpMenu = new QMenu("Help", menuBar);
    helpMenu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    appearanceMenu = new QMenu("Appearance", menuBar);
    appearanceMenu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    updateMenu = new QMenu("Updates", menuBar);
    updateMenu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    fontMenu = new QMenu("Font", menuBar);
    fontMenu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    exitMenu = new QMenu("Exit", menuBar);
    exitMenu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    aboutAction = new QAction("About Cryptographer", helpMenu);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutMsg()));

    darkMode = new QAction("Dark Mode", appearanceMenu);
    connect(darkMode, SIGNAL(triggered()), this, SLOT(setDarkMode()));
    connect(darkMode, SIGNAL(triggered()), this, SLOT(saveDarkMode()));
    lightMode = new QAction("Light Mode", appearanceMenu);
    connect(lightMode, SIGNAL(triggered()), this, SLOT(setLightMode()));
    connect(lightMode, SIGNAL(triggered()), this, SLOT(saveLightMode()));

    reportIssue = new QAction("Report an issue", helpMenu);
    connect(reportIssue, SIGNAL(triggered()), this, SLOT(reportIssueFunc()));

    checkUpdate = new QAction("Check for updates", updateMenu);
    connect(checkUpdate, SIGNAL(triggered()), this, SLOT(checkUpdateFunc()));

    chooseFont = new QAction("Change app font", fontMenu);
    connect(chooseFont, SIGNAL(triggered()), this, SLOT(chooseFontFunc()));

    quitApp = new QAction("Quit application", exitMenu);
    connect(quitApp, &QAction::triggered, qApp, &QApplication::quit);

    aboutQtAct = new QAction("About Qt", helpMenu);
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);

    releasesSite = new QAction("Releases website", updateMenu);
    connect(releasesSite, SIGNAL(triggered()), this, SLOT(openGithubReleases()));

    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAct);
    helpMenu->addAction(reportIssue);

    appearanceMenu->addAction(darkMode);
    appearanceMenu->addAction(lightMode);

    updateMenu->addAction(checkUpdate);
    updateMenu->addAction(releasesSite);

    fontMenu->addAction(chooseFont);

    exitMenu->addAction(quitApp);

    menuBar->addMenu(appearanceMenu);
    menuBar->addMenu(helpMenu);
    menuBar->addMenu(updateMenu);
    menuBar->addMenu(fontMenu);
    menuBar->addMenu(exitMenu);

    connect(this, SIGNAL(appFontChanged(QFont)), this, SLOT(saveFont()));

    QLabel *appTitle = new QLabel("<u>Cryptographer</u>", this);

    // Create a slot to update the font of topLabel
    auto updateAppTitleFont = [appTitle]() {
        QFont appTitleFont = QApplication::font();
        appTitleFont.setBold(true);
        appTitleFont.setPixelSize(50);
        appTitle->setFont(appTitleFont);
    };

    // Connect the fontChanged signal
    connect(this, &MainWindow::appFontChanged, this, [appTitle](QFont newFont) {
        newFont.setBold(true);
        newFont.setPixelSize(50);
        appTitle->setFont(newFont);
    });

    // Initial call to set the initial font
    updateAppTitleFont();

    QLabel *appDesc = new QLabel("Cipher any message and be able to decipher it", this);
    auto updateAppDescFont = [appDesc]() {
        QFont appDescFont = QApplication::font();
        appDescFont.setBold(true);
        appDescFont.setPixelSize(30);
        appDesc->setFont(appDescFont);
    };
    connect(this, &MainWindow::appFontChanged, this, [appDesc](QFont newFont) {
        newFont.setBold(true);
        newFont.setPixelSize(30);
        appDesc->setFont(newFont);
    });
    updateAppDescFont();

    QPushButton *cipher = new QPushButton("Cipher a new message");
    cipher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    connect(cipher, SIGNAL(clicked()), this, SLOT(cipherInputDialog()));

    QPushButton *decipher = new QPushButton("Decipher an already ciphered message");
    decipher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    connect(decipher, SIGNAL(clicked()), this, SLOT(decipherText()));

    auto updateButtonsFont = [cipher, decipher]() {
        QFont buttonsFont = QApplication::font();
        buttonsFont.setPixelSize(20);
        cipher->setFont(buttonsFont);
        decipher->setFont(buttonsFont);
    };
    connect(this, &MainWindow::appFontChanged, this, [cipher, decipher](QFont newFont) {
        newFont.setPixelSize(20);
        cipher->setFont(newFont);
        decipher->setFont(newFont);
    });
    updateButtonsFont();

    QLabel *appIconLabel = new QLabel(this);
    QIcon appIcon = QIcon(":/Cryptographer.svg");
    QPixmap appIconPix = appIcon.pixmap(QSize(90, 90));
    appIconLabel->setPixmap(appIconPix);

    // Create layouts
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout();  // New layout for buttons
    QHBoxLayout *titleLayout = new QHBoxLayout();

    // Add widgets to layouts
    titleLayout->addWidget(appIconLabel);
    titleLayout->addWidget(appTitle);
    titleLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    mainLayout->addLayout(titleLayout);
    mainLayout->addSpacing(25); //add some spacing here
    mainLayout->addWidget(appDesc, 0, Qt::AlignTop | Qt::AlignHCenter);
    mainLayout->addStretch(); // Add stretch to push buttons to the center
    mainLayout->addLayout(buttonLayout);  // Add the button layout
    mainLayout->addStretch(); // Add stretch to push buttons to the bottom

    // Add buttons to the button layout
    buttonLayout->addWidget(cipher);
    buttonLayout->addWidget(decipher);

    // Set the main layout to the central widget
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {}

void MainWindow::setDarkMode()
{
    //QApplication::setStyle("fusion");
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window,QColor(35,35,35));
    darkPalette.setColor(QPalette::WindowText,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::WindowText,QColor(127,127,127));
    darkPalette.setColor(QPalette::Base,QColor(35,35,35));
    darkPalette.setColor(QPalette::AlternateBase,QColor(66,66,66));
    darkPalette.setColor(QPalette::ToolTipBase,QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipText,Qt::white);
    darkPalette.setColor(QPalette::Text,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::Text,QColor(127,127,127));
    darkPalette.setColor(QPalette::Dark,QColor(35,35,35));
    darkPalette.setColor(QPalette::Shadow,QColor(20,20,20));
    darkPalette.setColor(QPalette::Button,QColor(42,42,42));
    darkPalette.setColor(QPalette::ButtonText,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::ButtonText,QColor(127,127,127));
    darkPalette.setColor(QPalette::BrightText,Qt::red);
    darkPalette.setColor(QPalette::Link,QColor(42,130,218));
    darkPalette.setColor(QPalette::Highlight,QColor(70,130,180));
    darkPalette.setColor(QPalette::Disabled,QPalette::Highlight,QColor(80,80,80));
    darkPalette.setColor(QPalette::HighlightedText,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::HighlightedText,QColor(127,127,127));

    QApplication::setPalette(darkPalette);
}

void MainWindow::setLightMode()
{
    //QApplication::setStyle("fusion");
    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::WindowText, Qt::black);
    lightPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
    lightPalette.setColor(QPalette::Base, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::AlternateBase, QColor(200, 200, 200));
    lightPalette.setColor(QPalette::ToolTipBase, QColor(255, 255, 255));
    lightPalette.setColor(QPalette::ToolTipText, Qt::black);
    lightPalette.setColor(QPalette::Text, Qt::black);
    lightPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    lightPalette.setColor(QPalette::Dark, QColor(200, 200, 200));
    lightPalette.setColor(QPalette::Shadow, QColor(180, 180, 180));
    lightPalette.setColor(QPalette::Button, QColor(215, 215, 215));
    lightPalette.setColor(QPalette::ButtonText, Qt::black);
    lightPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
    lightPalette.setColor(QPalette::BrightText, Qt::red);
    lightPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    lightPalette.setColor(QPalette::Highlight, QColor(130, 190, 255));
    lightPalette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
    lightPalette.setColor(QPalette::HighlightedText, Qt::black);
    lightPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));

    QApplication::setPalette(lightPalette);
}

void MainWindow::aboutMsg()
{
    QDialog aboutDialog(this);
    aboutDialog.setWindowTitle("About Cryptographer");
    aboutDialog.resize(550, 400);
    aboutDialog.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QVBoxLayout* layout = new QVBoxLayout(&aboutDialog);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(10);

    QLabel *titleIconLabel = new QLabel(this);
    QIcon titleIcon = QIcon(":/Cryptographer.svg");
    QPixmap titleIconPix = titleIcon.pixmap(QSize(75, 75));
    titleIconLabel->setPixmap(titleIconPix);
    titleIconLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleIconLabel);

    QLabel* titleLabel = new QLabel("Cryptographer");
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 25px; font-weight: bold;");
    layout->addWidget(titleLabel);

    QPushButton* versionBtn = new QPushButton();
    versionBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    versionLabel = new QLabel("Release: "+appVersion);
    versionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    versionBtn->setText(versionLabel->text());
    versionBtn->setStyleSheet("font-px: 13;");
    versionBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum); //keep the length and width of the button both minimum as per that of text inside
    //set the width of the button to that of the text inside and add some more width to accomodate the full text which includes spaces
    versionBtn->setFixedWidth(versionBtn->fontMetrics().horizontalAdvance(versionBtn->text())+30);
    versionBtn->setToolTip("Click to copy stats");
    connect(versionBtn, SIGNAL(clicked()), this, SLOT(copyVersionText()));
    QHBoxLayout* versBtnLayout = new QHBoxLayout(&aboutDialog);
    versBtnLayout->addWidget(versionBtn);
    layout->addLayout(versBtnLayout, Qt::AlignCenter);

    descriptionLabel = new QLabel("OS: "+QSysInfo::prettyProductName()+", Architecture: "+QSysInfo::currentCpuArchitecture()+", Kernel: "+QSysInfo::kernelType()+" "+QSysInfo::kernelVersion());
    descriptionLabel->setAlignment(Qt::AlignCenter);
    descriptionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layout->addWidget(descriptionLabel);

    QLabel* authorLabel = new QLabel("Developer: <a href=\"mailto:sampreet.roy21@gmail.com\">Sampreet Roy</a>");
    authorLabel->setAlignment(Qt::AlignCenter);
    authorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    authorLabel->setTextFormat(Qt::RichText);
    authorLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    authorLabel->setOpenExternalLinks(true);
    QObject::connect(authorLabel, &QLabel::linkActivated, [](const QString& link) {
        QDesktopServices::openUrl(QUrl(link));
    });
    layout->addWidget(authorLabel);

    QLabel* websiteLabel = new QLabel("My Links: <a href=\"https://github.com/imsamroy\">GitHub</a>, <a href=\"https://instagram.com/iamsamroy__\">Instagram</a>, <a href=\"https://twitter.com/WrathOf839\">X (Twitter)</a>");
    websiteLabel->setAlignment(Qt::AlignCenter);
    websiteLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    websiteLabel->setTextFormat(Qt::RichText);
    websiteLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    websiteLabel->setOpenExternalLinks(true);
    QObject::connect(websiteLabel, &QLabel::linkActivated, [](const QString& link2) {
        QDesktopServices::openUrl(QUrl(link2));
    });
    layout->addWidget(websiteLabel);


    QLabel* licenseLabel = new QLabel("Software License: GNU GPLv3");
    licenseLabel->setAlignment(Qt::AlignCenter);
    licenseLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layout->addWidget(licenseLabel);

    QPushButton* closeButton = new QPushButton("Close");
    closeButton->setBaseSize(100, 30);
    closeButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QObject::connect(closeButton, &QPushButton::clicked, &aboutDialog, &QDialog::close);

    QPushButton* sourceLink = new QPushButton("Source code");
    sourceLink->setBaseSize(100,30);
    sourceLink->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QUrl projectGithubLink("https://github.com/imsamroy/Cryptographer");
    QObject::connect(sourceLink, &QPushButton::clicked, [projectGithubLink]() {
        QDesktopServices::openUrl(projectGithubLink);
    });

    QHBoxLayout* buttonLayout = new QHBoxLayout(&aboutDialog);
    buttonLayout->addWidget(sourceLink);
    buttonLayout->addWidget(closeButton);
    layout->addLayout(buttonLayout, Qt::AlignCenter);

    aboutDialog.exec();
}

void MainWindow::reportIssueFunc()
{
    QUrl githubIssueLink("https://github.com/imsamroy/Cryptographer/issues");
    QDesktopServices::openUrl(githubIssueLink);
}

void MainWindow::checkUpdateFunc()
{
    GitHubApi *gitHubApi = new GitHubApi("imsamroy", "Cryptographer", this);

    connect(gitHubApi, &GitHubApi::requestFinished, this, &MainWindow::handleRequestFinished);

    gitHubApi->fetchLatestReleaseTag(this);
}

void MainWindow::handleRequestFinished()
{
    GitHubApi* gitHubApi = qobject_cast<GitHubApi*>(sender());
    if (gitHubApi) {
        QString latestReleaseTag = gitHubApi->returnReleaseTag();
        if (latestReleaseTag != "Error") {
            if (latestReleaseTag == appVersion) {
                QMessageBox::information(this, "Update check", "Cryptographer is up to date (Version: "+appVersion+")");
            }
            else {
                QMessageBox *newUpdateMsg = new QMessageBox(this);
                newUpdateMsg->setWindowTitle("Update check");
                newUpdateMsg->setIcon(QMessageBox::Information); //QMessageBox::information was used to directly declare the widget but QMessageBox::Information here represents only the icon
                newUpdateMsg->setText("New update available: "+latestReleaseTag+"<br>Current version: "+appVersion+"<br>Visit: <a href = \"https://github.com/imsamroy/Cryptographer/releases\">https://github.com/imsamroy/Cryptographer/releases/latest</a> to download the latest release");
                newUpdateMsg->setTextFormat(Qt::RichText);
                newUpdateMsg->setTextInteractionFlags(Qt::TextBrowserInteraction);
                newUpdateMsg->addButton(QMessageBox::Close);
                newUpdateMsg->exec(); //exec() means the code will wait till the user closes the msgbox, but if show() is used then code will continue to progress while the msgbox is opened
            }
        }
        else {
            //error (warning box of failed tag extraction will load from the githubapi class already in this case
        }
    }
}

void MainWindow::copyVersionText()
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(versionLabel->text()+descriptionLabel->text());
}

void MainWindow::chooseFontFunc()
{
    fontDialog = new QDialog(this);
    fontDialog->setWindowTitle("Customize application font");
    fontBox = new QFontComboBox();
    fontPreview = new QLabel("This is how this font will look on text and 123!@#");
    fontPreview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QVBoxLayout *fontDialogLayout = new QVBoxLayout(fontDialog);
    fontDialogLayout->addWidget(fontBox);
    fontDialogLayout->addWidget(fontPreview);

    fontDiaBtnLay = new QHBoxLayout();
    QPushButton *closeFontDialog = new QPushButton("Cancel", fontDialog);
    fontDiaBtnLay->addWidget(closeFontDialog);
    connect(closeFontDialog, SIGNAL(clicked()), fontDialog, SLOT(close()));

    QPushButton *applyFont = new QPushButton("Apply Font", fontDialog);
    fontDiaBtnLay->addWidget(applyFont);
    connect(applyFont, SIGNAL(clicked()), this, SLOT(applyFontFunc()));

    fontDialogLayout->addLayout(fontDiaBtnLay);
    fontDialog->show();
    connect(fontBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(changePreviewFont()));
}

void MainWindow::changePreviewFont()
{
    selectedFont = QFont(fontBox->itemText(fontBox->currentIndex()));
    fontPreview->setFont(selectedFont);
}

void MainWindow::applyFontFunc()
{
    QApplication::setFont(selectedFont);
    fontDialog->close();
    emit appFontChanged(selectedFont);
}

void MainWindow::openGithubReleases()
{
    QDesktopServices::openUrl(QUrl("https://github.com/imsamroy/Cryptographer/releases"));
}

void MainWindow::cipherInputDialog()
{
    cipherInput = new QDialog(this);
    cipherInput->setWindowTitle("Cipher a new message");
    cipherInput->resize(420, 200);

    QLabel *cipherInputInfo = new QLabel("Choose how to share the message to be ciphered:");

    QPushButton *enterMsg = new QPushButton("Enter message to be ciphered");
    enterMsg->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(enterMsg, SIGNAL(clicked()), this, SLOT(cipherEnterMsg()));

    QPushButton *openMsgFile = new QPushButton("Select text file containing message");
    openMsgFile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(openMsgFile, SIGNAL(clicked()), this, SLOT(cipherOpenMsgFile()));

    QVBoxLayout *vbox = new QVBoxLayout(cipherInput);
    vbox->addWidget(cipherInputInfo, 0, Qt::AlignTop | Qt::AlignHCenter);
    vbox->addStretch();
    vbox->addWidget(enterMsg);
    vbox->addWidget(openMsgFile);
    vbox->addStretch();

    cipherInput->show();
}

void MainWindow::cipherEnterMsg()
{
    message = QInputDialog::getMultiLineText(this, "Cipher a new message", "Enter message to be ciphered:");
    cipherInput->close();
    cipherText();
}

void MainWindow::cipherOpenMsgFile()
{
    QString openedFilePath = QFileDialog::getOpenFileName(this, "Select text file containing message", QDir::homePath(), "Text files (*.txt)");
    cipherInput->close();
    if (!openedFilePath.isEmpty()) {
        QFile file(openedFilePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream inStream(&file);
            message = inStream.readAll();
            file.close();
            cipherText();
        }
        else {
            QMessageBox::critical(this, "Error: Cannot open file", "Cannot open selected file for reading message to be ciphered");
        }
    }
    else {
        QMessageBox::warning(this, "Error: No file selected", "No file selected, so no message received for ciphering");
    }
}

void MainWindow::cipherText()
{
    if (!message.isEmpty()) {
        QMessageBox info(this);
        info.setWindowTitle("Ready to cipher message");
        info.setIcon(QMessageBox::Information);
        info.setText("Your message has been received and its ciphered form will be saved to a text file in your system<br>Click OK to continue");
        info.addButton(QMessageBox::Ok);
        info.exec();

        QString userEnteredPath = QFileDialog::getSaveFileName(this, "Save text file with ciphered message", QDir::homePath(), "Text Files (*.txt)");
        if (!userEnteredPath.isEmpty()) {
            if (userEnteredPath.right(4) == ".txt") {
                savedFilePath = userEnteredPath;
            }
            else {
                savedFilePath = userEnteredPath + ".txt";
            }
            QFile file(savedFilePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream outStream(&file);

                QChar arr[message.length()];
                int arr2[message.length()];

                for (int i = 0; i < message.length(); i++) {
                    arr[i] = message[i];
                    arr2[i] = static_cast<int>(arr[i].unicode()); //obtains unicode value of each character and casts it to int
                    outStream << qSetPadChar('0') << qSetFieldWidth(3) << arr2[i]; //each char is converted to int and each value when it is written to file has to be of 3 chars and if it is of 2 digits then a 0 will come before it to fill the gap
                }

                file.close();

                QDialog *cipherSuccessInfo = new QDialog(this);
                cipherSuccessInfo->setWindowTitle("Message successfully ciphered");
                cipherSuccessInfo->resize(400, 200);

                QLabel *cipherSuccess = new QLabel("Your message has been successfully ciphered!<br><br>The ciphered message is stored in your system<br>in the file at: "+savedFilePath);
                QVBoxLayout *vbox = new QVBoxLayout(cipherSuccessInfo);

                QPushButton *openCipheredFileDir = new QPushButton("Open folder containing ciphered message");
                openCipheredFileDir->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                connect(openCipheredFileDir, SIGNAL(clicked()), this, SLOT(openCipheredFileDirFunc()));

                QPushButton *openCipheredFile = new QPushButton("Open ciphered message");
                openCipheredFile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                connect(openCipheredFile, SIGNAL(clicked()), this, SLOT(openCipheredFileFunc()));

                vbox->addWidget(cipherSuccess, 0, Qt::AlignTop | Qt::AlignHCenter);
                vbox->addStretch();
                vbox->addWidget(openCipheredFile);
                vbox->addWidget(openCipheredFileDir);
                vbox->addStretch();

                cipherSuccessInfo->show();
            }
            else {
                QMessageBox::critical(this, "Error: Cannot write to file", "Cannot open file for writing the ciphered message to it");
            }
        }
        else {
            QMessageBox::warning(this, "Error: Message not ciphered", "No file saved, so message not ciphered");
        }
    }
    else {
        QMessageBox::warning(this, "Error: No message to cipher", "No message was entered or file containing message was empty");
    }
}

void MainWindow::openCipheredFileDirFunc()
{
#ifdef Q_OS_LINUX
    QProcess::startDetached("xdg-open", {QFileInfo(savedFilePath).absolutePath()});
#endif

#ifdef Q_OS_WIN
    QProcess::startDetached("explorer.exe", {"/select,", QDir::toNativeSeparators(savedFilePath)});
#endif
}

void MainWindow::openCipheredFileFunc()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(savedFilePath).absoluteFilePath()));
}

void MainWindow::decipherText()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Decipher an already ciphered message");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("You have to select the text file containing the ciphered message which you saved during ciphering<br>Click OK to continue");
    msgBox.addButton(QMessageBox::Ok);
    msgBox.exec();

    QString openCipheredMsgPath = QFileDialog::getOpenFileName(this, "Open text file containing ciphered message", QDir::homePath(), "Text Files (*.txt)");
    if (!openCipheredMsgPath.isEmpty()) {
        QFile file(openCipheredMsgPath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream inStream(&file);
            QString cipheredMsg = inStream.readAll();
            file.close();

            if (!cipheredMsg.isEmpty()) {
                size_t count = std::count_if(cipheredMsg.begin(), cipheredMsg.end(), []( QChar c ) { return c.isDigit(); });
                int x = count / 3;
                QChar arr[x];
                int arr2[x];
                QString s;
                decipheredMsg = "";

                for (int i = 0; i < cipheredMsg.length(); i = i+3)
                {
                    s = QString(cipheredMsg[i]) + QString(cipheredMsg[i+1]) + QString(cipheredMsg[i+2]);
                    for (int j = 0; j < 1; j++)
                    {
                        arr2[j] = s.toInt();
                        arr[j] = (QChar)arr2[j];
                        decipheredMsg += arr[j];
                    }
                }

                //QMessageBox::information(this, "Message deciphered successfully", "The message is: "+decipheredMsg);
                QDialog *decipherInfo = new QDialog(this);
                decipherInfo->setWindowTitle("Message deciphered successfully");
                decipherInfo->resize(400, 100);
                QVBoxLayout *vbox2 = new QVBoxLayout(decipherInfo);
                QLabel *label = new QLabel("The message is: \n\n"+decipheredMsg);
                QPushButton *copy = new QPushButton("Copy message");
                connect(copy, SIGNAL(clicked()), this, SLOT(copyMsg()));
                vbox2->addWidget(label);
                vbox2->addStretch();
                vbox2->addWidget(copy);
                decipherInfo->exec();
            }
            else {
                QMessageBox::warning(this, "Error: No message to decipher", "File selected is empty, so no ciphered message received for deciphering");
            }
        }
        else {
            QMessageBox::critical(this, "Error: Cannot open file", "Cannot open file to read ciphered message and decipher it");
        }
    }
    else {
        QMessageBox::warning(this, "Error: Message not deciphered", "No file selected, so ciphered message not received and deciphering could not be done");
    }
}

void MainWindow::copyMsg()
{
    QClipboard *clip = QGuiApplication::clipboard();
    clip->setText(decipheredMsg);
}

void MainWindow::saveDarkMode()
{
    QByteArray bytes;
    if (themeSettingsFile->open(QIODevice::ReadOnly)) {
        bytes = themeSettingsFile->readAll();
        themeSettingsFile->close();
    }
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(bytes, &error);
    if (error.error == QJsonParseError::NoError) {
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            obj["theme"] = "dark";
            QJsonDocument doc2;
            doc2.setObject(obj);
            if (themeSettingsFile->open(QIODevice::ReadWrite | QIODevice::Truncate)) {
                themeSettingsFile->resize(0);
                themeSettingsFile->write(doc2.toJson(QJsonDocument::JsonFormat::Indented));
                themeSettingsFile->encodeName("UTF-8");
                themeSettingsFile->close();
            }
        }
    }
    else {
        QMessageBox::warning(this, "JSON Parse Error", "Failed to parse settings JSON file: "+error.errorString());
    }
}

void MainWindow::saveLightMode()
{
    QByteArray bytes;
    if (themeSettingsFile->open(QIODevice::ReadOnly)) {
        bytes = themeSettingsFile->readAll();
        themeSettingsFile->close();
    }
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(bytes, &error);
    if (error.error == QJsonParseError::NoError) {
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            obj["theme"] = "light";
            QJsonDocument doc2;
            doc2.setObject(obj);
            if (themeSettingsFile->open(QIODevice::ReadWrite | QIODevice::Truncate)) {
                themeSettingsFile->resize(0);
                themeSettingsFile->write(doc2.toJson(QJsonDocument::JsonFormat::Indented));
                themeSettingsFile->encodeName("UTF-8");
                themeSettingsFile->close();
            }
        }
    }
    else {
        QMessageBox::warning(this, "JSON Parse Error", "Failed to parse settings JSON file: "+error.errorString());
    }
}

void MainWindow::saveFont()
{
    QByteArray bytes;
    if (fontSettingsFile->open(QIODevice::ReadOnly)) {
        bytes = fontSettingsFile->readAll();
        fontSettingsFile->close();
    }
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(bytes, &error);
    if (error.error == QJsonParseError::NoError) {
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            obj["font"] = QApplication::font().family();
            QJsonDocument doc2;
            doc2.setObject(obj);
            if (fontSettingsFile->open(QIODevice::ReadWrite | QIODevice::Truncate)) {
                fontSettingsFile->resize(0);
                fontSettingsFile->write(doc2.toJson(QJsonDocument::JsonFormat::Indented));
                fontSettingsFile->encodeName("UTF-8");
                fontSettingsFile->close();
            }
        }
    }
    else {
        QMessageBox::warning(this, "JSON Parse Error", "Failed to parse settings JSON file: "+error.errorString());
    }
}
