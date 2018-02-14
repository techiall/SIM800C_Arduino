# SIM800C

TCP/IP 应用库

```c++
// 模块初始化	
bool init();
	
// 模块重启
bool restart();

// 获得 SIM 卡状态
bool sim_status();

// 网络注册状态
bool net_registration();

// 检查 GPRS 附着状态
bool gprs_status();

// 设置 APN
bool apn(const char *apn = "CMNET");

// 创建 TCP 链接
bool create_tcp(const char *ip, unsigned int port);

// 发送数据到远程服务
bool tcp_send(const char *send);

// 返回执行后的返回值
String send_result();

// 获得本地 IP 数值
unsigned long get_num_localip();
	
// 获得本地 IP 字符串
String get_str_localip();

// 设置 AT 指令回显
bool echo(bool flag); 

// 调试 AT 指令
void debug();

// 创建本地 TCP 服务器
bool create_tcp_server(unsigned int port);

// 设置链路模式
bool multi_link_mode(bool flag);

// 执行指令
unsigned int exec(const char *AT, unsigned int timeout = DEFAULT_TIMEOUT);

// 基站定位
bool base_station_position(double &longitude, double &latitude, unsigned int &precision);
```

