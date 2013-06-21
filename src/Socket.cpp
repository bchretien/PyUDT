#include "Socket.hh"

#include <udt/udt.h>
#include <string>
#include <sstream>
#include <arpa/inet.h> // inet_pton

#include "Exception.hh"
#include "Debug.hh"

namespace py = boost::python;

namespace pyudt4 {

sockaddr_in Socket::build_sockaddr_in(const char* ip, uint16_t port,
                              short family)
{
    // Socket address structure
    sockaddr_in addr;

    // Set address family
    addr.sin_family = family;

    // Set port
    addr.sin_port = htons(port);

    // Set IP address
    if (ip != nullptr)
    {
        inet_pton(AF_INET, ip, &addr.sin_addr);
    }
    else
    {
        addr.sin_addr.s_addr = INADDR_ANY;
    }

    // Set to zero
    memset(&(addr.sin_zero), '\0', 8);

    return addr;
}

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

    PYUDT_LOG_TRACE("Created UDT socket " << descriptor_);

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

    PYUDT_LOG_TRACE("Create Socket object from existing socket " << descriptor_);
}


Socket::~Socket()
{
    // TODO: make sure that the socket is no longer referenced in an epoll

    // Close socket on destruction
    if (close_on_delete_)
    {
        int res = UDT::close(descriptor_);

        // FIXME: currently ignore invalid socket errors
        // This happens when the socket destructor is called after the epoll
        // destructor.
        if (res == UDT::ERROR
            && UDT::getlasterror().getErrorCode() != CUDTException::EINVSOCK)
        {
            translateUDTError();
            return;
        }
        else PYUDT_LOG_TRACE("Closed UDT socket " << descriptor_);
    }

    PYUDT_LOG_TRACE("Destroyed UDT socket " << descriptor_);
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


void Socket::recv(char* buf, int buf_len) const throw()
{
    if (buf == nullptr)
    {
        throw Exception("Null buffer provided during Socket::recv", "");
    }

    int res;

    // Initialize the buffer to \0
    memset(buf, '\0', buf_len);

    // FIXME: useful python macros?
    Py_BEGIN_ALLOW_THREADS;
    res = UDT::recv(descriptor_, buf, buf_len, 0);
    Py_END_ALLOW_THREADS;

    if (res == UDT::ERROR)
    {
        translateUDTError();
        return;
    }

    PYUDT_LOG_TRACE("Received " << buf_len << " byte(s) from socket "
                    << descriptor_ << " that were stored in "
                    << static_cast<void *>(&buf));
}


py::str Socket::recv(int buf_len) const throw()
{
    int res;
    char* buf = (char*) malloc (buf_len * sizeof(char));

    // Initialize the buffer to \0
    memset(buf, '\0', buf_len);

    // FIXME: useful python macros?
    Py_BEGIN_ALLOW_THREADS;
    res = UDT::recv(descriptor_, buf, buf_len, 0);
    Py_END_ALLOW_THREADS;

    if (res == UDT::ERROR)
    {
        free(buf);
        translateUDTError();

        // None
        return py::str();
    }

    py::str py_buf = buf;
    free(buf);

    PYUDT_LOG_TRACE("Received " << buf_len << " byte(s) from socket "
                    << descriptor_ << " that were stored in "
                    << static_cast<void *>(&buf));

    return py_buf;
}


void Socket::send(const char* buf, int buf_len) const throw()
{
    if (buf == nullptr)
    {
        throw Exception("Null buffer provided during Socket::send", "");
    }

    int res;

    // FIXME: useful python macros?
    Py_BEGIN_ALLOW_THREADS;
    res = UDT::send(descriptor_, buf, buf_len, 0);
    Py_END_ALLOW_THREADS;

    if (res == UDT::ERROR)
    {
        translateUDTError();
        return;
    }

    PYUDT_LOG_TRACE("Sent " << buf_len << " byte(s) from socket "
                    << descriptor_ << " that were stored in "
                    << static_cast<void *>(&buf));
}


void Socket::send(py::object py_buf) const throw()
{
    // pointer to buffer
    char* buf = nullptr;
    // true buffer length
    int buf_len = 0;
    // passed in length size
    int pref_len = 0;

    // TODO: find a way to use Boost.Python instead
    if (!PyArg_ParseTuple(py_buf.ptr(), "s#ii", &buf, &buf_len, &pref_len))
    {
        Exception e("Wrong arguments: Socket::send((char*)buf, (int)buf_len)", "");
        translateException(e);
        throw e;
    }
    else
    {
        if ((pref_len - buf_len) > buf_len)
        {
            Exception e("Buffer length must not double real buffer length", "");
            translateException(e);
            throw e;

            return;
        }
    }

    if (buf == nullptr)
    {
        Exception e("Null buffer provided during Socket::send", "");
        translateException(e);
        throw e;
    }

    int res;

    // FIXME: useful python macros?
    Py_BEGIN_ALLOW_THREADS;
    res = UDT::send(descriptor_, buf, buf_len, 0);
    Py_END_ALLOW_THREADS;

    if (res == UDT::ERROR)
    {
        translateUDTError();
        return;
    }

    PYUDT_LOG_TRACE("Sent " << buf_len << " byte(s) from socket "
                    << descriptor_ << " that were stored in "
                    << static_cast<void *>(&buf));
}


void Socket::bind(const char* ip, uint16_t port) throw()
{
    sockaddr_in addr = build_sockaddr_in(ip, port);

    int res = UDT::bind(descriptor_, (sockaddr*) &addr, sizeof(addr));

    if (res == UDT::ERROR)
    {
        translateUDTError();
        return;
    }

    PYUDT_LOG_TRACE("Bound socket " << descriptor_ << " to address "
                    << ip << ":" << port);
}


void Socket::bind_to_udp(UDPSOCKET udp_socket) throw()
{
    if (UDT::ERROR == UDT::bind2(descriptor_, udp_socket))
    {
        translateUDTError();
        return;
    }

    PYUDT_LOG_TRACE("Bound UDT socket " << descriptor_
                    << " to UDP socket " << udp_socket);
}


void Socket::listen(unsigned int backlog) throw()
{
    if (UDT::ERROR == UDT::listen(descriptor_, backlog))
    {
        translateUDTError();
        return;
    }

    PYUDT_LOG_TRACE("Socket " << descriptor_
                    << " set to listen state. Maximum number of pending "
                    << "connections = " << backlog);
}


void Socket::connect(const char* ip, uint16_t port) throw()
{
    sockaddr_in addr = build_sockaddr_in(ip, port);

    if (UDT::ERROR == UDT::connect(descriptor_, (sockaddr*) &addr, sizeof(addr)))
    {
        translateUDTError();
        return;
    }

    PYUDT_LOG_TRACE("Connect socket " << descriptor_ << " to address "
                    << ip << ":" << port);
}


Socket* Socket::accept() throw()
{
    // Parameters of the incoming connection
    sockaddr addr;
    int addrlen;

    // Retrieve an incoming connection
    UDTSOCKET client = UDT::accept(descriptor_, &addr, &addrlen);

    if (client == UDT::ERROR)
    {
        translateUDTError();
        return NULL;
    }

    PYUDT_LOG_TRACE("Accept connection to socket " << descriptor_
                    << " from address " << addr.sa_data);

    return new Socket(client);
}

} // namespace pyudt4
