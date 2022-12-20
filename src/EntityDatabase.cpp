//
// Created by Ian Ferenc Diks on 18.12.22.
//
#include "EntityDatabase.h"
#include "Exception.h"

namespace genetrailcore {

    void EntityDatabase::clear() {
        name_to_index_.clear();
        db_.clear();
    }

    size_t EntityDatabase::index(const std::string &name) {
        auto res = name_to_index_.find(name);
        if (res == name_to_index_.end()) {
            res = name_to_index_.emplace(name, db_.size()).first;
            db_.push_back(name);
        }
        return res->second;
    }

    size_t EntityDatabase::index(const std::string &name) const {
        auto res = name_to_index_.find(name);
        if (res == name_to_index_.end()) { throw UnknownEntry(name); }
        return res->second;
    }


}// namespace genetrailcore