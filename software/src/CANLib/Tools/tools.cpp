#include <cassert>
#include <cstdint>
#include <tools.hpp>

// Mask for can errors
bool CAN::isError(const can_frame& frame) {
    return frame.can_id & CAN_ERR_FLAG;
}

// Mask for RTRs
bool CAN::isRemoteTransmissionRequest(const can_frame& frame) {
    return frame.can_id & CAN_RTR_FLAG;
}

// Determines the format of the CAN frame
CanFormat CAN::frameFormat(const can_frame& frame) {
    if (frame.can_id & CAN_EFF_FLAG) {
        return CanFormat::Extended;
    }
    return CanFormat::Standard;
}

// Masks for the CAN frame ID
uint32_t CAN::frameId(const can_frame& frame) {
    switch (frameFormat(frame)) {
    case CanFormat::Standard:
        return frame.can_id & CAN_SFF_MASK; // 11 LSB
    case CanFormat::Extended:
        return frame.can_id & CAN_EFF_MASK; // 29 LSB
    default:
        break;
    }
    assert(false); // Only 2 formats supported
    return 0;
}