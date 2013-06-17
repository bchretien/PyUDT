#include "Exception.hh"

#include <Python.h>
#include <udt/udt.h>

#include <boost/python.hpp>

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
    PyErr_SetString(PyExc_TypeError, e.what());
}

void translateUDTError()
{
    do {
        // FIXME: translate the error correctly
        /*PyObject *__obj = PyTuple_New(2);

        PyTuple_SetItem(__obj, 0,
                    PyInt_FromLong(UDT::getlasterror().getErrorCode()));

        PyTuple_SetItem(__obj, 1,
                PyString_FromString
                (UDT::getlasterror().getErrorMessage()));

        UDT::getlasterror().clear();*/
        //PyErr_SetObject(pyudt4_exception_obj, __obj);

        return;
    } while (0);
}

} // namespace pyudt4
