#ifndef __PYUDT_DEBUG_HH_
#define __PYUDT_DEBUG_HH_

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

#ifdef NDEBUG
#   define PYUDT_DEBUG_ONLY(X)
#else
#   define PYUDT_DEBUG_ONLY(X) X
#endif // NDEBUG

namespace pyudt4 {

//log4cxx::BasicConfigurator::configure();
// Define a static logger variable so that it references the
// Logger instance named "pyudt".
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("pyudt"));

/**
 * Simple logger class to facilitate logging with Log4CXX.
 */
class Logger
{
public:
    static void init_logger()
    {
        // Use the basic configurator
        log4cxx::BasicConfigurator::configure();

        // Set default level to TRACE
        logger->setLevel(log4cxx::Level::getTrace());

        LOG4CXX_INFO(logger, "Starting logging.");
    }

    static void load_logger_configuration(std::string filename)
    {
        log4cxx::PropertyConfigurator::configure(filename.c_str());
    }
};

} // namespace pyudt4

#define PYUDT_LOG_DEBUG(expression)       \
    PYUDT_DEBUG_ONLY(LOG4CXX_DEBUG        \
    (log4cxx::Logger::getLogger("pyudt"), \
     expression))

#define PYUDT_LOG_TRACE(expression)       \
    PYUDT_DEBUG_ONLY(LOG4CXX_TRACE        \
    (log4cxx::Logger::getLogger("pyudt"), \
     expression))

#define PYUDT_LOG_INFO(expression)        \
    PYUDT_DEBUG_ONLY(LOG4CXX_INFO         \
    (log4cxx::Logger::getLogger("pyudt"), \
     expression))

#define PYUDT_LOG_ERROR(expression)       \
    PYUDT_DEBUG_ONLY(LOG4CXX_ERROR        \
    (log4cxx::Logger::getLogger("pyudt"), \
     expression))

#endif // __PYUDT_DEBUG_HH_
