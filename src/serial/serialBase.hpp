#pragma once

#include <boost/asio.hpp>
#include <boost/shared_array.hpp>
#include <boost/utility.hpp>
#include <functional>
#include <memory>
#include <vector>

namespace cpptide::serial
{

class AsyncSerialImpl : private boost::noncopyable
{
public:
    AsyncSerialImpl() : io_(), port_(io_), backgroundThread_(), open_(false),
                        error_(false) {}

    boost::asio::io_service io_;   // Io service object
    boost::asio::serial_port port_;// Serial port object
    std::thread backgroundThread_; // Thread that runs read/write operations
    bool open_;                    // True if port open
    bool error_;                   // Error flag
    mutable std::mutex errorMutex_;// Mutex for access to error

    /// Data are queued here before they go in writeBuffer
    std::vector<char> writeQueue_;
    boost::shared_array<char> writeBuffer_;// Data being written
    size_t writeBufferSize_;               // Size of writeBuffer
    std::mutex writeQueueMutex_;           // Mutex for access to writeQueue
    char readBuffer[1024];                 // data being read

    // Read complete callback
    std::function<void(const char *, size_t)> read_callback;
    // Write complete callback
    std::function<void(size_t)> write_complete_callback;
};

}// namespace cpptide::serial