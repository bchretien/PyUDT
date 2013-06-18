#ifndef __PYUDT_SOCKET_HH_
#define __PYUDT_SOCKET_HH_

#include <boost/python.hpp>
#include <string>
#include <udt/udt.h>
#include <map>

namespace pyudt4 {

/**
 * C++ wrapper for a UDT socket.
 */
class Socket
{
public:

    /**
     * Default constructor.
     */
    Socket();

    /**
     * Construct socket object from an existing UDT socket descriptor.
     */
    Socket(UDTSOCKET descriptor, bool close_on_delete = true);

    /**
     * Destructor.
     */
    ~Socket();

    /**
     * Return the socket descriptor.
     */
    const UDTSOCKET& getDescriptor() const;

    /**
     * Set the socket descriptor.
     */
    void setDescriptor(UDTSOCKET descriptor);

    /**
     * Return the address family of the socket.
     */
    const int& getAddressFamily() const;

    /**
     * Set the address family domain: UDT::AF_INET or UDT::AF_INET6.
     */
    void setAddressFamily(int addr_family);

    /**
     * Return the type of the socket.
     */
    const int& getType() const;

    /**
     * Set the socket type: UDT::SOCK_STREAM or UDT::SOCK_DGRAM.
     */
    void setType(int type);

    /**
     * Return the protocol of the socket.
     */
    const int& getProtocol() const;

    /**
     * Set the socket protocol.
     */
    void setProtocol(int protocol);

    /**
     * Get whether the socket should be closed when destructor is called.
     */
    const bool& getCloseOnDelete() const;

    /**
     * Set whether the socket should be closed when destructor is called.
     */
    void setCloseOnDelete(bool close_on_delete);

    /**
     * Put the socket's information in a string.
     */
    std::string str() const;

private:
    /**
     * UDT socket descriptor.
     */
    UDTSOCKET descriptor_;

    /**
     * IP address family: UDT::AF_INET or UDT::AF_INET6.
     */
    int addr_family_;

    /**
     * Type of the socket: UDT::SOCK_STREAM or UDT::SOCK_DGRAM.
     */
    int type_;

    /**
     * Protocol to be used.
     */
    int protocol_;

    /**
     * Whether to close the socket when the destructor is called.
     */
    bool close_on_delete_;
};

} // namespace pyudt4

#endif // __PYUDT_SOCKET_HH_
