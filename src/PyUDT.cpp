#include <boost/python.hpp>

#include "Epoll.hh"
#include "Socket.hh"
#include "Exception.hh"

using namespace boost::python;
using namespace pyudt4;

BOOST_PYTHON_MODULE(pyudt)
{
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

    class_<Epoll>("Epoll")
    .def("id", &Epoll::setId)
    .def("id", &Epoll::getId, return_value_policy<copy_const_reference>())
    .def("add_usock", &Epoll::add_usock)
    .def("add_ssock", &Epoll::add_ssock)
    .def("remove_usock", &Epoll::remove_usock)
    .def("remove_ssock", &Epoll::remove_ssock)
    ;

    register_exception_translator<Exception>(translateException);
}
