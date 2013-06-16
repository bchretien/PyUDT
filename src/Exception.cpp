#include "Exception.hh"

#include <Python.h>
#include <udt/udt.h>

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
