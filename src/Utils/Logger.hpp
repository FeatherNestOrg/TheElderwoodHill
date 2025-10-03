#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/fmt/fmt.h>
#include <memory>
#include <string>

namespace teh::utils
{
    class Logger
    {
    public:
        // Logger categories for different game systems
        enum class Category
        {
            GAME,           // Main game logic
            GRAPHICS,       // Rendering, SDL operations
            MAP,           // Map loading and processing
            RESOURCE,      // Asset loading
            INPUT,         // Input handling
            PERFORMANCE,   // Performance metrics
            NETWORK        // Network operations (future use)
        };

        // Initialize the logging system
        static void init();

        // Shutdown the logging system
        static void shutdown();

        // Get logger for specific category
        static std::shared_ptr<spdlog::logger> getLogger(Category category);

        // Convenience methods for different log levels
        template<typename FormatString, typename... Args>
        static void trace(Category category, const FormatString& format, Args&&... args)
        {
            auto logger = getLogger(category);
            logger->trace(fmt::runtime(format), std::forward<Args>(args)...);
            logger->flush();
        }

        template<typename FormatString, typename... Args>
        static void debug(Category category, const FormatString& format, Args&&... args)
        {
            auto logger = getLogger(category);
            logger->debug(fmt::runtime(format), std::forward<Args>(args)...);
            logger->flush();
        }

        template<typename FormatString, typename... Args>
        static void info(Category category, const FormatString& format, Args&&... args)
        {
            auto logger = getLogger(category);
            logger->info(fmt::runtime(format), std::forward<Args>(args)...);
            logger->flush();
        }

        template<typename FormatString, typename... Args>
        static void warn(Category category, const FormatString& format, Args&&... args)
        {
            auto logger = getLogger(category);
            logger->warn(fmt::runtime(format), std::forward<Args>(args)...);
            logger->flush();
        }

        template<typename FormatString, typename... Args>
        static void error(Category category, const FormatString& format, Args&&... args)
        {
            auto logger = getLogger(category);
            logger->error(fmt::runtime(format), std::forward<Args>(args)...);
            logger->flush();
        }

        template<typename FormatString, typename... Args>
        static void critical(Category category, const FormatString& format, Args&&... args)
        {
            auto logger = getLogger(category);
            logger->critical(fmt::runtime(format), std::forward<Args>(args)...);
            logger->flush();
        }

        // Set log level for all loggers
        static void setLevel(spdlog::level::level_enum level);

        // Enable/disable console output
        static void setConsoleOutput(bool enabled);

    private:
        static std::shared_ptr<spdlog::logger> s_gameLogger;
        static std::shared_ptr<spdlog::logger> s_graphicsLogger;
        static std::shared_ptr<spdlog::logger> s_mapLogger;
        static std::shared_ptr<spdlog::logger> s_resourceLogger;
        static std::shared_ptr<spdlog::logger> s_inputLogger;
        static std::shared_ptr<spdlog::logger> s_performanceLogger;
        static std::shared_ptr<spdlog::logger> s_networkLogger;

        static bool s_initialized;
        static bool s_consoleEnabled;

        // Helper methods
        static std::string getCategoryName(Category category);
        static std::shared_ptr<spdlog::logger> createLogger(const std::string& name);
    };
}

// Convenience macros for common logging
#define TEH_LOG_TRACE(category, ...)    teh::utils::Logger::trace(category, __VA_ARGS__)
#define TEH_LOG_DEBUG(category, ...)    teh::utils::Logger::debug(category, __VA_ARGS__)
#define TEH_LOG_INFO(category, ...)     teh::utils::Logger::info(category, __VA_ARGS__)
#define TEH_LOG_WARN(category, ...)     teh::utils::Logger::warn(category, __VA_ARGS__)
#define TEH_LOG_ERROR(category, ...)    teh::utils::Logger::error(category, __VA_ARGS__)
#define TEH_LOG_CRITICAL(category, ...) teh::utils::Logger::critical(category, __VA_ARGS__)

// Category-specific convenience macros
#define TEH_GAME_LOG(level, ...)        TEH_LOG_##level(teh::utils::Logger::Category::GAME, __VA_ARGS__)
#define TEH_GRAPHICS_LOG(level, ...)    TEH_LOG_##level(teh::utils::Logger::Category::GRAPHICS, __VA_ARGS__)
#define TEH_MAP_LOG(level, ...)         TEH_LOG_##level(teh::utils::Logger::Category::MAP, __VA_ARGS__)
#define TEH_RESOURCE_LOG(level, ...)    TEH_LOG_##level(teh::utils::Logger::Category::RESOURCE, __VA_ARGS__)
#define TEH_INPUT_LOG(level, ...)       TEH_LOG_##level(teh::utils::Logger::Category::INPUT, __VA_ARGS__)
#define TEH_PERF_LOG(level, ...)        TEH_LOG_##level(teh::utils::Logger::Category::PERFORMANCE, __VA_ARGS__)
#define TEH_NET_LOG(level, ...)         TEH_LOG_##level(teh::utils::Logger::Category::NETWORK, __VA_ARGS__)
