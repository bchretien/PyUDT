#ifndef __PYUDT_SOCKET_HH_
#define __PYUDT_SOCKET_HH_

#include <boost/python.hpp>
#include <string>
#include <udt/udt.h>
#include <map>

namespace pyudt4 {

/**
 * Class describing a UDT socket.
 */
class Socket
{
public:

    /**
     * Default constructor.
     */
    Socket();

    /**
     * Destructor.
     */
    ~Socket();

    /**
     * Return the descriptor of the socket.
     */
    const UDTSOCKET& getDescriptor() const;

    /**
     * Set the socket descriptor.
     */
    void setDescriptor(UDTSOCKET descriptor);

    /**
     * Return the domain of the socket.
     */
    const int& getDomain() const;

    /**
     * Set the socket domain.
     */
    void setDomain(int domain);

    /**
     * Return the type of the socket.
     */
    const int& getType() const;

    /**
     * Set the socket type.
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
     * Return whether the socket is valid.
     */
    const int& getIsValid() const;

    /**
     * Set whether the socket is valid.
     */
    void setIsValid(int is_valid);

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
     * UDT socket family.
     */
    int domain_;

    /**
     * UDT socket type.
     */
    int type_;

    /**
     * UDT socket protocol.
     */
    int protocol_;

    /**
     * Whether the UDT socket is valid.
     */
    int is_valid_;
};

} // namespace pyudt4

#endif // __PYUDT_SOCKET_HH_
