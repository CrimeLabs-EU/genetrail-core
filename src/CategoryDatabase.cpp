//
// Created by Ian Ferenc Diks on 20.12.22.
//
#include "CategoryDatabase.h"

namespace genetrailcore {

    CategoryDatabase::CategoryDatabase(const std::shared_ptr<EntityDatabase> &db) : entity_database_(db) {}

    void CategoryDatabase::reserve(size_t capacity) { categories_.reserve(capacity); }

    CategoryDatabase::iterator CategoryDatabase::begin() { return categories_.begin(); }

    CategoryDatabase::const_iterator CategoryDatabase::begin() const { return categories_.begin(); }

    CategoryDatabase::iterator CategoryDatabase::end() { return categories_.end(); }

    CategoryDatabase::const_iterator CategoryDatabase::end() const { return categories_.end(); }

    CategoryDatabase::const_iterator CategoryDatabase::cbegin() const { return categories_.cbegin(); }

    CategoryDatabase::const_iterator CategoryDatabase::cend() const { return categories_.cend(); }

    const std::string &CategoryDatabase::name() const { return name_; }

    void CategoryDatabase::setName(const std::string &name) { name_ = name; }

    const std::string &CategoryDatabase::creationDate() const { return creation_date_; }

    void CategoryDatabase::setCreationDate(const std::string &date) { creation_date_ = date; }

    const std::string &CategoryDatabase::sourceUrl() const { return source_url_; }

    void CategoryDatabase::setSourceUrl(const std::string &sourceUrl) { source_url_ = sourceUrl; }

    const std::string &CategoryDatabase::identifier() { return identifier_; }

    void CategoryDatabase::setIdentifier(const std::string &identifier) { identifier_ = identifier; }

    Category &CategoryDatabase::operator[](size_t i) { return categories_[i]; }

    const Category &CategoryDatabase::operator[](size_t i) const { return categories_[i]; }

    size_t CategoryDatabase::size() const { return categories_.size(); }

    Metadata &CategoryDatabase::metadata() { return metadata_; }

    const Metadata &CategoryDatabase::metadata() const { return metadata_; }
}// namespace genetrailcore