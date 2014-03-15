#include <boost/python.hpp>
#include <signal.h>

#include "Memory.hh"
#include "Epoll.hh"
#include "Socket.hh"
#include "Exception.hh"
#include "Debug.hh"

using namespace boost::python;
using namespace pyudt4;

namespace pyudt4 {

/**
 * Release the UDT library.
 */
static void udt_cleanup()
{
    PYUDT_LOG_TRACE("Releasing UDT...");
    if (UDT::ERROR == UDT::cleanup())
    {
        translateUDTError();
        return;
    }
    PYUDT_LOG_TRACE("UDT released!");
}


/**
 * Signal handler that provides a way to clean UDT when signals are caught.
 */
static void signal_handler(int sig)
{
    bool is_handled = false;

    switch (sig)
    {
    case SIGINT:
        PYUDT_LOG_ERROR("Caught SIGINT.");
        is_handled = true;
        break;

    case SIGTERM:
        PYUDT_LOG_ERROR("Caught SIGTERM.");
        is_handled = true;
        break;

    default:
        // Unhandled signal, doing nothing
        break;
    }

    if (is_handled)
    {
        // Cleaning UDT, hoping that this call will not hang...
        udt_cleanup();
        exit(sig);
    }
}


/**
 * Handle system signals to call the proper cleaning functions.
 */
static void handle_signals()
{
    if (signal(SIGINT, signal_handler) == SIG_ERR)
    {
        PYUDT_LOG_ERROR("Cannot catch SIGINT.");
    }
    if (signal(SIGTERM, signal_handler) == SIG_ERR)
    {
        PYUDT_LOG_ERROR("Cannot catch SIGTERM.");
    }

    PYUDT_LOG_TRACE("Signal handler set.");
}


/**
 * Initialize the UDT library.
 */
static void udt_startup()
{
    PYUDT_LOG_TRACE("Initializing UDT...");
    if (UDT::ERROR == UDT::startup())
    {
        translateUDTError();
        return;
    }
    PYUDT_LOG_TRACE("UDT initialized!");

    // Also set a signal handler, to clean UDT if things go awry
    handle_signals();
}


namespace detail {

/**
 * Based on the solution provided by Luc Bourhis.
 * @See: http://cci.lbl.gov/cctbx_sources/boost_adaptbx/tuple_conversion.h
 */
template <class T>
struct to_python
{
    template <class Head, class Tail>
    static inline
    py::tuple type_to_python(boost::tuples::cons<Head,Tail> const& x)
    {
        py::tuple head = py::make_tuple(x.get_head());
        py::tuple tail = type_to_python(x.get_tail());
        return py::tuple(head + tail);
    }

    static inline
    py::tuple type_to_python(boost::tuples::null_type)
    {
        return py::tuple();
    }

    template <class U>
    static inline
    py::tuple type_to_python(std::set<U> const& s)
    {
        return py::tuple(s);
    }

    template <class U>
    static inline
    py::object type_to_python(shared_ptr<U> const& x)
    {
        return py::object(x.get());
    }

    static
    PyObject* convert(T const& x)
    {
        return py::incref(type_to_python(x).ptr());
    }
};

} // namespace detail

/**
 * Template tuple converter to Python.
 */
template<class T>
struct to_python
{
    to_python()
    {
        boost::python::to_python_converter<T, detail::to_python<T> >();
    }
};

} // namespace pyudt4


// Member function overloads
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(epoll_wait, Epoll::wait, 1, 5)

BOOST_PYTHON_MODULE(udt4_ext)
{
    // CONVERTERS

    to_python<boost::tuple<const char*, uint16_t> >();
    to_python<boost::tuple<Socket_ptr, boost::tuple<const char*, uint16_t> > >();
    to_python<std::set<int> >();

    // SOCKET

    // Member function pointer variables
    void (Socket::*socket_send)     (const char*, int) const = &Socket::send;
    void (Socket::*socket_send_py)  (object)           const = &Socket::send;
    void (Socket::*socket_send_str) (std::string)      const = &Socket::send;
    void (Socket::*socket_recv)     (char*, int)       const = &Socket::recv;
    str  (Socket::*socket_recv_obj) (int)              const = &Socket::recv;
    void (Socket::*socket_bind)     (const char*, uint16_t)  = &Socket::bind;
    void (Socket::*socket_bind_obj) (object)                 = &Socket::bind;
    void (Socket::*socket_connect)  (const char*, uint16_t)  = &Socket::connect;
    void (Socket::*socket_connect_obj) (object)              = &Socket::connect;

    class_<Socket, shared_ptr<Socket> >("Socket", init<>())
    .def(init<UDTSOCKET,bool>())
    .def("descriptor", &Socket::setDescriptor)
    .def("descriptor", &Socket::getDescriptor, return_value_policy<copy_const_reference>())
    .def("addr_family", &Socket::setAddressFamily)
    .def("addr_family", &Socket::getAddressFamily, return_value_policy<copy_const_reference>())
    .def("type", &Socket::setType)
    .def("type", &Socket::getType, return_value_policy<copy_const_reference>())
    .def("protocol", &Socket::setProtocol)
    .def("protocol", &Socket::getProtocol, return_value_policy<copy_const_reference>())
    .def("close_on_delete", &Socket::setCloseOnDelete)
    .def("close_on_delete", &Socket::getCloseOnDelete, return_value_policy<copy_const_reference>())
    .def("close", &Socket::close)
    .def("__str__", &Socket::str)
    .def("send", socket_send)
    .def("send", socket_send_py)
    .def("send", socket_send_str)
    .def("recv", socket_recv)
    .def("recv", socket_recv_obj)
    .def("bind", socket_bind)
    .def("bind", socket_bind_obj)
    .def("bind_to_udp", &Socket::bind_to_udp)
    .def("listen", &Socket::listen)
    .def("connect", socket_connect)
    .def("connect", socket_connect_obj)
    .def("accept", &Socket::accept)
    ;

    // EPOLL

    // Member function pointer variables
    void (Epoll::*add_usock)      (object)         = &Epoll::add_usock;
    void (Epoll::*add_usock_flags)(object, object) = &Epoll::add_usock;
    void (Epoll::*add_ssock)      (object)         = &Epoll::add_ssock;
    void (Epoll::*add_ssock_flags)(object, object) = &Epoll::add_ssock;

    class_<Epoll>("Epoll")
    .def("id", &Epoll::setId)
    .def("id", &Epoll::getId, return_value_policy<copy_const_reference>())
    .def("add_ssock", add_ssock)
    .def("add_ssock", add_ssock_flags)
    .def("add_usock", add_usock)
    .def("add_usock", add_usock_flags)
    .def("remove_usock", &Epoll::remove_usock)
    .def("remove_ssock", &Epoll::remove_ssock)
    .def("garbage_collect", &Epoll::garbage_collect)
    .def("wait", &Epoll::wait,
         epoll_wait(args("ms_timeout", "do_uread", "do_uwrite",
                                       "do_sread", "do_swrite"),
                    "Wait for an epoll event. A timeout can be set."))
    .def("get_read_udt", &Epoll::get_read_udt)
    .def("get_write_udt", &Epoll::get_write_udt)
    .def("get_read_tcp", &Epoll::get_read_tcp)
    .def("get_write_tcp", &Epoll::get_write_tcp)
    ;

    // Enums
    enum_<EPOLLOpt>("EPOLLOpt")
    .value("UDT_EPOLL_IN", UDT_EPOLL_IN)
    .value("UDT_EPOLL_OUT", UDT_EPOLL_OUT)
    .value("UDT_EPOLL_ERR", UDT_EPOLL_ERR)
    .export_values()
    ;

    // EXCEPTION

    register_exception_translator<Exception>(translateException);

    // LOGGER

    class_<Logger>("Logger")
    .def("init_logger", &Logger::init_logger)
    .staticmethod("init_logger")
    .def("load_logger_configuration", &Logger::load_logger_configuration)
    .staticmethod("load_logger_configuration")
    ;

     // GENERAL FUNCTIONS

    def("startup", udt_startup);
    def("cleanup", udt_cleanup);
}
