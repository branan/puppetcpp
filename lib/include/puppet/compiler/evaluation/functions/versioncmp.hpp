/**
 * @file
 * Declares the versioncmp function.
 */
#pragma once

#include "function_call_context.hpp"

namespace puppet { namespace compiler { namespace evaluation { namespace functions {

    /**
     * Implements the versioncmp function.
     */
    struct versioncmp
    {
        /**
         * Called to invoke the function.
         * @param context The function call context.
         * @return Returns the resulting value.
         */
        runtime::values::value operator()(function_call_context& context) const;
    };

}}}}  // puppet::compiler::evaluation::functions
