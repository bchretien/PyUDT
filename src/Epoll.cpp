#include "Epoll.hh"

#include <udt/udt.h>
#include <iostream>
#include <boost/python.hpp>

#include "py-udt4.hh"
#include "Exception.hh"

namespace py = boost::python;

namespace pyudt4 {

Epoll::Epoll()
{
    // Create the epoll
    id_ = UDT::epoll_create();

    // Catch a possible error
    if (id_ < 0)
    {
        translateUDTError();
        return;
    }
}

Epoll::~Epoll()
{
    // Release the epoll
    if (UDT::epoll_release(id_) < 0)
    {
        translateUDTError();
        return;
    }
}

const int& Epoll::getId() const
{
    return id_;
}

void Epoll::setId(int id)
{
    id_ = id;
}

void Epoll::add_usock(py::object py_socket, py::object py_flags) throw()
{
    try
    {
        Socket* socket;
        int flags;

        try
        {
            socket = py::extract<Socket*>(py_socket);
            flags = py::extract<int>(py_flags);
        }
        catch (...)
        {
            Exception e("Wrong arguments: Epoll::add_usock((Socket)s, (int)flags)", "");
            translateException(e);
            throw e;
        }

        objmap_[socket->getDescriptor()] = socket;

        if (UDT::ERROR == UDT::epoll_add_usock(id_, socket->getDescriptor(), &flags))
        {
            translateUDTError();
            return;
        }
    }
    catch (py::error_already_set& err)
    {
        std::cerr << parse_python_exception() << std::endl;
        throw err;
    }
}

void Epoll::remove_usock(py::object py_socket) throw()
{
    try
    {
        Socket* socket;

        try
        {
            socket = py::extract<Socket*>(py_socket);
        }
        catch (...)
        {
            Exception e("Wrong arguments: Epoll::remove_usock((Socket)s)", "");
            translateException(e);
            throw e;
        }

        objmap_.erase(socket->getDescriptor());

        if (UDT::ERROR == UDT::epoll_remove_usock(id_, socket->getDescriptor()))
        {
            translateUDTError();
            return;
        }
    }
    catch (py::error_already_set& err)
    {
        std::cerr << parse_python_exception() << std::endl;
        throw err;
    }
}

void Epoll::add_ssock(py::object py_socket, py::object py_flags) throw()
{
    try
    {
        // file descriptor of a system socket
        SYSSOCKET socket;
        int flags;

        try
        {
            socket = py::extract<SYSSOCKET>(py_socket);
            flags = py::extract<int>(py_flags);
        }
        catch (...)
        {
            Exception e("Wrong arguments: Epoll::add_ssock((SYSSOCKET)s, (int)flags)", "");
            translateException(e);
            throw e;
        }

        if (UDT::ERROR == UDT::epoll_add_ssock(id_, socket, &flags))
        {
            translateUDTError();
            return;
        }
    }
    catch (py::error_already_set& err)
    {
        std::cerr << parse_python_exception() << std::endl;
        throw err;
    }
}

void Epoll::remove_ssock(py::object py_socket) throw()
{
    try
    {
        SYSSOCKET socket;

        try
        {
            socket = py::extract<SYSSOCKET>(py_socket);
        }
        catch (...)
        {
            Exception e("Wrong arguments: Epoll::remove_ssock((SYSSOCKET)s)", "");
            translateException(e);
            throw e;
        }

        if (UDT::ERROR == UDT::epoll_remove_ssock(id_, socket))
        {
            translateUDTError();
            return;
        }
    }
    catch (py::error_already_set& err)
    {
        std::cerr << parse_python_exception() << std::endl;
        throw err;
    }
}

} // namespace pyudt4
