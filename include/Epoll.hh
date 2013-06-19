#ifndef __PYUDT_EPOLL_HH_
#define __PYUDT_EPOLL_HH_

#include "Socket.hh"

namespace py = boost::python;

namespace pyudt4 {

class Epoll
{
public:

    /**
     * Default constructor.
     */
    Epoll();

    /**
     * Destructor.
     */
    ~Epoll();

    /**
     * Get the id of the epoll.
     */
    const int& getId() const;

    /**
     * Set the id of the epoll.
     */
    void setId(int);

    /**
     * Add an UDT socket to the epoll.
     */
    void add_usock(py::object py_socket) throw();
    void add_usock(py::object py_socket, py::object py_events) throw();

    /**
     * Remove an UDT socket from the epoll.
     */
    void remove_usock(py::object py_socket) throw();

    /**
     * Add a system socket to the epoll.
     */
    void add_ssock(py::object py_socket) throw();
    void add_ssock(py::object py_socket, py::object py_events) throw();

    /**
     * Remove a system socket from the epoll.
     */
    void remove_ssock(py::object py_socket) throw();

    /**
     * Garbage collect the sockets of the epoll. This may not be needed.
     */
     void garbage_collect() throw ();

private:
    /**
     * Epoll id
     */
    int id_;

    /**
     * UDTSOCKET --> PyUDT socket map
     */
    std::map<UDTSOCKET, Socket*> objmap_;
};

} // namespace pyudt4

#endif // __PYUDT_EPOLL_HH_
