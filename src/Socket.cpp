#include "Socket.hh"

#include <udt/udt.h>
#include <string>
#include <sstream>

#include "Exception.hh"

namespace pyudt4 {

Socket::Socket()
: descriptor_(0),
  domain_(0),
  type_(0),
  protocol_(0),
  is_valid_(0)
{
}

Socket::~Socket()
{
    // Close socket on destruction
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

const int& Socket::getDomain() const
{
    return domain_;
}

void Socket::setDomain(int domain)
{
    domain_ = domain;
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

const int& Socket::getIsValid() const
{
    return is_valid_;
}

void Socket::setIsValid(int is_valid)
{
    is_valid_ = is_valid;
}

std::string Socket::str() const
{
    std::stringstream ss;
    ss << "------------" << std::endl
       << "Socket:   " << descriptor_ << std::endl
       << "Domain:   " << domain_     << std::endl
       << "Type:     " << type_       << std::endl
       << "Protocol: " << protocol_   << std::endl
       << "------------" << std::endl;
    return ss.str();
}

} // namespace pyudt4
