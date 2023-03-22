

#include <iostream>
#include <string>

#include "utils/color.h"
#include "utils/logger.h"

/**************************************/

void IPrinter::print(const char* msg) const
{
    std::cout << msg << std::endl;
}

/**************************************/

class LoggerImpl
{
public:
    explicit LoggerImpl(std::unique_ptr<IPrinter> printer);

    void info(const std::string& msg) const;
    void error(const std::string& msg) const;
    void warning(const std::string& msg) const;
    void debug(const std::string& msg) const;

private:
    std::unique_ptr<IPrinter> m_printer{};
};

LoggerImpl::LoggerImpl(std::unique_ptr<IPrinter> printer)
    : m_printer(std::move(printer))
{ }

void LoggerImpl::info(const std::string& msg) const
{
    const std::string info_msg{Color::green() + "[INFO]" + Color::reset() + " " + msg};
    m_printer->print(info_msg.c_str());
}

void LoggerImpl::error(const std::string& msg) const
{
    const std::string error_msg{Color::red() + "[ERROR]" + Color::reset() + " " + msg};
    m_printer->print(error_msg.c_str());
}

void LoggerImpl::warning(const std::string& msg) const
{
    const std::string warn_msg{Color::yellow() + "[WARN]" + Color::reset() + " " + msg};
    m_printer->print(warn_msg.c_str());
}

void LoggerImpl::debug(const std::string& msg) const
{
    const std::string debug_msg{Color::blue() + "[DEBUG]" + Color::reset() + " " + msg};
    m_printer->print(debug_msg.c_str());
}

static LoggerImpl& instance(std::unique_ptr<IPrinter> printer = std::make_unique<IPrinter>())
{
    static LoggerImpl instance{std::move(printer)};
    return instance;
}

/**************************************/
// Remember that static is per a shared lib, so if you have multiple shared targets,
// init will work only for the foremost target. E.g. if gui links against 'core' target
// and core target would dynamically link to 'detector' target, only logs from the 'core'
// would be printed with a new printer
void Logger::init(std::unique_ptr<IPrinter> printer)
{
    instance(std::move(printer));
}

void Logger::enable_color()
{
    Color::enable();
}

void Logger::disable_color()
{
    Color::disable();
}

void Logger::log(LogType type, const std::stringstream& stream)
{
    switch(type)
    {
    case LogType::INFO:
        instance().info(stream.str());
        break;
    case LogType::ERROR:
        instance().error(stream.str());
        break;
    case LogType::WARNING:
        instance().warning(stream.str());
        break;
    case LogType::DEBUG:
        instance().debug(stream.str());
        break;
    default:
        break;
    }
}
