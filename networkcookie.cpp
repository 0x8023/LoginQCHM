#include "networkcookie.h"

NetworkCookie::NetworkCookie(){

}

QList<QNetworkCookie> NetworkCookie::getCookies(){
    return (allCookies());
}

void NetworkCookie::setCookies(const QList<QNetworkCookie> &cookieList){
    setAllCookies(cookieList);
}
