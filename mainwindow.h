#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QString>
#include <QLabel>
#include <QFontComboBox>
#include <QDialog>
#include <QLabel>
#include <QFont>
#include <QHBoxLayout>
#include <QFile>
#include <QRadioButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QMenuBar *menuBar;
    QMenu *helpMenu;
    QAction *aboutAction;
    QMenu *appearanceMenu;
    QAction *darkMode;
    QAction *lightMode;
    QAction *reportIssue;
    QMenu *updateMenu;
    QAction *checkUpdate;
    QString appVersion;
    QLabel *versionLabel;
    QLabel *descriptionLabel;
    QMenu *fontMenu;
    QAction *chooseFont;
    QFontComboBox *fontBox;
    QDialog *fontDialog;
    QLabel *fontPreview;
    QFont selectedFont;
    QHBoxLayout *fontDiaBtnLay;
    QMenu *exitMenu;
    QAction *quitApp;
    QAction *aboutQtAct;
    QAction *releasesSite;
    QDialog *cipherInput;
    QString message;
    QString savedFilePath;
    QString decipheredMsg;
    QFile *themeSettingsFile;
    QFile *fontSettingsFile;

public slots:
    void setDarkMode();
    void setLightMode();
    void aboutMsg();
    void reportIssueFunc();
    void checkUpdateFunc();
    void handleRequestFinished();
    void copyVersionText();
    void chooseFontFunc();
    void changePreviewFont();
    void applyFontFunc();
    void openGithubReleases();
    void cipherInputDialog();
    void cipherEnterMsg();
    void cipherOpenMsgFile();
    void cipherText();
    void openCipheredFileDirFunc();
    void openCipheredFileFunc();
    void decipherText();
    void copyMsg();
    void saveDarkMode();
    void saveLightMode();
    void saveFont();

signals:
    void appFontChanged(QFont newFont);
};
#endif // MAINWINDOW_H
