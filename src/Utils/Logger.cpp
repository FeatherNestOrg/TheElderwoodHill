#include "Logger.hpp"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace teh::utils
{
    // Static member definitions
    std::shared_ptr<spdlog::logger> Logger::s_gameLogger = nullptr;
    std::shared_ptr<spdlog::logger> Logger::s_graphicsLogger = nullptr;
    std::shared_ptr<spdlog::logger> Logger::s_mapLogger = nullptr;
    std::shared_ptr<spdlog::logger> Logger::s_resourceLogger = nullptr;
    std::shared_ptr<spdlog::logger> Logger::s_inputLogger = nullptr;
    std::shared_ptr<spdlog::logger> Logger::s_performanceLogger = nullptr;
    std::shared_ptr<spdlog::logger> Logger::s_networkLogger = nullptr;

    bool Logger::s_initialized = false;
    bool Logger::s_consoleEnabled = true; // Default to enabled

    void Logger::init()
    {
        if (s_initialized)
        {
            return;
        }

        try
        {
            // Create logs directory if it doesn't exist
            const std::string logsDir = "logs";
            if (!fs::exists(logsDir))
            {
                fs::create_directories(logsDir);
            }

            // Drop all existing loggers first
            spdlog::drop_all();

            // Set default pattern first
            spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");

            // Create all category loggers
            s_gameLogger = createLogger("GAME");
            s_graphicsLogger = createLogger("GRAPHICS");
            s_mapLogger = createLogger("MAP");
            s_resourceLogger = createLogger("RESOURCE");
            s_inputLogger = createLogger("INPUT");
            s_performanceLogger = createLogger("PERFORMANCE");
            s_networkLogger = createLogger("NETWORK");

            // Set default log level based on build type
#ifdef NDEBUG
            // Release build - only INFO and above
            setLevel(spdlog::level::info);
#else
            // Debug build - all levels
            setLevel(spdlog::level::trace);
#endif

            s_initialized = true;

            if (s_gameLogger)
            {
                s_gameLogger->info("Logger system initialized successfully");
                s_gameLogger->flush();
            }
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            std::cerr << "Log initialization failed: " << ex.what() << std::endl;
        }
        catch (const std::exception& ex)
        {
            std::cerr << "Log initialization failed with exception: " << ex.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Log initialization failed with unknown exception" << std::endl;
        }
    }

    void Logger::shutdown()
    {
        if (!s_initialized)
        {
            return;
        }

        TEH_LOG_INFO(Category::GAME, "Shutting down logger system");

        // Force flush all loggers before shutdown
        spdlog::apply_all([&](const std::shared_ptr<spdlog::logger>& l)
        {
            l->flush();
        });

        // Drop all loggers
        spdlog::drop_all();

        s_initialized = false;
    }

    std::shared_ptr<spdlog::logger> Logger::getLogger(Category category)
    {
        if (!s_initialized)
        {
            init();
        }

        std::shared_ptr<spdlog::logger> logger;
        switch (category)
        {
        case Category::GAME: logger = s_gameLogger;
            break;
        case Category::GRAPHICS: logger = s_graphicsLogger;
            break;
        case Category::MAP: logger = s_mapLogger;
            break;
        case Category::RESOURCE: logger = s_resourceLogger;
            break;
        case Category::INPUT: logger = s_inputLogger;
            break;
        case Category::PERFORMANCE: logger = s_performanceLogger;
            break;
        case Category::NETWORK: logger = s_networkLogger;
            break;
        default: logger = s_gameLogger;
            break;
        }

        // If logger is still null, return a default console logger
        if (!logger && s_gameLogger)
        {
            return s_gameLogger;
        }

        return logger;
    }

    void Logger::setLevel(spdlog::level::level_enum level)
    {
        // Only set level if already initialized to avoid recursion
        if (s_initialized)
        {
            spdlog::set_level(level);
        }
    }

    void Logger::setConsoleOutput(bool enabled)
    {
        s_consoleEnabled = enabled;

        if (s_initialized)
        {
            // Recreate all loggers with new console setting
            s_gameLogger = createLogger("GAME");
            s_graphicsLogger = createLogger("GRAPHICS");
            s_mapLogger = createLogger("MAP");
            s_resourceLogger = createLogger("RESOURCE");
            s_inputLogger = createLogger("INPUT");
            s_performanceLogger = createLogger("PERFORMANCE");
            s_networkLogger = createLogger("NETWORK");
        }
    }

    std::string Logger::getCategoryName(Category category)
    {
        switch (category)
        {
        case Category::GAME: return "GAME";
        case Category::GRAPHICS: return "GRAPHICS";
        case Category::MAP: return "MAP";
        case Category::RESOURCE: return "RESOURCE";
        case Category::INPUT: return "INPUT";
        case Category::PERFORMANCE: return "PERFORMANCE";
        case Category::NETWORK: return "NETWORK";
        default: return "UNKNOWN";
        }
    }

    std::shared_ptr<spdlog::logger> Logger::createLogger(const std::string& name)
    {
        std::vector<spdlog::sink_ptr> sinks;

        // Add console sink if enabled
#ifdef TEH_ENABLE_CONSOLE_LOG
        if (s_consoleEnabled)
        {
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(spdlog::level::trace);
            sinks.push_back(console_sink);
        }
#endif

        // Add rotating file sink for all logs
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            "logs/" + name + ".log",
            1024 * 1024 * 5, // 5MB per file
            3 // Keep 3 files
        );
        file_sink->set_level(spdlog::level::trace);
        sinks.push_back(file_sink);

        // Add separate error file sink for warnings and errors
        if (name != "PERFORMANCE") // Performance logs don't need error file
        {
            auto error_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                "logs/error.log", true
            );
            error_sink->set_level(spdlog::level::warn);
            sinks.push_back(error_sink);
        }

        // Create synchronous logger
        auto logger = std::make_shared<spdlog::logger>(
            name,
            sinks.begin(),
            sinks.end()
        );

        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::debug); // Flush on debug level and above
        spdlog::register_logger(logger);

        return logger;
    }
}
