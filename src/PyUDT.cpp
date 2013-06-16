#include <boost/python.hpp>

#include "Epoll.hh"
#include "Socket.hh"
#include "Exception.hh"

using namespace boost::python;
using namespace pyudt4;

BOOST_PYTHON_MODULE(pyudt)
{
    class_<Socket>("Socket")
    .def("setDescriptor", &Socket::setDescriptor)
    .def("getDescriptor", &Socket::getDescriptor, return_value_policy<copy_const_reference>())
    .def("setDomain", &Socket::setDomain)
    .def("getDomain", &Socket::getDomain, return_value_policy<copy_const_reference>())
    .def("setType", &Socket::setType)
    .def("getType", &Socket::getType, return_value_policy<copy_const_reference>())
    .def("setProtocol", &Socket::setProtocol)
    .def("getProtocol", &Socket::getProtocol, return_value_policy<copy_const_reference>())
    .def("setIsValid", &Socket::setIsValid)
    .def("getIsValid", &Socket::getIsValid, return_value_policy<copy_const_reference>())
    .def("__str__", &Socket::str)
    ;

    class_<Epoll>("Epoll")
    .def("setId", &Epoll::setId)
    .def("getId", &Epoll::getId, return_value_policy<copy_const_reference>())
    ;

    register_exception_translator<Exception>(translateException);
}
