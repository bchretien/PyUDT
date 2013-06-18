#include "Socket.hh"

#include <udt/udt.h>
#include <string>
#include <sstream>

#include "Exception.hh"

namespace pyudt4 {

Socket::Socket()
: descriptor_(0),
  addr_family_(AF_INET),
  type_(SOCK_STREAM),
  protocol_(0),
  close_on_delete_(true)
{
    // Create a UDT socket
    descriptor_ = UDT::socket(addr_family_, type_, protocol_);

    // Catch a possible error
    if (descriptor_ == UDT::INVALID_SOCK)
    {
        translateUDTError();
        return;
    }

    // TODO: set default socket options
}

Socket::Socket(UDTSOCKET descriptor, bool close_on_delete)
: descriptor_(descriptor),
  addr_family_(0),
  type_(0),
  protocol_(0),
  close_on_delete_(close_on_delete)
{
    // TODO: find a way to get the address family, type and protocol from an
    //       existing UDT socket.
}

Socket::~Socket()
{
    // TODO: make sure that the socket is no longer referenced in an epoll

    // Close socket on destruction
    if (close_on_delete_)
        if (UDT::ERROR == UDT::close(descriptor_))
        {
            translateUDTError();
            return;
        }
}

const UDTSOCKET& Socket::getDescriptor() const
{
    return descriptor_;
}

void Socket::setDescriptor(UDTSOCKET descriptor)
{
    descriptor_ = descriptor;
}

const int& Socket::getAddressFamily() const
{
    return addr_family_;
}

void Socket::setAddressFamily(int addr_family)
{
    addr_family_ = addr_family;
}

const int& Socket::getType() const
{
    return type_;
}

void Socket::setType(int type)
{
    type_ = type;
}

const int& Socket::getProtocol() const
{
    return protocol_;
}

void Socket::setProtocol(int protocol)
{
    protocol_ = protocol;
}

const bool& Socket::getCloseOnDelete() const
{
    return close_on_delete_;
}

void Socket::setCloseOnDelete(bool close_on_delete)
{
    close_on_delete_ = close_on_delete;
}

std::string Socket::str() const
{
    std::stringstream ss;
    ss << "--------------" << std::endl
       << "Socket:       " << descriptor_  << std::endl
       << "Addr family:  " << addr_family_ << std::endl
       << "Type:         " << type_        << std::endl
       << "Protocol:     " << protocol_    << std::endl
       << "--------------" << std::endl;
    return ss.str();
}

} // namespace pyudt4
