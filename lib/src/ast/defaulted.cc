#include <puppet/ast/defaulted.hpp>
#include <puppet/cast.hpp>

using namespace std;
using namespace puppet::lexer;

namespace puppet { namespace ast {

    defaulted::defaulted()
    {
    }

    defaulted::defaulted(token_position position) :
        _position(rvalue_cast(position))
    {
    }

    token_position const& defaulted::position() const
    {
        return _position;
    }

    ostream& operator<<(ostream& os, defaulted const&)
    {
        os << "default";
        return os;
    }

}}  // namespace puppet::ast
