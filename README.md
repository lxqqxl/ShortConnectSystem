# ShortConnectSystem

1. ET和LT模式解决：防止多次触发，导致一直回显
2. Location地址不跳转问题：跳转地址必须要完全输入如：https://www.baidu.com
3. 跳转地址成功以后进程异常终止：必须更改当前fd的状态，由可写变为可读，或者直接关闭，然后重Epoll里面剔除，由于这是短链接系统，采用直接剔除，没有改变状态，但是进程为什么会退出，这一点还是不清楚
4. epoll_ctl(epollfd, EPOLL_CTL_DEL,  fd, &ev)出错，就是在剔除文件fd的时候必须先在epoll里面剔除，在close掉，不然会报错，不是有效的一个fd
5. 注意，mysql的字段不能出现关键字，如key这些不然无法正常操作，虽然可以建表
6. 在操作mysql的时候，对于varchar类型，查找的时候必须要加“”，不然无法正常操作，这是一个小细节，例如：select * from short_connect where short_key = "xianqi";
7. 发布订阅模式
8. 