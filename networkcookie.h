#ifndef NETWORKCOOKIE_H
#define NETWORKCOOKIE_H

#include <QNetworkCookie>
#include <QNetworkCookieJar>

class NetworkCookie : public QNetworkCookieJar{
    public:
        NetworkCookie();
        QList<QNetworkCookie> getCookies();
        void setCookies(const QList<QNetworkCookie> &cookieList);
};

#endif // NETWORKCOOKIE_H
