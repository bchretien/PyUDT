#include "Epoll.hh"

#include <udt/udt.h>
#include <iostream>
#include <set>
#include <boost/python.hpp>

#include "Exception.hh"
#include "Debug.hh"

namespace py = boost::python;

namespace pyudt4 {

Epoll::Epoll()
{
    PYUDT_LOG_TRACE("Creating an epoll...");

    // Create the epoll
    id_ = UDT::epoll_create();

    // Catch a possible error
    if (id_ < 0)
    {
        PYUDT_LOG_ERROR("Could not create an epoll");
        translateUDTError();
        return;
    }

    PYUDT_LOG_TRACE("Created epoll " << id_);
}


Epoll::~Epoll()
{
    PYUDT_LOG_TRACE("Releasing epoll " << id_ << "...");

    // Release the epoll
    if (UDT::epoll_release(id_) < 0)
    {
        PYUDT_LOG_ERROR("Could not release epoll " << id_);
        translateUDTError();
        return;
    }

    PYUDT_LOG_TRACE("Released epoll " << id_);
}


const int& Epoll::getId() const
{
    return id_;
}


void Epoll::setId(int id)
{
    id_ = id;
}


void Epoll::add_usock(py::object py_socket) throw()
{
    Socket* socket;

    try
    {
        try
        {
            socket = py::extract<Socket*>(py_socket);
        }
        catch (...)
        {
            Exception e("Wrong arguments: Epoll::add_usock((Socket)s)", "");
            translateException(e);
            throw e;
        }

        objmap_[socket->getDescriptor()] = socket;

        if (UDT::ERROR == UDT::epoll_add_usock(id_, socket->getDescriptor()))
        {
            PYUDT_LOG_ERROR("Could not add UDT socket "
                            << socket->getDescriptor()
                            << " to epoll " << id_);

            translateUDTError();
            return;
        }
    }
    catch (py::error_already_set& err)
    {
        std::cerr << parse_python_exception() << std::endl;
        throw err;
    }

    PYUDT_LOG_TRACE("Added UDT socket "
                    << socket->getDescriptor()
                    << " to epoll " << id_);
}


void Epoll::add_usock(py::object py_socket, py::object py_flags) throw()
{
    Socket* socket;
    int flags;

    try
    {
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
            PYUDT_LOG_ERROR("Could not add UDT socket "
                            << socket->getDescriptor()
                            << " to epoll " << id_);

            translateUDTError();
            return;
        }
    }
    catch (py::error_already_set& err)
    {
        std::cerr << parse_python_exception() << std::endl;
        throw err;
    }

    PYUDT_LOG_TRACE("Added UDT socket "
                    << socket->getDescriptor()
                    << " to epoll " << id_
                    << " with flag " << flags);
}


void Epoll::remove_usock(py::object py_socket) throw()
{
    Socket* socket;

    try
    {
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
            PYUDT_LOG_ERROR("Could not remove UDT socket "
                            << socket->getDescriptor()
                            << " from epoll " << id_);

            translateUDTError();
            return;
        }
    }
    catch (py::error_already_set& err)
    {
        std::cerr << parse_python_exception() << std::endl;
        throw err;
    }

    PYUDT_LOG_TRACE("Removed UDT socket "
                    << socket->getDescriptor()
                    << " from epoll " << id_);
}


void Epoll::add_ssock(py::object py_socket) throw()
{
    // file descriptor of a system socket
    SYSSOCKET socket;

    try
    {
        try
        {
            socket = py::extract<SYSSOCKET>(py_socket);
        }
        catch (...)
        {
            Exception e("Wrong arguments: Epoll::add_ssock((SYSSOCKET)s)", "");
            translateException(e);
            throw e;
        }

        if (UDT::ERROR == UDT::epoll_add_ssock(id_, socket))
        {
            PYUDT_LOG_ERROR("Could not add system socket "
                            << socket << " to epoll " << id_);

            translateUDTError();
            return;
        }
    }
    catch (py::error_already_set& err)
    {
        std::cerr << parse_python_exception() << std::endl;
        throw err;
    }

    PYUDT_LOG_TRACE("Added system socket "
                    << socket
                    << " to epoll " << id_);
}


void Epoll::add_ssock(py::object py_socket, py::object py_flags) throw()
{
    // file descriptor of a system socket
    SYSSOCKET socket;
    int flags;

    try
    {
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
            PYUDT_LOG_ERROR("Could not add system socket "
                            << socket << " to epoll " << id_);

            translateUDTError();
            return;
        }
    }
    catch (py::error_already_set& err)
    {
        std::cerr << parse_python_exception() << std::endl;
        throw err;
    }

    PYUDT_LOG_TRACE("Added system socket "
                    << socket
                    << " to epoll " << id_);
}


void Epoll::remove_ssock(py::object py_socket) throw()
{
    SYSSOCKET socket;

    try
    {
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
            PYUDT_LOG_ERROR("Could not remove system socket "
                            << socket << " from epoll " << id_);

            translateUDTError();
            return;
        }
    }
    catch (py::error_already_set& err)
    {
        std::cerr << parse_python_exception() << std::endl;
        throw err;
    }

    PYUDT_LOG_TRACE("Removed system socket "
                    << socket
                    << " from epoll " << id_);
}

void Epoll::garbage_collect() throw()
{
    std::map<UDTSOCKET, Socket*>::iterator iter;
    UDTSTATUS status;

    for (iter = objmap_.begin();
         iter != objmap_.end();
         ++iter)
    {
        status = UDT::getsockstate(iter->first);
        if (  status == BROKEN
           || status == CLOSED
           || status == NONEXIST)
        {
            // Remove the UDT socket from the epoll
            UDT::epoll_remove_usock(id_, iter->first);

            // Destroy the socket
            delete iter->second;

            // Remove the socket from the map
            objmap_.erase(iter);
        }
    }

    PYUDT_LOG_TRACE("Garbage collection done for epoll " << id_);
}

int Epoll::wait(int64_t ms_timeout,
                bool do_uread, bool do_uwrite,
                bool do_sread, bool do_swrite) throw ()
{
    int res = UDT::epoll_wait(id_,
                             (do_uread)? &read_udt_:nullptr,
                             (do_uwrite)? &write_udt_:nullptr,
                             ms_timeout,
                             (do_sread)? &read_sys_:nullptr,
                             (do_swrite)? &write_sys_:nullptr);

    if (res == UDT::ERROR)
    {
        if (UDT::getlasterror().getErrorCode() == CUDTException::ETIMEOUT)
            res = 0;
        else translateUDTError();
    }

    PYUDT_LOG_TRACE("Number of UDT/system sockets ready for IO in epoll "
                    << id_ << ": " << res);

    return res;
}

} // namespace pyudt4
