#pragma once

#include <stdexcept>

class InvalidMoveException : public std::exception
{
public:
    InvalidMoveException(const std::string &message) : m_message(message) {}

    const char *what() const noexcept override
    {
        return m_message.c_str();
    }

private:
    std::string m_message;
};
