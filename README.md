# 简介
利用OpenSSL完成安全通信的密钥协商服务器和客户端

# 客户端功能
1. 密钥协商：通过非对称加密实现服务器的公钥分发。
2. 共享内存：将服务器公钥写入共享内存，供其他进程使用

# 服务器功能
1. 生成对称加密公钥
2. 公钥写入内存和数据库

# Socket编程
1. 实现了多路IO转接+多线程
2. 利用多路IO转接函数实现了连接超时，发送超时，接收超时，监听超时功能
3. 使用Protobuf实现跨平台数据传输

# 使用
1. 这是一个基于Linux平台的项目
2. 需要在VS中编译，然后在Linux平台下使用
3. 用到的库包括openssl，protobuf，jsoncpp，mysql。需要在linux环境下做好安装配置


