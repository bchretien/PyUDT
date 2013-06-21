#include <boost/python.hpp>

#include "Epoll.hh"
#include "Socket.hh"
#include "Exception.hh"
#include "Debug.hh"

using namespace boost::python;
using namespace pyudt4;

namespace pyudt4 {

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
}

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

} // namespace pyudt4


// Member function overloads
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(epoll_wait, Epoll::wait, 1, 5)

BOOST_PYTHON_MODULE(pyudt)
{
    // SOCKET

    // Member function pointer variables
    void   (Socket::*socket_send)     (const char*, int) const = &Socket::send;
    void   (Socket::*socket_send_py)  (object)           const = &Socket::send;
    void   (Socket::*socket_recv)     (char*, int)       const = &Socket::recv;
    str    (Socket::*socket_recv_obj) (int)              const = &Socket::recv;

    class_<Socket>("Socket", init<>())
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
    .def("__str__", &Socket::str)
    .def("send", socket_send)
    .def("send", socket_send_py)
    .def("recv", socket_recv)
    .def("recv", socket_recv_obj)
    .def("bind", &Socket::bind)
    .def("bind_to_udp", &Socket::bind_to_udp)
    .def("listen", &Socket::listen)
    .def("connect", &Socket::connect)
    .def("accept", &Socket::accept, return_value_policy<manage_new_object>())
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
