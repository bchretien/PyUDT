#include "Epoll.hh"

#include <udt/udt.h>
#include <iostream>
#include <boost/python.hpp>

#include "py-udt4.hh"
#include "Exception.hh"

namespace pyudt4 {

Epoll::Epoll()
{
    id_ = UDT::epoll_create();
}

Epoll::~Epoll()
{
    /* Remove id from epoll */
    UDT::epoll_release(id_);
}

const int& Epoll::getId() const
{
    return id_;
}

void Epoll::setId(int id)
{
    id_ = id;
}

void Epoll::add_usock(boost::python::object& args) throw()
{
    using namespace boost::python;

    tuple args_tuple = make_tuple(args);
    extract<int> get_descriptor(args_tuple[0]);
    extract<int> get_flag(args_tuple[1]);

    if (!get_descriptor.check() || !get_flag.check())
    {
        Exception e("argument: UDTSOCKET, flags", "");
        translateException(e);
        throw e;
    }

    Socket* sock = new Socket();
    sock->setDescriptor(get_descriptor());

    int flag = get_flag();

    //Py_INCREF(sock);
    objmap_[sock->getDescriptor()] = sock;

    if (UDT::ERROR == UDT::epoll_add_usock(id_, sock->getDescriptor(), &flag))
    {
        translateUDTError();
        return;
    }

    //return Py_BuildValue("i", 0);
}

} // namespace pyudt4
