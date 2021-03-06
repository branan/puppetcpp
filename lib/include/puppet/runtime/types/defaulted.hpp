/**
 * @file
 * Declares the default type.
 */
#pragma once

#include "../values/forward.hpp"
#include <ostream>

namespace puppet { namespace runtime { namespace types {

    /**
     * Represents the Puppet Default type.
     */
    struct defaulted
    {
        /**
         * Gets the name of the type.
         * @return Returns the name of the type (i.e. Default).
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
    };

    /**
     * Stream insertion operator for defaulted type.
     * @param os The output stream to write the type to.
     * @return Returns the given output stream.
     */
    std::ostream& operator<<(std::ostream& os, defaulted const&);

    /**
     * Equality operator for defaulted.
     * @param left The left type to compare.
     * @param right The right type to compare.
     * @return Returns true if the two types are equal or false if not.
     */
    bool operator==(defaulted const& left, defaulted const& right);

    /**
     * Inequality operator for defaulted.
     * @param left The left type to compare.
     * @param right The right type to compare.
     * @return Returns true if the two types are not equal or false if they are equal.
     */
    bool operator!=(defaulted const& left, defaulted const& right);

    /**
     * Hashes the defaulted type.
     * @param type The defaulted type to hash.
     * @return Returns the hash value for the type.
     */
    size_t hash_value(defaulted const& type);

}}}  // namespace puppet::runtime::types
