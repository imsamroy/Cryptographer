#ifndef GITHUBAPI_H
#define GITHUBAPI_H

#include <QObject>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>

class GitHubApi : public QObject
{
    Q_OBJECT

public:
    GitHubApi(const QString& owner, const QString& repo, QObject* parent = nullptr);
    void fetchLatestReleaseTag(QWidget* parent = nullptr);
    QString returnReleaseTag();

private slots:
    void onRequestFinished(QNetworkReply* reply);
    void extractReleaseTag(const QJsonObject& jsonObject);

private:
    QString owner_;
    QString repo_;
    QNetworkAccessManager networkManager_;
    QWidget* parent_;  // Store the parent widget
    QString releaseTag;

signals:
    void requestFinished();

};

#endif // GITHUBAPI_H
