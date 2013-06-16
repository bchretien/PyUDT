#ifndef __PYUDT_EPOLL_HH_
#define __PYUDT_EPOLL_HH_

#include "Socket.hh"

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
    void add_usock(boost::python::object& args) throw();

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
