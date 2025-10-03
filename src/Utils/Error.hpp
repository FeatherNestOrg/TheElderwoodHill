#ifndef THEELDERWOODHILL_ERROR_HPP
#define THEELDERWOODHILL_ERROR_HPP
#include <string>

namespace teh
{
    enum class ErrorCode
    {
    };

    struct Error
    {
        std::string message;
        ErrorCode code;
    };
}

#endif //THEELDERWOODHILL_ERROR_HPP
