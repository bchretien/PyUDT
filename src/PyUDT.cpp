#include <boost/python.hpp>

#include "Epoll.hh"
#include "Socket.hh"
#include "Exception.hh"
#include "Debug.hh"

using namespace boost::python;
using namespace pyudt4;

BOOST_PYTHON_MODULE(pyudt)
{
    // SOCKET

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
}
