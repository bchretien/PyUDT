#ifndef __PYUDT_SOCKET_HH_
#define __PYUDT_SOCKET_HH_

#include <boost/python.hpp>
#include <boost/tuple/tuple.hpp>
#include <string>
#include <udt/udt.h>
#include <map>

#include "Memory.hh"

namespace py = boost::python;

namespace pyudt4 {

// Forward declarations
class Socket;

// For simplicity
typedef shared_ptr<Socket> Socket_ptr;

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
     * @param descriptor descriptor of the UDT socket.
     * @param close_on_delete whether to close the socket when object is
     * destroyed. Default = false.
     */
    Socket(UDTSOCKET descriptor, bool close_on_delete = false);

    /**
     * Destructor.
     */
    ~Socket();

    /**
     * Close the UDT socket.
     */
    void close();

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

    /**
     * Read a certain amount of data into a local memory buffer.
     * @param buf memory buffer used to store the received data.
     * @param buf_len length of the buffer.
     */
    void recv(char* buf, int buf_len) const throw();

    /**
     * Read a certain amount of data into a local memory buffer.
     * @param buf_len length of the buffer.
     * @return Python object containing the received data.
     */
    py::str recv(int buf_len) const throw();

    /**
     * Send a certain amount of data from an application buffer.
     * @param buf buffer of data to be sent.
     * @param buf_len length of the data to send.
     */
    void send(const char* buf, int buf_len) const throw();

    /**
     * Send a string.
     * @param str string to send.
     */
    void send(std::string str) const throw();

    /**
     * Send a certain amount of data from an application buffer.
     * @param py_data Python object contained the buffer of the data to be sent.
     */
    void send(boost::python::object py_data) const throw();

    /**
     * Bind a UDT socket to a known or an available local address.
     * @param ip IP address.
     * @param port port.
     */
    void bind(const char* ip, uint16_t port) throw();

    /**
     * Bind a UDT socket to a known or an available local address.
     * @param py_address Python tuple containing the address and the port.
     */
    void bind(boost::python::object py_address) throw();

    /**
     * Bind to an existing UDP socket.
     * @param udp_socket UDP socket to bind to.
     */
    void bind_to_udp(SYSSOCKET udp_socket) throw();

    /**
     * Enable a server UDT entity to wait for clients to connect.
     * @param backlog maximum number of pending connections.
     */
    void listen(unsigned int backlog) throw();

    /**
     * Connect to a server socket (in regular mode) or a peer socket
     * (in rendez-vous mode) to set up a UDT connection.
     * @param ip IP address.
     * @param port port.
     */
    void connect(const char* ip, uint16_t port) throw();

    /**
     * Connect to a server socket (in regular mode) or a peer socket
     * (in rendez-vous mode) to set up a UDT connection.
     * @param py_address Python tuple containing the address and the port.
     */
    void connect(boost::python::object py_address) throw();

    /**
     * Retrieve an incoming connection.
     * @return a tuple containing the socket of the incoming connection and its
     *  associated address/port.
     */
    boost::tuple<Socket_ptr, boost::tuple<const char*,uint16_t> >
    accept() throw();

private:
    /**
     * Build the structure containing the socket IP address, port, address
     * family etc.
     * @param ip IP address.
     * @param port port.
     * @param family address family (AF_INET or AF_INET6).
     */
    sockaddr_in build_sockaddr_in(const char* ip, uint16_t port,
                                  short family = AF_INET);

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

    /**
     * Whether the socket is alive.
     */
    bool is_alive_;
};

} // namespace pyudt4

#endif // __PYUDT_SOCKET_HH_
