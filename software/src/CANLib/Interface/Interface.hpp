#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <thread>

#include <defs.hpp>

namespace CAN {

class Interface {
  public:
    Interface() = default;
    virtual ~Interface();

  protected:
    RetCode startReceiving(const char* canbus_interface_name, can_filter* filters, const size_t num_of_filters, uint32_t read_timeout_ms);
    void stopReceiving();

    // As of now, newFrame, newError and newTimeout should be extremely fast functions before
    // emitting more signals
    virtual void newFrame(const can_frame&) = 0;
    virtual void newError(const can_frame&) = 0;
    virtual void newTimeout() = 0;

    template <size_t N>
    RetCode write(uint32_t address, const std::array<uint8_t, N>& payload, bool extended = false) {
        static_assert(0 <= N && N <= 8, "Size of payload must be between 0 and 8");
        if (address & 0xE0000000) { // Disallow passing anything but the address.
            return RetCode::InvalidParam;
        }

        if ((address & 0x1FFFF800) && !extended) { // Check if there are any bits in the extended section.
            return RetCode::InvalidParam;
        }

        if (extended) {
            address |= CAN_EFF_FLAG; // Set extended format.
        }

        can_frame frame;
        frame.can_id = address;
        frame.len = N;
        memcpy(frame.data, payload.data(), N);

        if (::write(m_socket, (void*)&frame, sizeof(frame)) != sizeof(frame)) {
            return RetCode::WriteErr;
        }

        return RetCode::Success;
    }

    RetCode write(uint32_t address, const uint8_t* payload, const uint8_t count, bool extended = false) {
        if (count > 8 || count < 1) { // Force count to be a good value
            return RetCode::InvalidParam;
        }

        if (address & 0xE0000000) { // Disallow passing anything but the address.
            return RetCode::InvalidParam;
        }

        if ((address & 0x1FFFF800) && !extended) { // Check if there are any bits in the extended section.
            return RetCode::InvalidParam;
        }

        if (extended) {
            address |= CAN_EFF_FLAG; // Set extended format.
        }

        can_frame frame;
        frame.can_id = address;
        frame.len = count;
        memcpy(frame.data, payload, count);

        if (::write(m_socket, (void*)&frame, sizeof(frame)) != sizeof(frame)) {
            return RetCode::WriteErr;
        }

        return RetCode::Success;
    }

  private:
    RetCode openSocket(const char* canbus_interface_name, can_filter* filters, size_t filter_count, uint32_t read_timeout_ms);
    RetCode read(can_frame& frame);
    void readLoop();

  private:
    int32_t m_socket = -1;

    std::thread m_reading_thread;
    std::atomic<bool> m_should_exit = false;
};

} // namespace CAN