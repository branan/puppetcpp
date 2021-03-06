#include <puppet/compiler/evaluation/operators/logical_not.hpp>

using namespace std;
using namespace puppet::runtime::values;

namespace puppet { namespace compiler { namespace evaluation { namespace operators {

    value logical_not::operator()(unary_operator_context const& context) const
    {
        return !context.operand().is_truthy();
    }

}}}}  // namespace puppet::compiler::evaluation::operators
