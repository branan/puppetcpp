/**
 * @file
 * Declares the regexp type.
 */
#pragma once

#include "../values/forward.hpp"
#include <ostream>
#include <string>

namespace puppet { namespace runtime { namespace types {

    /**
     * Represents the Puppet Regexp type.
     */
    struct regexp
    {
        /**
         * Constructs a Regexp type.
         * @param pattern The regex pattern for the type.  If empty, all patterns match.
         */
        explicit regexp(std::string pattern = std::string());

        /**
         * Gets the pattern being matched or empty string if all patterns match.
         * @return Returns the match pattern.
         */
        std::string const& pattern() const;

        /**
         * Gets the name of the type.
         * @return Returns the name of the type (i.e. Regexp).
         */
        static char const* name();

        /**
         * Determines if the given value is an instance of this type.
         * @param value The value to determine if it is an instance of this type.
         * @return Returns true if the given value is an instance of this type or false if not.
         */
        bool is_instance(values::value const& value) const;

        /**
         * Determines if the given type is a specialization (i.e. more specific) of this type.
         * @param other The other type to check for specialization.
         * @return Returns true if the other type is a specialization or false if not.
         */
        bool is_specialization(values::type const& other) const;

     private:
        std::string _pattern;
    };

    /**
     * Stream insertion operator for regexp type.
     * @param os The output stream to write the type to.
     * @param type The type to write.
     * @return Returns the given output stream.
     */
    std::ostream& operator<<(std::ostream& os, regexp const& type);

    /**
     * Equality operator for regexp.
     * @param left The left type to compare.
     * @param right The right type to compare.
     * @return Returns true if the two types are equal or false if not.
     */
    bool operator==(regexp const& left, regexp const& right);

    /**
     * Inequality operator for regexp.
     * @param left The left type to compare.
     * @param right The right type to compare.
     * @return Returns true if the two types are not equal or false if they are equal.
     */
    bool operator!=(regexp const& left, regexp const& right);

    /**
     * Hashes the regexp type.
     * @param type The regexp type to hash.
     * @return Returns the hash value for the type.
     */
    size_t hash_value(regexp const& type);

}}}  // namespace puppet::runtime::types
