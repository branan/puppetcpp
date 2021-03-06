#include <puppet/runtime/values/value.hpp>
#include <boost/functional/hash.hpp>

using namespace std;

namespace puppet { namespace runtime { namespace types {

    char const* any::name()
    {
        return "Any";
    }

    ostream& operator<<(ostream& os, any const&)
    {
        os << any::name();
        return os;
    }

    bool operator==(any const&, any const&)
    {
        return true;
    }

    bool operator!=(any const& left, any const& right)
    {
        return !(left == right);
    }

    bool any::is_instance(values::value const& value) const
    {
        // All values are an instance of Any
        return true;
    }

    bool any::is_specialization(values::type const& other) const
    {
        // All types (except for Any) are specializations of Any
        return !boost::get<any>(&other);
    }

    size_t hash_value(any const&)
    {
        static const size_t name_hash = boost::hash_value(any::name());

        size_t seed = 0;
        boost::hash_combine(seed, name_hash);
        return seed;
    }

}}}  // namespace puppet::runtime::types
