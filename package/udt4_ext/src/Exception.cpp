#include "Exception.hh"

#include <Python.h>
#include <udt/udt.h>

#include <boost/python.hpp>
#include <boost/lexical_cast.hpp>

#include "Debug.hh"

namespace pyudt4 {

Exception::Exception(std::string message,
                     std::string extraData)
: message_(message),
  extraData_(extraData)
{
}

Exception::~Exception() throw()
{
}

const char* Exception::what() const throw()
{
    return this->message_.c_str();
}

std::string Exception::getMessage()
{
    return this->message_;
}

std::string Exception::getExtraData()
{
    return this->extraData_;
}

// See: http://thejosephturner.com/blog/2011/06/15/embedding-python-in-c-applications-with-boostpython-part-2/
std::string parse_python_exception()
{
    using namespace boost::python;

    PyObject *type_ptr = NULL, *value_ptr = NULL, *traceback_ptr = NULL;
    PyErr_Fetch(&type_ptr, &value_ptr, &traceback_ptr);
    std::string ret("Unfetchable Python error");

    if(value_ptr != NULL)
    {
        handle<> h_val(value_ptr);
        str a(h_val);
        extract<std::string> returned(a);
        if(returned.check())
            ret +=  ": " + returned();
        else
            ret += std::string(": Unparseable Python error: ");
    }

    if(traceback_ptr != NULL)
    {
        handle<> h_tb(traceback_ptr);
        object tb(import("traceback"));
        object fmt_tb(tb.attr("format_tb"));
        object tb_list(fmt_tb(h_tb));
        object tb_str(str("\n").join(tb_list));
        extract<std::string> returned(tb_str);
        if(returned.check())
            ret += ": " + returned();
        else
            ret += std::string(": Unparseable Python traceback");
    }
    return ret;
}

void translateException(const Exception& e)
{
    // Log the error
    PYUDT_LOG_ERROR(e.what());

    PyErr_SetString(PyExc_TypeError, e.what());
}

void translateUDTError() throw()
{
    // Get the error message
    long err_code = UDT::getlasterror().getErrorCode();
    std::string err_msg = "[UDT error "
                        + boost::lexical_cast<std::string>(err_code)
                        + "] " + UDT::getlasterror().getErrorMessage();

    // Clear the error message from the error buffer
    UDT::getlasterror().clear();

    // Raise an exception
    Exception e(err_msg, "");
    translateException(e);
    throw e;
}

} // namespace pyudt4
