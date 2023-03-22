
#pragma once

#include <memory>
#include <sstream>

class IPrinter
{
public:
    virtual void print(const char* msg) const;
    virtual ~IPrinter() = default;
};

class Logger
{
public:
    static void init(std::unique_ptr<IPrinter> printer);
    static void enable_color();
    static void disable_color();

    template <typename... Args>
    static void info(Args... args);

    template <typename... Args>
    static void error(Args... args);

    template <typename... Args>
    static void warning(Args... args);

    template <typename... Args>
    static void debug(Args... args);

private:
    enum class LogType
    {
        INFO,
        ERROR,
        WARNING,
        DEBUG
    };

    template <LogType type, typename... Args>
    static void log(Args... args)
    {
        std::stringstream ss{};
        const auto print_helper = [&ss](const auto& arg) { ss << arg << " "; };
        (print_helper(args), ...);
        log(type, ss);
    }

    static void log(LogType type, const std::stringstream& stream);
};

template <typename... Args>
void Logger::info(Args... args)
{
    log<LogType::INFO>(args...);
}

template <typename... Args>
void Logger::error(Args... args)
{
    log<LogType::ERROR>(args...);
}

template <typename... Args>
void Logger::warning(Args... args)
{
    log<LogType::WARNING>(args...);
}

template <typename... Args>
void Logger::debug(Args... args)
{
    log<LogType::DEBUG>(args...);
}
