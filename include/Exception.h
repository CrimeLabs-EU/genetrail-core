//
// Created by Ian Ferenc Diks on 17.12.22.
//

#ifndef GENETRAILCORE_EXCEPTION_H
#define GENETRAILCORE_EXCEPTION_H

#include "macros.h"
#include <exception>
#include <string>

namespace genetrailcore {

    class GENETRAILCORE_EXPORT IOError : public std::exception {
    public:
        IOError(const std::string &error) noexcept;

        virtual ~IOError() {}

        virtual const char *what() const noexcept override;

    private:
        std::string msg_;
    };

    class GENETRAILCORE_EXPORT NotImplemented : public std::exception {
    public:
        NotImplemented(const char *file, int line, const std::string &method) noexcept;

        const char *what() const noexcept override;

    private:
        std::string msg_;
    };

    class GENETRAILCORE_EXPORT InvalidIndex : public std::exception {
    public:
        InvalidIndex(unsigned int i, unsigned int max) noexcept;

        const char *what() const noexcept override;

    private:
        std::string msg_;
    };

    class GENETRAILCORE_EXPORT InvalidKey : public std::exception {
    public:
        InvalidKey(const std::string &str) noexcept;

        const char *what() const noexcept override;

    private:
        std::string msg_;
    };

    class GENETRAILCORE_EXPORT UnknownEntry : public std::exception {
    public:
        UnknownEntry(const std::string &entry_as_string) noexcept;

        const char *what() const noexcept override;

    private:
        std::string msg_;
    };
}// namespace genetrailcore


#endif//GENETRAILCORE_EXCEPTION_H
