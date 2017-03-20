#include <QCoreApplication> //Qt核心
#include <autologin.h> //自己写的类库

int main(int argc, char *argv[]) //主函数
{
    QCoreApplication a(argc, argv); //Qt核心类库

    AutoLogin *al = new AutoLogin(); //实例化自己写的类库
    QObject::connect(al, SIGNAL(quit()), //信号是我类库中的quit()信号
                     &a, SLOT(quit())); //槽是Qt核心退出
    al->setIdentity(argv[1]); //把用户名作为第一个参数传入
    al->setPassword(argv[2]); //把密码作为第二个参数传入
    al->autoLogin(); //开始登录

    return a.exec(); //进入Qt事件循环
}
