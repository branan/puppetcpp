#include <puppet/runtime/values/value.hpp>
#include <boost/functional/hash.hpp>

using namespace std;

namespace puppet { namespace runtime { namespace types {

    char const* undef::name()
    {
        return "Undef";
    }

    bool undef::is_instance(values::value const& value) const
    {
        return value.is_undef();
    }

    bool undef::is_specialization(values::type const& other) const
    {
        // No specializations of Undef
        return false;
    }

    ostream& operator<<(ostream& os, undef const&)
    {
        os << undef::name();
        return os;
    }

    bool operator==(undef const&, undef const&)
    {
        return true;
    }

    bool operator!=(undef const& left, undef const& right)
    {
        return !(left == right);
    }

    size_t hash_value(undef const&)
    {
        static const size_t name_hash = boost::hash_value(undef::name());

        size_t seed = 0;
        boost::hash_combine(seed, name_hash);
        return seed;
    }

}}}  // namespace puppet::runtime::types
