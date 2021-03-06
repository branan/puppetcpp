/**
 * @file
 * Declares the variant type.
 */
#pragma once

#include "../values/forward.hpp"
#include "../values/array.hpp"
#include <ostream>
#include <vector>

namespace puppet { namespace runtime { namespace types {

    /**
     * Represents the Puppet Variant type.
     */
    struct variant
    {
        /**
         * Constructs a Variant type.
         * @param types The types that make up the variant.
         */
        explicit variant(std::vector<std::unique_ptr<values::type>> types = std::vector<std::unique_ptr<values::type>>());

        /**
         * Copy constructor for variant type.
         * @param other The other variant type to copy from.
         */
        variant(variant const& other);

        /**
         * Move constructor for variant type.
         * Uses the default implementation.
         */
        variant(variant&&) noexcept = default;

        /**
         * Copy assignment operator for variant type.
         * @param other The other variant type to copy assign from.
         * @return Returns this variant type.
         */
        variant& operator=(variant const& other);

        /**
         * Move assignment operator for variant type.
         * Uses the default implementation.
         * @return Returns this variant type.
         */
        variant& operator=(variant&&) noexcept = default;

        /**
         * Gets the variant types.
         * @return Returns the variant types.
         */
        std::vector<std::unique_ptr<values::type>> const& types() const;

        /**
         * Gets the name of the type.
         * @return Returns the name of the type (i.e. Variant).
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
        std::vector<std::unique_ptr<values::type>> _types;
    };

    /**
     * Stream insertion operator for variant type.
     * @param os The output stream to write the type to.
     * @param type The type to write.
     * @return Returns the given output stream.
     */
    std::ostream& operator<<(std::ostream& os, variant const& type);

    /**
     * Equality operator for variant.
     * @param left The left type to compare.
     * @param right The right type to compare.
     * @return Returns true if the two types are equal or false if not.
     */
    bool operator==(variant const& left, variant const& right);

    /**
     * Inequality operator for variant.
     * @param left The left type to compare.
     * @param right The right type to compare.
     * @return Returns true if the two types are not equal or false if they are equal.
     */
    bool operator!=(variant const& left, variant const& right);

    /**
     * Hashes the variant type.
     * @param type The variant type to hash.
     * @return Returns the hash value for the type.
     */
    size_t hash_value(variant const& type);

}}}  // namespace puppet::runtime::types
