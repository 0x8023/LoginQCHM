#ifndef AUTOLOGIN_H
#define AUTOLOGIN_H

#include <QObject>

#include <iostream>

#include <QtNetwork>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include <QRegExp>

#include <networkcookie.h>

class AutoLogin : public QObject
{
        Q_OBJECT
    public:
        explicit AutoLogin(QObject *parent = 0);
        ~AutoLogin();

        void autoLogin();
        void setIdentity(QString identity);
        void setPassword(QString password);

    signals:
        void quit();

    private:
        const QString REGULAR = "username=%1&password=%2";
        const QString LOGINURL = "http://192.168.1.93/post.php";

        QString Loginurl;
        QString Identity;
        QString Password;
        NetworkCookie* cookiejar;

        bool getConnectionState();
        void initGetUrl();
        bool getLoginState(QString html);
        QString getSessionUrl(QString html);
        void initGetCookies(QString url);
        void initLogin();

    private slots:
        void onReplyFinished(QNetworkReply* reply);
        void onCookieFinished(QNetworkReply* reply);
        void onLoginFinished(QNetworkReply* reply);
};

#endif // AUTOLOGIN_H
