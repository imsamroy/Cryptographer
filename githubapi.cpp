#include "githubapi.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>
#include <QDebug>
#include <QByteArray>
#include <QNetworkAccessManager>

GitHubApi::GitHubApi(const QString& owner, const QString& repo, QObject* parent)
    : QObject(parent), owner_(owner), repo_(repo), parent_(nullptr)
{
    connect(&networkManager_, &QNetworkAccessManager::finished, this, &GitHubApi::onRequestFinished);
}

void GitHubApi::fetchLatestReleaseTag(QWidget* parent)
{
    QUrl apiUrl(QString("https://api.github.com/repos/%1/%2/releases/latest").arg(owner_, repo_));
    QNetworkRequest request(apiUrl);
    networkManager_.get(request);
    parent_ = parent;  // Store the parent widget for later use
}

void GitHubApi::onRequestFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);
        extractReleaseTag(jsonDocument.object());
    } else {
        // Show a warning dialog with the parent widget
        QMessageBox::warning(parent_, "Network Error", reply->errorString());
        releaseTag = "Error";
    }
    emit requestFinished();
    reply->deleteLater();
}

void GitHubApi::extractReleaseTag(const QJsonObject& jsonObject)
{
    QJsonValue tagName = jsonObject.value("tag_name");
    if (tagName.isString()) {
        releaseTag = tagName.toString();
    } else {
        // Show a warning dialog with the parent widget
        QMessageBox::critical(parent_, "Error", "Unable to extract tag name");
        releaseTag = "Error";
    }
}

QString GitHubApi::returnReleaseTag()
{
    return releaseTag;
}
