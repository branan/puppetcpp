/**
 * @file
 * Declares the splat operator.
 */
#pragma once

#include "unary_operator_context.hpp"

namespace puppet { namespace compiler { namespace evaluation { namespace operators {

    /**
     * Implements the splat operator.
     */
    struct splat
    {
        /**
         * Called to invoke the operator.
         * @param context The unary operator context.
         * @return Returns the resulting value.
         */
        runtime::values::value operator()(unary_operator_context const& context) const;
    };

}}}}  // namespace puppet::compiler::evaluation::operators
