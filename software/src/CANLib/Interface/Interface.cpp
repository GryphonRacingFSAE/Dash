#include <cassert>
#include <cerrno>
#include <cstring>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <Interface.hpp>

using namespace CAN;

RetCode Interface::startReceiving(const char* canbus_interface_name, can_filter* filters, const size_t num_of_filters, uint32_t read_timeout_ms) {
    if (this->openSocket(canbus_interface_name, filters, num_of_filters, read_timeout_ms) != RetCode::Success) {
        return RetCode::SocketErr;
    }

    m_reading_thread = std::thread(&Interface::readLoop, this);
    return RetCode::Success;
}

void Interface::readLoop() {
    can_frame frame = {};
    while (!this->m_should_exit) {
        switch (this->read(frame)) {
        case RetCode::Success:
            this->newFrame(frame);
            break;
        case RetCode::Timeout:
            this->newTimeout();
            break;
        default:
            this->newError(frame);
            break;
        }
    }
    this->m_should_exit = false;
}

void Interface::stopReceiving() {
    m_should_exit = true;
    if (m_reading_thread.joinable()) {
        m_reading_thread.join();
    }
    ::close(m_socket);
}

RetCode Interface::openSocket(const char* canbus_interface_name, can_filter* filters, size_t filter_count, uint32_t read_timeout_ms) {
    m_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (m_socket < 0) {
        return RetCode::SocketErr;
    }
    ifreq ifr;

    strncpy(ifr.ifr_name, canbus_interface_name, IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';
    ioctl(m_socket, SIOCGIFINDEX, &ifr);

    if (filter_count > 0) {
        if (setsockopt(m_socket, SOL_CAN_RAW, CAN_RAW_FILTER, filters, filter_count * sizeof(can_filter)) < 0) {
            return RetCode::SocketErr;
        }
    }

    if (read_timeout_ms != 0) {
        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = read_timeout_ms * 1000;

        if (setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (void*)&tv, sizeof(timeval)) < 0) {
            return RetCode::SocketErr;
        }
    }

    sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(m_socket, (sockaddr*)(&addr), sizeof(addr)) < 0) {
        return RetCode::SocketErr;
    }
    return RetCode::Success;
}

RetCode Interface::read(can_frame& frame) {
    // Read in a CAN frame
    auto num_bytes = ::read(m_socket, &frame, sizeof(frame));
    if (num_bytes == -1) {     // Error during read
        if (errno == EAGAIN) { // ::read timed out
            return RetCode::Timeout;
        }
        return RetCode::ReadErr;
    }

    if (num_bytes != sizeof(frame)) { // Not all bytes were retrieved
        return RetCode::ReadErr;
    }
    return RetCode::Success;
}

Interface::~Interface() {
    this->stopReceiving();
}