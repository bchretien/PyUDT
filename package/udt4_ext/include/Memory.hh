#ifndef __PYUDT_MEMORY_HH_
#define __PYUDT_MEMORY_HH_

#include <boost/version.hpp>

// Cf. http://stackoverflow.com/a/6323298/1043187
// Compilation seems to fail for older Boost versions
#if (__cplusplus > 199711L) && (BOOST_VERSION >= 105300)
#include <memory>
namespace pyudt4
{
    using std::shared_ptr;
    using std::make_shared;
}
#else
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
namespace pyudt4
{
    using boost::shared_ptr;
    using boost::make_shared;
}
#endif

#endif // __PYUDT_MEMORY_HH_
