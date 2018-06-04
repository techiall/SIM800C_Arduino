// SIM800C.h

#ifndef _SIM800C_h
#define _SIM800C_h


#include <Arduino.h>
#include <SoftwareSerial.h>


#define DEFAULT_TIMEOUT 1000

class SIM800C
{
	SoftwareSerial sim;


	bool find_result(const String buffer,
	                 const char *res1, const char *res2 = "");
	/*
	 * 清理缓冲区，上一次条 at 指令发送完，可能有些没收到，保留在缓冲区内
	 */
	void clean_buffer();

public:
	/*
	 * 初始化，利用软串口配置 RX, TX，并设置波特率
	 */
	SIM800C(const int TX_PIN = 8, const int RX_PIN = 9,
	        const int baud_rate = 9600) : sim(TX_PIN, RX_PIN)
	{
		sim.begin(baud_rate);
	}

	String serial_event();

	void send_at_command(const char *at, const bool newline = true,
	                     const unsigned long delay_time = 3000);

	/*
	 * 调试 at 指令
	 * 测试配置串口后，是否可以正常接收，发送数据
	 */
	void debug();

	/*
	 * 重启模块 
	 */
	bool restart();

	/*
	 * tcp 连接初始化
	 * AT
	 * AT+IPR=9600
	 * AT&W
	 * AT+CPIN?
	 * AT+CSQ
	 * AT+CREG?
	 * AT+CGREG?
	 * AT+CAGTT=1
	 * AT+CIPMODE=1
	 * AT+CSTT=\"CMNET\" 默认为 CMNET，后期优化为传参数
	 * "AT+CIICR
	 * AT+CIFSR
	 */
	bool tcp_link_init();

	bool create_http_request(const char *method, const char *ip,
	                         unsigned int port);

	bool http_send(const char *send);

	String create_http_request_message(const char *method,
	                                   const char *url, const char *version,
	                                   const char *head, const char *body = "");

	String get_at_result(const char *at, const bool newline = true,
	                     const unsigned long delay_time = 3000);
};

#endif

