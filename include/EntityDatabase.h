//
// Created by Ian Ferenc Diks on 18.12.22.
//

#ifndef GENETRAILCORE_ENTITYDATABASE_H
#define GENETRAILCORE_ENTITYDATABASE_H

#include <algorithm>
#include <functional>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "macros.h"

namespace GeneTrail::Core {

    class GENETRAILCORE_EXPORT EntityDatabase {
    public:
        EntityDatabase() = default;
        EntityDatabase(const EntityDatabase &) = default;
        EntityDatabase(EntityDatabase &&) = default;

        EntityDatabase &operator=(const EntityDatabase &) = default;
        EntityDatabase &operator=(EntityDatabase &&) = default;

        /**
         * Removes all entities from the database
         */
        void clear();

        /**
		 * Return the name of instance i
		 *
		 * @param i A valid id of an instance.
		 * @return The name of the instance.
		 */
        const std::string &name(size_t i) const { return db_[i]; }

        /**
		 * Return the id of an entity. If the entity is not yet known,
		 * a new id will be generated and returned.
		 *
		 * @param name The name of an entity.
		 * @return The (possibly new) id of the entity.
		 */
        size_t index(const std::string &name);

        /**
		 * Return the id of an entity. This is the constant
		 * version, which throws an exception if the entity is not known.
		 *
		 * @param name The name of an entity.
		 * @return The id of the entity.
		 *
		 * @throw UnkownEntry if the entity is not registered with the database.
		 */
        size_t index(const std::string &name) const;

        /**
		 * Overload for index(const std::string&)
		 */
        size_t operator()(const std::string &name) { return index(name); }

        /**
		 * Overload for index(const std::string&) const
		 */
        size_t operator()(const std::string &name) const { return index(name); }

        /**
		 * Overload for name(size_t i) const
		 */
        const std::string &operator()(size_t i) const { return name(i); }

        /**
		 * Helper function that looks up the entries in the range [a,b) and
		 * inserts the name/index into c.
		 *
		 * @param a The start of the input range.
		 * @param b The end of the input range.
		 * @param c The start of the output range.
		 */
        template<typename InputIterator, typename OutputIterator>
        void transform(InputIterator a, InputIterator b, OutputIterator c) {
            std::transform(a, b, c, std::ref(*this));
        }

        /**
		 * Helper function that looks up the entries in the range [a,b) and
		 * inserts the name/index into c.
		 *
		 * @param a The start of the input range.
		 * @param b The end of the input range.
		 * @param c The start of the output range.
		 *
		 * @throw UnkownEntry if [a,b) contains std::string instances that
		 *                    cannot be found in the database.
		 */
        template<typename InputIterator, typename OutputIterator>
        void transform(InputIterator a, InputIterator b, OutputIterator c) const {
            std::transform(a, b, c, std::cref(*this));
        }

        /**
		 * Helper function that looks up the entries in the input container and
		 * inserts the name/index into c.
		 *
		 * @param container The container with the input data.
		 * @param out The start of the output range.
		 */
        template<typename Container, typename OutputIterator>
        void transform(const Container &container, OutputIterator out) {
            std::transform(container.begin(), container.end(), out, std::ref(*this));
        }

        /**
		 * Helper function that looks up the entries in the input container and
		 * inserts the name/index into c.
		 *
		 * @param container The container with the input data.
		 * @param out The start of the output range.
		 *
		 * @throw UnkownEntry if the container contains std::string instances
		 *                    that cannot be found in the database.
		 */
        template<typename Container, typename OutputIterator>
        void transform(const Container &container, OutputIterator out) const {
            std::transform(container.begin(), container.end(), out, std::cref(*this));
        }

    private:
        std::unordered_map<std::string, size_t> name_to_index_;
        std::vector<std::string> db_;
    };

}// namespace genetrailcore


#endif//GENETRAILCORE_ENTITYDATABASE_H
