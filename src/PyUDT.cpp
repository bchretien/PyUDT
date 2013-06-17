#include <boost/python.hpp>

#include "Epoll.hh"
#include "Socket.hh"
#include "Exception.hh"

using namespace boost::python;
using namespace pyudt4;

BOOST_PYTHON_MODULE(pyudt)
{
    class_<Socket>("Socket")
    .def("fd", &Socket::setDescriptor)
    .def("fd", &Socket::getDescriptor, return_value_policy<copy_const_reference>())
    .def("domain", &Socket::setDomain)
    .def("domain", &Socket::getDomain, return_value_policy<copy_const_reference>())
    .def("type", &Socket::setType)
    .def("type", &Socket::getType, return_value_policy<copy_const_reference>())
    .def("protocol", &Socket::setProtocol)
    .def("protocol", &Socket::getProtocol, return_value_policy<copy_const_reference>())
    .def("is_valid", &Socket::setIsValid)
    .def("is_valid", &Socket::getIsValid, return_value_policy<copy_const_reference>())
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
