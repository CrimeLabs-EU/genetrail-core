//
// Created by Ian Ferenc Diks on 17.12.22.
//
#include "Exception.h"

#include <boost/lexical_cast.hpp>

namespace GeneTrail::Core {

    IOError::IOError(const std::string &error) noexcept : msg_("Input/Output error: " + error) {}

    const char *IOError::what() const noexcept { return msg_.c_str(); }

    NotImplemented::NotImplemented(const char *file, int line, const std::string &method) noexcept {
        msg_ = "The method '" + method + "' is currently not implemented in file: " + file + ", line: ";

        try {
            msg_ += boost::lexical_cast<std::string>(line);
        } catch (boost::bad_lexical_cast &e) { msg_ = "Error creating exception. (Bad lexical cast)"; }
    }

    const char *NotImplemented::what() const noexcept { return msg_.c_str(); }

    InvalidIndex::InvalidIndex(unsigned int i, unsigned int max) noexcept {
        try {
            msg_ = std::string("Invalid index ") + boost::lexical_cast<std::string>(i) + ". Strict upper bound is " +
                   boost::lexical_cast<std::string>(max) + ".";
        } catch (boost::bad_lexical_cast &e) { msg_ = std::string("Error creating exception. (Bad lexical cast)"); }
    }

    const char *InvalidIndex::what() const noexcept { return msg_.c_str(); }

    InvalidKey::InvalidKey(const std::string &key) noexcept {
        try {
            msg_ = "Invalid key: '" + key + "'!";
        } catch (...) {
            // This method may not throw, however creating a string may
            // allocate memory and thus throw.
        }
    }

    const char *InvalidKey::what() const noexcept { return msg_.c_str(); }

    UnknownEntry::UnknownEntry(const std::string &entry_as_string) noexcept
        : msg_("Error during lookup! Unknown entry \"" + entry_as_string + "\".") {}

    const char *UnknownEntry::what() const noexcept { return msg_.c_str(); }

}// namespace GeneTrail::Core