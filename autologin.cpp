#include "autologin.h"

//public
AutoLogin::AutoLogin(QObject *parent) : QObject(parent) {

}

AutoLogin::~AutoLogin() {

}

void AutoLogin::autoLogin() {
    if (!getConnectionState()) { //如果未连接网络
        std::cout << "No connection to network." << std::endl; //提示没有连接到网络
        emit quit(); //触发关闭程序的信号
        return; //跳出
    }
    initGetUrl(); //初始化获取url
}

void AutoLogin::setIdentity(QString identity) {
    Identity = identity;
}

void AutoLogin::setPassword(QString password) {
    Password = password;
}

//private slot
void AutoLogin::onReplyFinished(QNetworkReply* reply) { //百度访问完成回调函数
    if (reply->error() != QNetworkReply::NoError) { //如果请求失败
        std::cout << "Get url error." << std::endl; //提示获取url错误
        emit quit(); //触发关闭程序的信号
        return; //跳出
    }

    QString html = reply->readAll(); //读取获取的html代码
    if (getLoginState(html)) { //如果正常访问百度
        std::cout << "Connected network." << std::endl; //提示已经连接网络
        emit quit(); //触发关闭程序的信号
        return; //跳出
    }

    QString url = getSessionUrl(html); //获取需要跳转的地址
    if (url == "") { //如果没有获取到需要跳转的地址
        std::cout << "Only use in QCHM." << std::endl; //只能用于青岛酒店管理学院
        emit quit(); //触发关闭程序的信号
        return; //跳出
    }

    initGetCookies(url); //初始化获取cookie
}

void AutoLogin::onCookieFinished(QNetworkReply* reply) { //获取cookie完成回调函数
    if (reply->error() != QNetworkReply::NoError) { //如果请求失败
        std::cout << "Get cookies error." << std::endl; //提示获取cookie错误
        emit quit(); //触发关闭程序的信号
        return; //跳出
    }

    std::cout << "Your PHPSESSID is: "
              << cookiejar->getCookies()[0].value().data()
              << std::endl; //输出PHPSESSID信息

    initLogin(); //初始化登录
}

void AutoLogin::onLoginFinished(QNetworkReply* reply) { //登录完成回调函数
    if (reply->error() != QNetworkReply::NoError) { //如果请求失败
        std::cout << "Login error." << std::endl; //提示登录错误
    } else { //如果成功
        if (reply->readAll() == "<!--post ver:1.0.0 -->\n") { //如果有返回值
            std::cout << "Login successful." << std::endl; //提示登录成功
        } else { //如果没有
            std::cout << "Identity or password error." << std::endl; //提示账号密码错误
        }
    }

    emit quit(); //触发关闭程序的信号
    return; //跳出
}

//private
bool AutoLogin::getConnectionState() { //测试网络连通性
    QTcpSocket tcpClient; //TCP客户端
    tcpClient.abort(); //终止之前的连接并复位
    tcpClient.connectToHost("202.108.22.5", 80); //连接百度ip, 80端口
    if (!tcpClient.waitForConnected(512)) { //如果512毫秒之内不能连接百度
        return (false); //返回false
    }
    return (true); //如果连接成功, 返回true
}

void AutoLogin::initGetUrl() { //初始化获取url
    QNetworkAccessManager* manager = new QNetworkAccessManager(); //实例化网络访问类
    connect (manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(onReplyFinished(QNetworkReply*))); //关联信号和槽
    manager->get(QNetworkRequest(QUrl("http://202.108.22.5"))); //访问百度的服务器
}

bool AutoLogin::getLoginState(QString html) { //获取是否登录信息
    QRegExp* reg = new QRegExp("<title>(.*)</title>"); //匹配<title>标签
    qint8 con = reg->indexIn(html); //获取匹配位置, 若无匹配返回-1
    if (con != -1) { //如果有匹配
        if (reg->cap(1) == "百度一下，你就知道") { //如果匹配值为百度标题
            return (true); //返回true
        }
    }
    return (false); //若无匹配或标题不为"百度一下,你就知道", 返回false
}

QString AutoLogin::getSessionUrl(QString html) { //获取"用来获取Session"的Url
    QRegExp* reg = new QRegExp("self\.location\.href='(.*)'"); //匹配跳转目的页面
    qint8 con = reg->indexIn(html); //获取匹配位置, 若无匹配返回-1
    if (con != -1) { //如果有匹配
        return (reg->cap(1)); //返回匹配到的url
    }
    return (""); //如果没匹配则返回空字符串
}

void AutoLogin::initGetCookies(QString url) { //初始化获取cookie
    QNetworkAccessManager* manager = new QNetworkAccessManager(); //实例化网络访问类
    cookiejar = new NetworkCookie(); //cookie容器
    manager->setCookieJar(cookiejar); //设置网络访问类的cookie容器
    connect (manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(onCookieFinished(QNetworkReply*))); //关联信号和槽
    manager->get(QNetworkRequest(QUrl(url))); //访问url
}

void AutoLogin::initLogin() { //初始化登录
    QNetworkAccessManager* manager = new QNetworkAccessManager(); //实例化网络访问类
    manager->setCookieJar(cookiejar); //设置cookie为之前获取到的cookie
    connect (manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(onLoginFinished(QNetworkReply*))); //关联信号和槽
    QNetworkRequest* request = new QNetworkRequest(); //实例化网络请求
    request->setUrl(QUrl(LOGINURL)); //设置请求URL
    request->setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded"); //设置请求头
    QByteArray parameter; //post参数
    parameter.append(REGULAR.arg(Identity).arg(Password)); //构造post参数
    manager->post(*request, parameter); //发送post请求
}
