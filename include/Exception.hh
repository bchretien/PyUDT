#ifndef __PYUDT_EXCEPTION_HH_
#define __PYUDT_EXCEPTION_HH_

#include <string>
#include <exception>

namespace pyudt4 {

class Exception : public std::exception
{
public:
    Exception(std::string message, std::string extraData);
    ~Exception() throw();

    const char* what() const throw();

    std::string getMessage();
    std::string getExtraData();

private:
    std::string message_;
    std::string extraData_;
};


void translateException(const Exception& e);
void translateUDTError();

} // namespace pyudt4

#endif // __PYUDT_EXCEPTION_HH_
