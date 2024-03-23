#pragma once

#include "src/serial/serialBase.hpp"
#include <boost/asio.hpp>

class AsyncSerial : private boost::noncopyable
{
public:
	typedef boost::asio::serial_port_base::parity parity_t;
	typedef boost::asio::serial_port_base::character_size character_size_t;
	typedef boost::asio::serial_port_base::flow_control flow_control_t;
	typedef boost::asio::serial_port_base::stop_bits stop_bits_t;

	AsyncSerial();
	AsyncSerial(const std::string &devname, unsigned int baud_rate,
				parity_t opt_parity = parity_t(parity_t::none),
				character_size_t opt_csize = character_size_t(8),
				flow_control_t opt_flow = flow_control_t(flow_control_t::none),
				stop_bits_t opt_stop = stop_bits_t(stop_bits_t::one));

	virtual ~AsyncSerial() = 0;

public:
	void open(const std::string &devname, unsigned int baud_rate,
			  parity_t opt_parity = parity_t(parity_t::none),
			  character_size_t opt_csize = character_size_t(8),
			  flow_control_t opt_flow = flow_control_t(flow_control_t::none),
			  stop_bits_t opt_stop = stop_bits_t(stop_bits_t::one));

	/// @brief 是否已经打开串口
	/// @return true: 已经打开, false: 未打开
	bool isOpen() const;

	/// @brief 是否发送错误
	/// @return
	bool errorStatus() const;

	/// @brief 关闭串口
	void close();

	/// @brief 向串口写数据, 添加到写缓存, 使用 io_.post() 将任务（doWrite）添加到 I/O 服务队列
	/// @param data 写入的数据char *
	/// @param size 数据长度
	void write(const char *data, size_t size);

	/// @brief 向串口写数据, 添加到写缓存, 使用 io_.post() 将任务（doWrite）添加到 I/O 服务队列
	/// @param data 写入的数据vector
	void write(const std::vector<char> &data);

	/// @brief 向串口写数据, 添加到写缓存, 使用 io_.post() 将任务（doWrite）添加到 I/O 服务队列
	/// @param s: 写入的数据string
	void writeString(const std::string &s);

private:
	/// @brief 向串口读数据
	void doRead();

	/// 读完成回调, 以及开启下一次读
	void readEnd(const boost::system::error_code &error,
				 size_t bytes_transferred);

	/// @brief 向串口写数据, 由 I/O 队列线程调用
	void doWrite();

	/// @brief 写完成回调
	/// @param error
	void writeEnd(const boost::system::error_code &error);

	/// @brief 关闭串口
	void doClose();

protected:
	/// @brief 设置错误
	/// @param e 错误
	void setErrorStatus(bool e);

	/// @brief 设置读完成回调
	/// @param callback
	void setReadCallback(const std::function<void(const char *, size_t)> &callback);

	/// @brief 清除读完成回调
	void clearReadCallback();

public:
	static const int readBufferSize_ = 512;
	std::shared_ptr<AsyncSerialImpl> pimpl_;
};
