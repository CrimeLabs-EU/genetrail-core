//
// Created by Ian Ferenc Diks on 18.12.22.
//
#include "Metadata.h"
#include "Exception.h"

#include <memory>

namespace GeneTrail::Core {

    Metadata::Metadata(const Metadata &metadata) {
        if (metadata.data_ != nullptr) { data_ = std::make_unique<Map>(*metadata.data_); }
    }

    Metadata &Metadata::operator=(const Metadata &metadata) {
        if (this == &metadata) { return *this; }

        if (metadata.data_ != nullptr) {
            data_ = std::make_unique<Map>(*metadata.data_);
        } else {
            data_ = nullptr;
        }

        return *this;
    }

    bool Metadata::has(const std::string &key) const {
        if (data_ == nullptr) { return false; }

        return data_->find(key) != data_->end();
    }

    const Metadata::Value &Metadata::operator[](const std::string &key) const { return get(key); }

    const Metadata::Value &Metadata::get(const std::string &key) const {
        if (data_ == nullptr) { throw InvalidKey(key); }

        auto res = data_->find(key);
        if (res != data_->end()) { return res->second; }

        throw InvalidKey(key);
    }

    Metadata::Value &Metadata::operator[](const std::string &key) {
        ensureData_();

        return (*data_)[key];
    }

    void Metadata::ensureData_() {
        if (data_ == nullptr) { data_ = std::make_unique<Map>(); }
    }

    bool Metadata::remove(const std::string &key) {
        if (data_ == nullptr) { return false; }

        return data_->erase(key) > 0;
    }

}// namespace genetrailcore