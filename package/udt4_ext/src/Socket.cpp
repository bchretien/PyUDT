#include "Socket.hh"

#include <udt/udt.h>
#include <string>
#include <sstream>
#include <arpa/inet.h> // inet_pton
#include <netdb.h> // getnameinfo
#include <boost/tuple/tuple.hpp>

#include "Exception.hh"
#include "Debug.hh"

namespace py = boost::python;

namespace pyudt4 {

namespace detail {

static
std::string af_to_string(int af)
{
    switch(af)
    {
    case AF_INET:
        return "AF_INET";
    case AF_INET6:
        return "AF_INET6";
    default:
        return "Unknown";
    }
}

static
std::string type_to_string(int type)
{
    switch(type)
    {
    case SOCK_STREAM:
        return "SOCK_STREAM";
    case SOCK_DGRAM:
        return "DGRAM";
    default:
        return "Unknown";
    }
}

} // namespace detail

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
  close_on_delete_(true),
  is_alive_(false)
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

    // Set default socket options
    bool blocking_send = false;
    bool blocking_recv = true;
    if (UDT::ERROR == UDT::setsockopt(descriptor_, 0, UDT_SNDSYN,
                                      &blocking_send, sizeof(blocking_send))
     || UDT::ERROR == UDT::setsockopt(descriptor_, 0, UDT_RCVSYN,
                                      &blocking_recv, sizeof(blocking_recv))
       )
    {
        translateUDTError();
        return;
    }

    PYUDT_LOG_TRACE("Set default options for UDT socket " << descriptor_);
}


Socket::Socket(UDTSOCKET descriptor, bool close_on_delete)
: descriptor_(descriptor),
  addr_family_(0),
  type_(0),
  protocol_(0),
  close_on_delete_(close_on_delete),
  is_alive_(true)
{
    PYUDT_LOG_TRACE("Created Socket object from existing socket " << descriptor_);

    PYUDT_LOG_TRACE("Setting options for UDT socket " << descriptor_);

    // TODO: find a way to get the address family, type and protocol from an
    //       existing UDT socket.
    addr_family_ = AF_INET;
    type_ = SOCK_STREAM;

    // Set default socket options
    bool blocking_send = false;
    bool blocking_recv = true;
    if (UDT::ERROR == UDT::setsockopt(descriptor_, 0, UDT_SNDSYN,
                                      &blocking_send, sizeof(blocking_send))
     || UDT::ERROR == UDT::setsockopt(descriptor_, 0, UDT_RCVSYN,
                                      &blocking_recv, sizeof(blocking_recv))
       )
    {
        translateUDTError();
        return;
    }

    PYUDT_LOG_TRACE("Default options set for UDT socket " << descriptor_);
}


Socket::~Socket()
{
    // TODO: make sure that the socket is no longer referenced in an epoll

    // Close socket on destruction
    if (close_on_delete_) close();

    PYUDT_LOG_TRACE("Destroyed UDT socket " << descriptor_);
}


void Socket::close()
{
    int res;

    if (is_alive_)
    {
        res = UDT::close(descriptor_);

        // FIXME: currently ignore invalid socket errors
        // This happens when the socket destructor is called after the epoll
        // destructor.
        if (res == UDT::ERROR
            && UDT::getlasterror().getErrorCode() != CUDTException::EINVSOCK)
        {
            translateUDTError();
            return;
        }
        else
        {
            PYUDT_LOG_TRACE("Closed UDT socket " << descriptor_);
        }

        is_alive_ = false;
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
    ss << "---------------------------\n"
       << "Socket:       " << descriptor_                        << '\n'
       << "Addr family:  " << detail::af_to_string(addr_family_) << '\n'
       << "Type:         " << detail::type_to_string(type_)      << '\n'
       << "Alive:        " << ((is_alive_)? "True":"False")      << '\n'
       << "---------------------------";
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
        PYUDT_LOG_ERROR("Could not receive data from socket " << descriptor_);
        translateUDTError();
        return;
    }

    PYUDT_LOG_TRACE("Received " << buf_len << " byte(s) from socket "
                    << descriptor_ << " that are stored in "
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
        PYUDT_LOG_ERROR("Could not receive data from socket " << descriptor_);
        translateUDTError();

        // None
        return py::str();
    }

    py::str py_buf = buf;
    free(buf);

    PYUDT_LOG_TRACE("Received " << buf_len << " byte(s) from socket "
                    << descriptor_ << " that are stored in "
                    << static_cast<void *>(&buf));

    return py_buf;
}

void Socket::send(std::string str) const throw()
{
    send(str.c_str(), (int)((str.size()) * sizeof(char)));
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
        PYUDT_LOG_ERROR("Could not send data through socket " << descriptor_);
        translateUDTError();
        return;
    }

    PYUDT_LOG_TRACE("Sent " << buf_len << " byte(s) through socket "
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
        PYUDT_LOG_ERROR("Could not send data through socket " << descriptor_);
        translateUDTError();
        return;
    }

    PYUDT_LOG_TRACE("Sent " << buf_len << " byte(s) through socket "
                    << descriptor_ << " that were stored in "
                    << static_cast<void *>(&buf));
}


void Socket::bind(py::object py_address) throw()
{
    char* ip = 0x0;
    uint16_t port = 0;

    try
    {
        py::tuple addr_tuple = py::extract<py::tuple>(py_address);
        ip = py::extract<char*>(addr_tuple[0]);
        port = py::extract<uint16_t>(addr_tuple[1]);
    }
    catch (...)
    {
        Exception e("Wrong arguments: Socket::bind( (addr, port) )", "");
        translateException(e);
        throw e;
    }

    bind(ip, port);
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

    is_alive_ = true;
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

    is_alive_ = true;
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


void Socket::connect(py::object py_address) throw()
{
    char* ip = 0x0;
    uint16_t port = 0;

    try
    {
        py::tuple addr_tuple = py::extract<py::tuple>(py_address);
        ip = py::extract<char*>(addr_tuple[0]);
        port = py::extract<uint16_t>(addr_tuple[1]);
    }
    catch (...)
    {
        Exception e("Wrong arguments: Socket::connect( (addr, port) )", "");
        translateException(e);
        throw e;
    }

    connect(ip, port);
}


void Socket::connect(const char* ip, uint16_t port) throw()
{
    sockaddr_in addr = build_sockaddr_in(ip, port);

    if (UDT::ERROR == UDT::connect(descriptor_, (sockaddr*) &addr, sizeof(addr)))
    {
        translateUDTError();
        return;
    }

    is_alive_ = true;
    PYUDT_LOG_TRACE("Connect socket " << descriptor_ << " to address "
                    << ip << ":" << port);
}


boost::tuple<Socket_ptr, boost::tuple<const char*,uint16_t> >
Socket::accept() throw()
{
    PYUDT_LOG_TRACE("Accepting connection to socket " << descriptor_ << "...");

    // Parameters of the incoming connection
    sockaddr_in client_addr;
    int client_addrlen;
    UDTSOCKET client_descriptor;

    // Retrieve an incoming connection
    Py_BEGIN_ALLOW_THREADS;
    client_descriptor = UDT::accept(descriptor_,
                                    (sockaddr*)&client_addr,
                                    &client_addrlen);
    Py_END_ALLOW_THREADS;

    if (client_descriptor == UDT::ERROR)
    {
        translateUDTError();
        return boost::tuple<Socket_ptr, boost::tuple<const char*,uint16_t> >
               (Socket_ptr(), boost::tuple<const char*,uint16_t>("", 0));
    }

    Socket_ptr client = make_shared<Socket>(client_descriptor);

    client->descriptor_  = client_descriptor;
    client->addr_family_ = addr_family_;
    client->type_        = type_;
    client->protocol_    = protocol_;
    client->is_alive_    = true;

    char client_host[NI_MAXHOST];
    char client_srvc[NI_MAXSERV];

    memset(client_host, '\0', sizeof(client_host));
    memset(client_srvc, '\0', sizeof(client_srvc));

    // Get the client hostname
    if (getnameinfo((sockaddr*) &client_addr, client_addrlen,
                                 client_host, sizeof(client_host) ,
                                 client_srvc, sizeof(client_srvc) ,
                                 NI_NUMERICHOST | NI_NUMERICSERV))
    {
        PYUDT_LOG_ERROR("Failed to get the client host info");
    }


    PYUDT_LOG_TRACE("Accepted connection to socket " << descriptor_
                    << " from address " << client_host);

    return boost::tuple<Socket_ptr, boost::tuple<const char*, uint16_t> >
           (make_shared<Socket>(client->getDescriptor()),
            boost::tuple<const char*, uint16_t>
            (client_host, client_addr.sin_port));
}

} // namespace pyudt4
