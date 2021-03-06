/**
 * @file
 * Declares the Puppet language expression evaluator.
 */
#pragma once

#include "context.hpp"
#include "../ast/ast.hpp"
#include "../../runtime/values/value.hpp"
#include <vector>
#include <memory>
#include <unordered_set>

namespace puppet { namespace compiler { namespace evaluation {

    /**
     * Represents the Puppet language expression evaluator.
     */
    struct evaluator : boost::static_visitor<runtime::values::value>
    {
        /**
         * Constructs an expression evaluator.
         * @param context The current evaluation context.
         */
        explicit evaluator(evaluation::context& context);

        /**
         * Gets the current evaluation context.
         * @return Returns the current evaluation context.
         */
        evaluation::context& context();

        /**
         * Evaluates all statements in a syntax tree.
         * @param tree The syntax tree to evaluate.
         * @param arguments The arguments for the tree (EPP syntax trees).
         */
        void evaluate(ast::syntax_tree const& tree, runtime::values::hash* arguments = nullptr);

        /**
         * Evaluates the given expression and returns the resulting runtime value.
         * @param expression The expression to evaluate.
         * @param productive True if the expression is required to be productive (i.e. has side effect) or false if not.
         * @return Returns the runtime value that is the result of evaluating the expression.
         */
        runtime::values::value evaluate(ast::expression const& expression, bool productive = false);

        /**
         * Evaluates the given postfix expression and returns the resulting runtime value.
         * @param expression The expression to evaluate.
         * @return Returns the runtime value that is the result of evaluating the expression.
         */
        runtime::values::value evaluate(ast::postfix_expression const& expression);

        /**
         * Evaluates the given primary expression and returns the resulting runtime value.
         * @param expression The expression to evaluate.
         * @return Returns the runtime value that is the result of evaluating the expression.
         */
        runtime::values::value evaluate(ast::primary_expression const& expression);

        /**
         * Determines if a value is a "match" for an expected value.
         * Uses the match operator for expected regex values or equality for other expected values.
         * @param actual The actual value.
         * @param actual_context The AST context of the actual value.
         * @param expected The expected value.
         * @param expected_context The AST context of the expected value.
         * @return Returns true if the values match or false if not.
         */
        bool is_match(runtime::values::value& actual, ast::context const& actual_context, runtime::values::value& expected, ast::context const& expected_context);

     private:
        template<class> friend class ::boost::detail::variant::invoke_visitor;
        runtime::values::value operator()(ast::undef const&);
        runtime::values::value operator()(ast::defaulted const&);
        runtime::values::value operator()(ast::boolean const& expression);
        runtime::values::value operator()(int64_t value);
        runtime::values::value operator()(long double value);
        runtime::values::value operator()(ast::number const& expression);
        runtime::values::value operator()(ast::string const& expression);
        runtime::values::value operator()(ast::regex const& expression);
        runtime::values::value operator()(ast::variable const& expression);
        runtime::values::value operator()(ast::name const& expression);
        runtime::values::value operator()(ast::bare_word const& expression);
        runtime::values::value operator()(ast::type const& expression);
        runtime::values::value operator()(ast::array const& expression);
        runtime::values::value operator()(ast::hash const& expression);
        runtime::values::value operator()(ast::expression const& expression);
        runtime::values::value operator()(ast::case_expression const& expression);
        runtime::values::value operator()(ast::if_expression const& expression);
        runtime::values::value operator()(ast::unless_expression const& expression);
        runtime::values::value operator()(ast::function_call_expression const& expression);
        runtime::values::value operator()(ast::resource_expression const& expression);
        runtime::values::value operator()(ast::resource_override_expression const& expression);
        runtime::values::value operator()(ast::resource_defaults_expression const& expression);
        runtime::values::value operator()(ast::class_expression const& expression);
        runtime::values::value operator()(ast::defined_type_expression const& expression);
        runtime::values::value operator()(ast::node_expression const& expression);
        runtime::values::value operator()(ast::collector_expression const& expression);
        runtime::values::value operator()(ast::unary_expression const& expression);
        runtime::values::value operator()(ast::epp_render_expression const& expression);
        runtime::values::value operator()(ast::epp_render_block const& expression);
        runtime::values::value operator()(ast::epp_render_string const& expression);

        runtime::values::value evaluate_body(std::vector<ast::expression> const& body);
        ast::resource_body const* find_default_body(ast::resource_expression const& expression);
        attributes evaluate_attributes(bool is_class, std::vector<ast::attribute> const& expressions);
        void splat_attribute(compiler::attributes& attributes, std::unordered_set<std::string>& names, ast::attribute const& expression);
        void validate_attribute(std::string const& name, runtime::values::value& value, ast::context const& context);
        std::vector<resource*> create_resources(bool is_class, std::string const& type_name, ast::resource_expression const& expression, attributes const& defaults);
        void set_attributes(compiler::resource& resource, compiler::attributes const& attributes);
        static runtime::values::type create_relationship_type();
        static runtime::values::type create_audit_type();

        void climb_expression(
            runtime::values::value& left,
            ast::context const& context,
            unsigned int min_precedence,
            std::vector<ast::binary_expression>::const_iterator& begin,
            std::vector<ast::binary_expression>::const_iterator const& end);

        void evaluate(
            runtime::values::value& left,
            ast::context const& left_context,
            ast::binary_operator op,
            ast::context const& operator_context,
            runtime::values::value& right,
            ast::context const& right_context);

        static unsigned int get_precedence(ast::binary_operator op);
        static bool is_right_associative(ast::binary_operator op);

        evaluation::context& _context;
    };

}}}  // namespace puppet::compiler::evaluation
