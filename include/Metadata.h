//
// Created by Ian Ferenc Diks on 18.12.22.
//

#ifndef GENETRAILCORE_METADATA_H
#define GENETRAILCORE_METADATA_H

#include "macros.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/variant.hpp>

namespace genetrailcore {

    class GENETRAILCORE_EXPORT Metadata {
    public:
        class Value;

        using Array = std::vector<Value>;
        using Object = std::unordered_map<std::string, Value>;

    private:
        // We use a boost variant to store the metadata
        using ValueBase =
                boost::variant<std::string, int64_t, double, boost::recursive_wrapper<Object>, boost::recursive_wrapper<Array>, bool, std::nullptr_t>;

    public:
        /**
	 * Helper class representing values stored in the Metadata object.
	 * This wraps boost::variant and provides constructor overloads so
	 * that all type conversions work properly.
	 */
        class Value {
        public:
            template<typename... Args>
            Value(Args... args) : content_(std::forward<Args>(args)...) {}

            Value(const char *value) : content_(std::string(value)) {}

            Value(int8_t value) : content_(static_cast<int64_t>(value)) {}
            Value(int16_t value) : content_(static_cast<int64_t>(value)) {}
            Value(int32_t value) : content_(static_cast<int64_t>(value)) {}

            Value(uint8_t value) : content_(static_cast<int64_t>(value)) {}
            Value(uint16_t value) : content_(static_cast<int64_t>(value)) {}
            Value(uint32_t value) : content_(static_cast<int64_t>(value)) {}
            Value(uint64_t value) : content_(static_cast<int64_t>(value)) {}

            /// Return the stored variant
            ValueBase &operator*() { return content_; }
            /// Return the stored variant
            const ValueBase &operator*() const { return content_; }

        private:
            ValueBase content_;
        };

        Metadata() = default;

        // Implement copy-ctor and assignment operator.
        // The move operations and destructor can be defaulted though.
        Metadata(const Metadata &metadata);
        Metadata(Metadata &&metadata) = default;
        Metadata &operator=(const Metadata &metadata);
        Metadata &operator=(Metadata &&metadata) = default;
        ~Metadata() = default;

        /**
	 * Return the attribute associted with key.
	 * If there is no matching attribute present, return
	 * an empty attribute.
	 */
        Value &operator[](const std::string &key);

        /**
	 * Obtain the metadata attribute associated with
	 * the given key.
	 *
	 * @throws InvalidKey if there is no attribute for key
	 */
        const Value &operator[](const std::string &key) const;

        /**
	 * Obtain the metadata attribute associated with
	 * the given key.
	 *
	 * @throws InvalidKey if there is no attribute for key
	 */
        const Value &get(const std::string &key) const;

        /**
	 * Check whether an attribute for key is present.
	 *
	 * @return true if a matching attribute was found.
	 */
        bool has(const std::string &key) const;

        /**
	 * Remove the attribute matching key
	 *
	 * @return true if an element has been removed.
	 */
        bool remove(const std::string &key);

        /**
	 * Add a new attribute to the metadata.
	 */
        template<typename... Args>
        void add(const std::string &key, Args... T) {
            ensureData_();
            data_->emplace(key, std::forward<Args>(T)...);
        }

        using Map = std::unordered_map<std::string, Value>;
        using iterator = Map::iterator;
        using const_iterator = Map::const_iterator;

        /// Return an iterator pointing to the first element
        iterator begin() {
            ensureData_();
            return data_->begin();
        }

        /// Return an iterator pointing to the end of the metadata
        iterator end() {
            ensureData_();
            return data_->end();
        };

        /// Return a constant iterator pointing to the first element
        const_iterator begin() const { return cbegin(); };

        /// Return a constant iterator pointing to the end of the metadata
        const_iterator end() const { return cend(); };

        /// Return a constant iterator pointing to the first element
        const_iterator cbegin() const {
            // TODO: This is hacky. I do not have a better solution though...
            if (data_ == nullptr) {
                Map map;
                return map.cbegin();
            }

            return data_->cbegin();
        };

        /// Return a constant iterator pointing to the end of the metadata
        const_iterator cend() const {
            // TODO: This is hacky. I do not have a better solution though...
            if (data_ == nullptr) {
                Map map;
                return map.cend();
            }

            return data_->cend();
        };

        /**
	 * @returns The number of metadata elements.
	 */
        size_t size() const { return data_ == nullptr ? 0 : data_->size(); }

        /**
	 * @returns true if no values are currently being stored.
	 */
        bool empty() const { return data_ == nullptr || data_->empty(); }

    private:
        void ensureData_();
        std::unique_ptr<Map> data_;
    };

    /**
 * Function for retrieving the content of
 * a Metadata::Value.
 */
    template<typename T>
    auto get(const Metadata::Value &value) -> decltype(boost::get<T>(*value)) {
        return boost::get<T>(*value);
    }

}// namespace genetrailcore


#endif//GENETRAILCORE_METADATA_H
