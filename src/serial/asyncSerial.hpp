#pragma once

#include "src/serial/SerialBase.hpp"
#include <boost/asio.hpp>

namespace cpptide::serial
{


class AsyncSerial : private boost::noncopyable
{
public:
    using parity_t         = boost::asio::serial_port_base::parity;
    using character_size_t = boost::asio::serial_port_base::character_size;
    using flow_control_t   = boost::asio::serial_port_base::flow_control;
    using stop_bits_t      = boost::asio::serial_port_base::stop_bits;

    struct SerialPortOptions
    {
        parity_t opt_parity             = parity_t(parity_t::none);
        character_size_t opt_csize      = character_size_t(8);
        flow_control_t opt_flow_control = flow_control_t(flow_control_t::none);
        stop_bits_t opt_stop_bits       = stop_bits_t(stop_bits_t::one);
    };

    // AsyncSerial();
    explicit AsyncSerial(const std::string &devname, unsigned int baud_rate, const SerialPortOptions &options = SerialPortOptions(), uint32_t readBufferSize = 1024);
    explicit AsyncSerial(const std::string &devname, unsigned int baud_rate,
                         parity_t opt_parity        = parity_t(parity_t::none),
                         character_size_t opt_csize = character_size_t(8),
                         flow_control_t opt_flow    = flow_control_t(flow_control_t::none),
                         stop_bits_t opt_stop       = stop_bits_t(stop_bits_t::one),
                         uint32_t readBufferSize    = 1024);

    ~AsyncSerial();

public:
    void open(const std::string &devname, unsigned int baud_rate,
              parity_t opt_parity        = parity_t(parity_t::none),
              character_size_t opt_csize = character_size_t(8),
              flow_control_t opt_flow    = flow_control_t(flow_control_t::none),
              stop_bits_t opt_stop       = stop_bits_t(stop_bits_t::one));

    void open(const std::string &devname, unsigned int baud_rate, const SerialPortOptions &options = SerialPortOptions());

    /// @brief Whether the serial port has been opened
    /// @return true: opened, false: not opened
    bool isOpen() const;

    /// @brief Whether the serial port has an error
    /// @return true: error, false: no error
    bool errorStatus() const;

    /// @brief Close the serial port
    void close();

    /// @brief Write data to the serial port, add it to the write cache, use io_.post() to add the task (doWrite) to the I/O service queue
    /// @param data written data [char *]
    /// @param size data size
    void write(const char *data, size_t size);

    /// @brief Write data to the serial port, add it to the write cache, use io_.post() to add the task (doWrite) to the I/O service queue
    /// @param data written data [std::vector<char>]
    void write(const std::vector<char> &data);

    /// @brief Write data to the serial port, add it to the write cache, use io_.post() to add the task (doWrite) to the I/O service queue
    /// @param s written data [std::string]
    void writeString(const std::string &s);

    /// @brief Whether the serial port is read
    /// @param is_read Whether to read [true: read, false: not read]
    void setRead(const bool &is_read) { is_read_ = is_read; }

    /// @brief Whether the serial port is write
    /// @param is_write Whether to write [true: write, false: not write]
    void setWrite(const bool &is_write) { is_write_ = is_write; }

private:
    /// @brief Read data from serial port
    void doRead();

    /// Read completion callback, and start the next read
    void readEnd(const boost::system::error_code &error,
                 size_t bytes_transferred);

    /// @brief Write data to the serial port, called by the I/O queue thread
    void doWrite();

    /// @brief Write completion callback
    /// @param error [const boost::system::error_code &] error code
    void writeEnd(const boost::system::error_code &error, size_t size);

    /// @brief Close the serial port
    void doClose();

public:
    /// @brief Set error status
    /// @param e [bool] error status [true: error, false: no error]
    void setErrorStatus(bool e);

    /// @brief Set read completion callback
    /// @param callback read completion callback
    void setReadCallback(const std::function<void(const char *, size_t)> &callback);

    /// @brief Set write completion callback
    /// @param callback write completion callback
    void setWriteCompleteCallback(const std::function<void(size_t)> &callback);

    /// @brief Clear read completion callback
    void clearReadCallback();

    /// @brief Clear write completion callback
    void clearWriteCompleteCallback();

public:
    const uint32_t readBufferSize_;
    std::shared_ptr<AsyncSerialImpl> pimpl_;
    bool is_read_ { true };
    bool is_write_ { true };
};

}// namespace cpptide::serial
