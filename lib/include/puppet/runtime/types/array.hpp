/**
 * @file
 * Declares the array type.
 */
#pragma once

#include "../values/forward.hpp"
#include <memory>
#include <limits>
#include <ostream>

namespace puppet { namespace runtime { namespace types {

    /**
     * Represents the Puppet Array type.
     */
    struct array
    {
        /**
         * Constructs an Array type.
         * @param type The element type of the array.  Defaults to the Data type.
         * @param from The "from" type parameter.
         * @param to The "to" type parameter.
         */
        explicit array(std::unique_ptr<values::type> type = nullptr, int64_t from = std::numeric_limits<int64_t>::min(), int64_t to = std::numeric_limits<int64_t>::max());

        /**
         * Copy constructor for array type.
         * @param other The other array type to copy from.
         */
        array(array const& other);

        /**
         * Move constructor for array type.
         * Uses the default implementation.
         */
        array(array&&) noexcept = default;

        /**
         * Copy assignment operator for array type.
         * @param other The other array type to copy assign from.
         * @return Returns this array type.
         */
        array& operator=(array const& other);

        /**
         * Move assignment operator for array type.
         * Uses the default implementation.
         * @return Returns this array type.
         */
        array& operator=(array&&) noexcept = default;

        /**
         * Gets the element type of the array.
         * @return Returns the element type of the array.
         */
        values::type const& element_type() const;

        /**
         * Gets the "from" type parameter.
         * @return Returns the "from" type parameter.
         */
        int64_t from() const;

        /**
         * Gets the "to" type parameter.
         * @return Returns the "to" type parameter.
         */
        int64_t to() const;

        /**
         * Gets the name of the type.
         * @return Returns the name of the type (i.e. Array).
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
        std::unique_ptr<values::type> _element_type;
        int64_t _from;
        int64_t _to;
    };

    /**
     * Stream insertion operator for array type.
     * @param os The output stream to write the type to.
     * @param type The type to write.
     * @return Returns the given output stream.
     */
    std::ostream& operator<<(std::ostream& os, array const& type);

    /**
     * Equality operator for array.
     * @param left The left type to compare.
     * @param right The right type to compare.
     * @return Returns true if the two types are equal or false if not.
     */
    bool operator==(array const& left, array const& right);

    /**
     * Inequality operator for array.
     * @param left The left type to compare.
     * @param right The right type to compare.
     * @return Returns true if the two types are not equal or false if they are equal.
     */
    bool operator!=(array const& left, array const& right);

    /**
     * Hashes the array type.
     * @param type The array type to hash.
     * @return Returns the hash value for the type.
     */
    size_t hash_value(array const& type);

}}}  // namespace puppet::runtime::types
