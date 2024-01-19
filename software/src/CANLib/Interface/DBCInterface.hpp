#pragma once

#include <dbcppp/Network.h>

#include <Interface.hpp>
#include <fmt/core.h>
#include <functional>
#include <memory>
#include <tools.hpp>

namespace CAN {

// A way to interface with DBC files and map them to functions defined by child classes
template <class CANNode>
class DBCInterface : public CAN::Interface {
  public:
    // Load a dbc file and start the CAN interface using settings from child classes
    DBCInterface(const std::string& dbc_file, const char* interface_name = "can0")
        : dbc_network(std::move(dbcppp::INetwork::LoadNetworkFromFile(dbc_file)[""])) {

        if (dbc_network == nullptr) {
            fmt::print("DBC ({}) and CAN interface unavailable.\n", dbc_file);
            return;
        }

        // Startup HW interface
        this->CAN::Interface::startReceiving(interface_name, CANNode::filters, CANNode::num_of_filters, CANNode::timeout_ms);

        fmt::print("Loaded DBC: {}\n", dbc_file);
        for (const dbcppp::IMessage& msg : dbc_network->Messages()) {
            fmt::print("\tLoaded Message: {}\n", msg.Name());
            can_messages.insert(std::make_pair(msg.Id(), &msg)); // Insert mapping between ID and message info
            for (const dbcppp::ISignal& sig : msg.Signals()) {
                fmt::print("\t\tLoaded Signal: {} ({})\n", sig.Name(), sig.Unit());
            }
        }
    }
    ~DBCInterface() = default;

  private:
    // If a child class defines a mapping between a dispatch function and the corresponding DBC
    // message & signal, dispatch it
    void newFrame(const can_frame& frame) override {
        const auto& message = can_messages.find(frame.can_id);
        if (message == can_messages.end())
            return; // Could not find decoding logic for message in the provided DBC

        // Dispatch for entire message if requested
        const auto message_dispatch_func_iter = can_message_dispatch.find(CAN::frameId(frame));
        if (message_dispatch_func_iter != can_message_dispatch.end()) {
            // Dispatch CAN frame with decoder
            std::invoke(message_dispatch_func_iter->second, dynamic_cast<CANNode*>(this), message->second, frame);
        }

        for (const dbcppp::ISignal& sig : message->second->Signals()) {
            const auto signal_dispatch_func_iter = can_signal_dispatch.find(sig.Name());
            if (signal_dispatch_func_iter != can_signal_dispatch.end()) {
                // Dispatch decoded CAN frame
                std::invoke(signal_dispatch_func_iter->second, dynamic_cast<CANNode*>(this), sig.RawToPhys(sig.Decode(frame.data)));
            }
        }
    }
    void newError(const can_frame&) override {
        fmt::print("Error\n");
    }
    void newTimeout() override{};

  protected:
    std::unique_ptr<dbcppp::INetwork> dbc_network;
    std::unordered_map<uint64_t, const dbcppp::IMessage*> can_messages;            // Mapping between CAN ID and DBC Message info
    std::unordered_map<std::string, void (CANNode::*)(float)> can_signal_dispatch; // Mapping between signal name and a member function of a child
                                                                                   // class which takes a float
    std::unordered_map<uint64_t, void (CANNode::*)(const dbcppp::IMessage*, const can_frame&)>
        can_message_dispatch; // This is if you want to deal with non-float signals or deal with entire messages
};

} // namespace CAN