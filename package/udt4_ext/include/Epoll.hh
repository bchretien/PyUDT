#ifndef __PYUDT_EPOLL_HH_
#define __PYUDT_EPOLL_HH_

#include "Socket.hh"

#include <set>

namespace py = boost::python;

namespace pyudt4 {

/**
 * Wrapper for UDT accesses to epolls.
 */
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
     * @return id of the epoll.
     */
    const int& getId() const;

    /**
     * Set the id of the epoll.
     * @param id id of the epoll
     */
    void setId(int id);

    /**
     * Add an UDT socket to the epoll.
     * @param py_socket socket to add.
     */
    void add_usock(py::object py_socket) throw();
    void add_usock(py::object py_socket, py::object py_events) throw();

    /**
     * Remove an UDT socket from the epoll.
     * @param py_socket socket to remove.
     */
    void remove_usock(py::object py_socket) throw();

    /**
     * Add a system socket to the epoll.
     * @param py_socket socket to add.
     */
    void add_ssock(py::object py_socket) throw();
    void add_ssock(py::object py_socket, py::object py_events) throw();

    /**
     * Remove a system socket from the epoll.
     * @param py_socket socket to remove.
     */
    void remove_ssock(py::object py_socket) throw();

    /**
     * Garbage collect the sockets of the epoll. This may not be needed.
     */
     void garbage_collect() throw ();

    /**
     * Wait for an epoll event. A timeout can be set.
     * @param ms_timeout The time that this epoll should wait for the status
     * change in the input groups, in milliseconds.
     * @param do_uread whether to wait for UDT sockets reads. Default is true.
     * @param do_uwrite whether to wait for UDT sockets writes. Default is true.
     * @param do_sread whether to wait for system sockets reads. Default is
     *        false.
     * @param do_swrite whether to wait for system sockets writes. Default is
     *        false.
     * @return total number of UDT sockets and system sockets ready for IO.
     *
     * According to UDT's documentation:
     * "Negative timeout value will make the function to wait until an event
     * happens. If the timeout value is 0, then the function returns immediately
     * with any sockets associated an IO event. If timeout occurs before any
     * event happens, the function returns 0."
     */
    int wait(int64_t ms_timeout, bool do_uread = true, bool do_uwrite = true,
             bool do_sread = false, bool do_swrite = false) throw ();

    /**
     * Get the UDT sockets available for reading.
     */
    const std::set<UDTSOCKET> get_read_udt() const;

    /**
     * Get the UDT sockets available for writing.
     */
    const std::set<UDTSOCKET> get_write_udt() const;

    /**
     * Get the TCP sockets available for reading.
     */
    const std::set<SYSSOCKET> get_read_tcp() const;

    /**
     * Get the TCP sockets available for writing.
     */
    const std::set<SYSSOCKET> get_write_tcp() const;

private:
    /**
     * Epoll id.
     */
    int id_;

    /**
     * UDTSOCKET --> PyUDT socket map
     */
    std::map<UDTSOCKET, Socket*> objmap_;

    /**
     * Set of UDT sockets available for reading.
     */
    std::set<UDTSOCKET> read_udt_;

    /**
     * Set of UDT sockets available for writing.
     */
    std::set<UDTSOCKET> write_udt_;

    /**
     * Set of system sockets that are ready to read.
     */
    std::set<SYSSOCKET> read_sys_;

    /**
     * Set of system sockets that are read to write, or are broken.
     */
    std::set<SYSSOCKET> write_sys_;
};

} // namespace pyudt4

#endif // __PYUDT_EPOLL_HH_
