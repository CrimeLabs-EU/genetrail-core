//
// Created by Ian Ferenc Diks on 20.12.22.
//

#ifndef GENETRAILCORE_CATEGORYDATABASE_H
#define GENETRAILCORE_CATEGORYDATABASE_H


#include "Category.h"
#include "Editor.h"
#include "Metadata.h"

#include "macros.h"

#include <memory>
#include <string>
#include <vector>

namespace genetrailcore {

    class GENETRAILCORE_EXPORT CategoryDatabase {

    public:
        using iterator = std::vector<Category>::iterator;
        using const_iterator = std::vector<Category>::const_iterator;

        explicit CategoryDatabase(const std::shared_ptr<EntityDatabase> &db);

        template<typename... Args>
        Category &addCategory(Args... args) {
            categories_.emplace_back(entity_database_.get(), std::forward<Args>(args)...);
            return categories_.back();
        }

        void reserve(size_t capacity);

        size_t size() const;

        iterator begin();
        iterator end();

        const_iterator begin() const;
        const_iterator end() const;

        const_iterator cbegin() const;
        const_iterator cend() const;

        const std::string &name() const;
        void setName(const std::string &name);

        Editor &editor();
        const Editor &editor() const;
        void setEditor(const Editor &editor);

        const std::string &creationDate() const;
        void setCreationDate(const std::string &date);

        const std::string &sourceUrl() const;
        void setSourceUrl(const std::string &sourceUrl);

        const std::string &identifier();
        void setIdentifier(const std::string &identifier);

        const Metadata &metadata() const;

        Metadata &metadata();

        const std::shared_ptr<EntityDatabase> &entityDatabase() const;

        const Category &operator[](size_t i) const;

        Category &operator[](size_t i);

    private:
        std::string name_;
        Editor editor_;

        std::string creation_date_;
        std::string source_url_;
        std::string identifier_;

        Metadata metadata_;
        std::shared_ptr<EntityDatabase> entity_database_;

        std::vector<Category> categories_;
    };

}// namespace genetrailcore


#endif//GENETRAILCORE_CATEGORYDATABASE_H
