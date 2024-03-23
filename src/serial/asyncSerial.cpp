#include "src/serial/asyncSerial.hpp"

#include <boost/bind.hpp>
#include <boost/shared_array.hpp>
#include <boost/functional.hpp>
#include <boost/thread.hpp>
#include <mutex>
#include <string>

// using namespace boost::placeholders;

AsyncSerial::AsyncSerial()
{
	pimpl_ = std::make_shared<AsyncSerialImpl>();
}

AsyncSerial::AsyncSerial(const std::string &devname, unsigned int baud_rate,
						 parity_t opt_parity, character_size_t opt_csize,
						 flow_control_t opt_flow, stop_bits_t opt_stop)
{
	pimpl_ = std::make_shared<AsyncSerialImpl>();
	open(devname, baud_rate, opt_parity, opt_csize, opt_flow, opt_stop);
}

void AsyncSerial::open(const std::string &devname, unsigned int baud_rate,
					   parity_t opt_parity, character_size_t opt_csize,
					   flow_control_t opt_flow, stop_bits_t opt_stop)
{
	if (isOpen())
		close();

	setErrorStatus(true); // If an exception is thrown, error_ remains true
	pimpl_->port_.open(devname);
	pimpl_->port_.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
	pimpl_->port_.set_option(opt_parity);
	pimpl_->port_.set_option(opt_csize);
	pimpl_->port_.set_option(opt_flow);
	pimpl_->port_.set_option(opt_stop);

	// This gives some work to the io_service before it is started
	pimpl_->io_.post(boost::bind(&AsyncSerial::doRead, this));

	std::thread t(boost::bind(&boost::asio::io_service::run, &pimpl_->io_));
	pimpl_->backgroundThread_.swap(t);
	setErrorStatus(false); // If we get here, no error
	pimpl_->open_ = true;  // Port is now open
}

bool AsyncSerial::isOpen() const
{
	return pimpl_->open_;
}

bool AsyncSerial::errorStatus() const
{
	std::lock_guard<std::mutex> l(pimpl_->errorMutex_);
	return pimpl_->error_;
}

void AsyncSerial::close()
{
	if (!isOpen())
		return;

	pimpl_->open_ = false;
	pimpl_->io_.post(boost::bind(&AsyncSerial::doClose, this));
	pimpl_->backgroundThread_.join();
	pimpl_->io_.reset();
	if (errorStatus())
	{
		throw(boost::system::system_error(boost::system::error_code(),
										  "Error while closing the device"));
	}
}

void AsyncSerial::write(const char *data, size_t size)
{
	{
		std::lock_guard<std::mutex> l(pimpl_->writeQueueMutex_);
		pimpl_->writeQueue_.insert(pimpl_->writeQueue_.end(), data, data + size);
	}
	pimpl_->io_.post(boost::bind(&AsyncSerial::doWrite, this));
}

void AsyncSerial::write(const std::vector<char> &data)
{
	{
		std::lock_guard<std::mutex> l(pimpl_->writeQueueMutex_);
		pimpl_->writeQueue_.insert(pimpl_->writeQueue_.end(), data.begin(), data.end());
	}
	pimpl_->io_.post(boost::bind(&AsyncSerial::doWrite, this));
}

void AsyncSerial::writeString(const std::string &s)
{
	{
		std::lock_guard<std::mutex> l(pimpl_->writeQueueMutex_);
		pimpl_->writeQueue_.insert(pimpl_->writeQueue_.end(), s.begin(), s.end());
	}
	pimpl_->io_.post(boost::bind(&AsyncSerial::doWrite, this));
}

AsyncSerial::~AsyncSerial()
{
	if (isOpen())
	{
		try
		{
			close();
		}
		catch (...)
		{
			// Don't throw from a destructor
		}
	}
}

void AsyncSerial::doRead()
{
	pimpl_->port_.async_read_some(boost::asio::buffer(pimpl_->readBuffer, readBufferSize_),
								  boost::bind(&AsyncSerial::readEnd, this,
											  boost::asio::placeholders::error,
											  boost::asio::placeholders::bytes_transferred));
}

void AsyncSerial::readEnd(const boost::system::error_code &error,
						  size_t bytes_transferred)
{

	if (error)
	{
		/// 如果读操作错误, 则关闭串口, 设置错误
		if (isOpen())
		{
			doClose();
			setErrorStatus(true);
		}
	}
	else
	{
		/// 如果读取成功, 执行回调并继续监听读
		if (pimpl_->read_callback)
		{
			pimpl_->read_callback(pimpl_->readBuffer, bytes_transferred);
		}
		// 继续读
		doRead();
	}
}

void AsyncSerial::doWrite()
{
	// 如果写入操作已在进行中，则不执行任何操作
	if (pimpl_->writeBuffer_ == 0)
	{
		std::lock_guard<std::mutex> l(pimpl_->writeQueueMutex_);
		pimpl_->writeBufferSize_ = pimpl_->writeQueue_.size();
		pimpl_->writeBuffer_.reset(new char[pimpl_->writeQueue_.size()]);
		std::copy(pimpl_->writeQueue_.begin(), pimpl_->writeQueue_.end(),
				  pimpl_->writeBuffer_.get());
		pimpl_->writeQueue_.clear();
		boost::asio::async_write(pimpl_->port_, boost::asio::buffer(pimpl_->writeBuffer_.get(), pimpl_->writeBufferSize_),
								 boost::bind(&AsyncSerial::writeEnd, this, boost::asio::placeholders::error));
	}
}

void AsyncSerial::writeEnd(const boost::system::error_code &error)
{
	if (!error)
	{
		std::lock_guard<std::mutex> l(pimpl_->writeQueueMutex_);
		if (pimpl_->writeQueue_.empty())
		{
			pimpl_->writeBuffer_.reset();
			pimpl_->writeBufferSize_ = 0;

			return;
		}
		pimpl_->writeBufferSize_ = pimpl_->writeQueue_.size();
		pimpl_->writeBuffer_.reset(new char[pimpl_->writeQueue_.size()]);
		std::copy(pimpl_->writeQueue_.begin(), pimpl_->writeQueue_.end(),
				  pimpl_->writeBuffer_.get());
		pimpl_->writeQueue_.clear();
		boost::asio::async_write(pimpl_->port_, boost::asio::buffer(pimpl_->writeBuffer_.get(), pimpl_->writeBufferSize_),
								 boost::bind(&AsyncSerial::writeEnd, this, boost::asio::placeholders::error));
	}
	else
	{
		setErrorStatus(true);
		doClose();
	}
}

void AsyncSerial::doClose()
{
	boost::system::error_code ec;
	// 将所有未完成的异步读取或写入操作立即完成
	pimpl_->port_.cancel(ec);
	if (ec)
		setErrorStatus(true);
	// 关闭串口。任何异步读取或写入操作都将立即取消
	pimpl_->port_.close(ec);
	if (ec)
		setErrorStatus(true);
}

void AsyncSerial::setErrorStatus(bool e)
{
	std::lock_guard<std::mutex> l(pimpl_->errorMutex_);
	pimpl_->error_ = e;
}

void AsyncSerial::setReadCallback(const std::function<void(const char *, size_t)> &callback)
{
	pimpl_->read_callback = callback;
}

void AsyncSerial::clearReadCallback()
{
	std::function<void(const char *, size_t)> empty;
	pimpl_->read_callback.swap(empty);
}