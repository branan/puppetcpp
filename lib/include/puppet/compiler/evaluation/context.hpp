/**
 * @file
 * Declares the evaluation context.
 */
#pragma once

#include "../node.hpp"
#include "../catalog.hpp"
#include "scope.hpp"
#include "collectors/collector.hpp"
#include "../../runtime/values/value.hpp"
#include <boost/optional.hpp>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <regex>
#include <iostream>

namespace puppet { namespace compiler { namespace evaluation {

    // Forward declaration of context.
    struct context;

    /**
     * Helper for creating a match scope in an evaluation context.
     */
    struct match_scope
    {
        /**
         * Constructs a match scope.
         * @param context The current evaluation context.
         */
        explicit match_scope(evaluation::context& context);

        /**
         * Destructs a match scope.
         */
        ~match_scope();

     private:
        evaluation::context& _context;
    };

    /**
     * Helper for setting a local scope.
     */
    struct local_scope : match_scope
    {
        /**
         * Constructs a local scope.
         * @param context The current evaluation context.
         * @param scope The scope to set in the evaluation context.  If nullptr, an ephemeral scope is created.
         */
        explicit local_scope(evaluation::context& context, std::shared_ptr<evaluation::scope> scope = nullptr);

        /**
         * Destructs the local scope.
         */
        ~local_scope();

     private:
        evaluation::context& _context;
    };

    /**
     * Helper for creating a node scope in an evaluation context.
     */
    struct node_scope
    {
        /**
         * Constructs a node scope.
         * @param context The current evaluation context.
         * @param resource The node resource.
         */
        node_scope(evaluation::context& context, compiler::resource& resource);

        /**
         * Destructs the node scope.
         */
        ~node_scope();

     private:
        evaluation::context& _context;
    };

    /**
     * Helper for setting a local EPP stream.
     */
    struct local_epp_stream
    {
        /**
         * Constructs a local EPP stream.
         * @param context The current evaluation context.
         * @param stream The stream to set in the evaluation context.
         */
        local_epp_stream(evaluation::context& context, std::ostream& stream);

        /**
         * Destructs the local EPP stream.
         */
        ~local_epp_stream();

     private:
        evaluation::context& _context;
    };

    /**
     * Represents a resource relationship resulting from a relationship operator.
     * Resource relationships are evaluated when the evaluation context is finalized.
     */
    struct resource_relationship
    {
        /**
         * Constructs a resource relationship.
         * @param relationship The relationship between the source and the target.
         * @param source The value representing the source.
         * @param source_context The AST context of the source.
         * @param target The value representing the target.
         * @param target_context The AST context of the target.
         */
        resource_relationship(
            compiler::relationship relationship,
            runtime::values::value source,
            ast::context const& source_context,
            runtime::values::value target,
            ast::context const& target_context);

        /**
         * Gets the relationship between the source and the target.
         * @return Returns the relationship between the source and the target.
         */
        compiler::relationship relationship() const;

        /**
         * Gets the source value.
         * @return Returns the source value.
         */
        runtime::values::value const& source() const;

        /**
         * Gets the AST context of the source.
         * @return Returns the AST context of the source.
         */
        ast::context const& source_context() const;

        /**
         * Gets the target value.
         * @return Returns the target value.
         */
        runtime::values::value const& target() const;

        /**
         * Gets the AST context of the target.
         * @return Returns the AST context of the target.
         */
        ast::context const& target_context() const;

     private:
        friend struct context;
        void evaluate(compiler::catalog& catalog) const;

        std::shared_ptr<ast::syntax_tree> _tree;
        compiler::relationship _relationship;
        runtime::values::value _source;
        ast::context const& _source_context;
        runtime::values::value _target;
        ast::context const& _target_context;
    };

    /**
     * Represents a resource override.
     * Resource overrides are applied immediately, upon resource declaration, or during evaluation context finalization.
     */
    struct resource_override
    {
        /**
         * Constructs a resource override.
         * @param type The resource type being overridden.
         * @param context The AST context of the override expression.
         * @param attributes The attributes to apply to the resource.
         * @param scope The scope where the override is taking place.
         */
        resource_override(
            runtime::types::resource type,
            ast::context const& context,
            compiler::attributes attributes = compiler::attributes(),
            std::shared_ptr<evaluation::scope> scope = nullptr);

        /**
         * Gets the resource type being overridden.
         * @return Returns the resource type being overridden.
         */
        runtime::types::resource const& type() const;

        /**
         * Gets the AST context for the resource override.
         * @return Returns the AST context for the resource override.
         */
        ast::context const& context() const;

        /**
         * Gets the attributes being applied to the resource.
         * @return Returns the attributes being applied to the resource.
         */
        compiler::attributes const& attributes() const;

        /**
         * Gets the scope where the override is taking place.
         * @return Returns the scope where the override is taking place.
         */
        std::shared_ptr<evaluation::scope> const& scope() const;

     private:
        friend struct context;
        void evaluate(compiler::catalog& catalog) const;

        std::shared_ptr<ast::syntax_tree> _tree;
        runtime::types::resource _type;
        ast::context const& _context;
        compiler::attributes _attributes;
        std::shared_ptr<evaluation::scope> _scope;
    };

    /**
     * Represents a defined type that has been declared.
     */
    struct declared_defined_type
    {
        /**
         * Constructs a declared defined type.
         * @param resource The resource of the declared defined type.
         * @param definition The defined type definition.
         */
        declared_defined_type(compiler::resource& resource, defined_type const& definition);

        /**
         * Gets the resource of the declared defined type.
         * @return Returns the resource of the declared defined type.
         */
        compiler::resource const& resource() const;

        /**
         * Gets the definition of the defined type.
         * @return Returns the definition of the defined type.
         */
        defined_type const& definition() const;

     private:
        friend struct context;
        void evaluate(evaluation::context& context) const;

        compiler::resource& _resource;
        defined_type const& _definition;
    };

    /**
     * Represents the evaluation context.
     */
    struct context
    {
        /**
         * Constructs an evaluation context.
         * @param node The node being compiled.
         * @param catalog The catalog being compiled.
         */
        explicit context(compiler::node& node, compiler::catalog& catalog);

        /**
         * Default move constructor for context.
         */
        context(context&&) = default;

        /**
         * Default move assignment operator for context.
         * @return Returns this context.
         */
        context& operator=(context&&) = default;

        /**
         * Gets the node being compiled.
         * @return Returns the node being compiled.
         */
        compiler::node& node() const;

        /**
         * Gets the catalog being compiled.
         * @return Returns the catalog being compiled.
         */
        compiler::catalog& catalog() const;

        /**
         * Gets the currentregistry.
         * @return Returns the current registry.
         */
        compiler::registry const& registry() const;

        /**
         * Gets the current function dispatcher.
         * @return Returns the current function dispatcher.
         */
        evaluation::dispatcher const& dispatcher() const;

        /**
         * Gets the current scope.
         * @return Returns the current scope and will never return nullptr.
         */
        std::shared_ptr<scope> const& current_scope();

        /**
         * Gets the top scope.
         * @return Returns the top scope and will never return nullptr.
         */
        std::shared_ptr<scope> const& top_scope();

        /**
         * Gets the node scope.
         * @return Returns the node scope or nullptr if there currently is no node scope.
         */
        std::shared_ptr<scope> const& node_scope();

        /**
         * Gets the node or top scope.
         * @return Returns the node scope if there is one, otherwise returns the top scope.
         */
        std::shared_ptr<scope> const& node_or_top();

        /**
         * Adds a named scope to the evaluation context.
         * @param scope The scope to add to the evaluation context.
         * @return Returns true if the scope was added or false if the scope already exists.
         */
        bool add_scope(std::shared_ptr<evaluation::scope> scope);

        /**
         * Finds a scope by name.
         * @param name The name of the scope to find.
         * @return Returns a pointer to the scope if found or nullptr if the scope is not found.
         */
        std::shared_ptr<scope> find_scope(std::string const& name) const;

        /**
         * Sets the given matches into the context.
         * Note: This member function has no effect unless a match scope is present.
         * @param matches The matches to set.
         */
        void set(std::smatch const& matches);

        /**
         * Looks up a variable's value.
         * @param expression The variable expression to lookup.
         * @param warn Specifies whether or not a warning should be logged if a namespace-qualified variable cannot be looked up.
         * @return Returns the variable's value or nullptr if the variable was not found.
         */
        std::shared_ptr<runtime::values::value const> lookup(ast::variable const& expression, bool warn = true);

        /**
         * Looks up a match variable value by index.
         * @param index The index of the match variable.
         * @return Returns the match variable's value or nullptr if the variable wasn't found.
         */
        std::shared_ptr<runtime::values::value const> lookup(size_t index) const;

        /**
         * Creates a match scope.
         * @return Returns the match scope.
         */
        match_scope create_match_scope();

        /**
         * Creates a local scope.
         * @param scope The scope to change to.  If nullptr, the scope will be ephemeral.
         * @return Returns the local scope.
         */
        local_scope create_local_scope(std::shared_ptr<evaluation::scope> scope = nullptr);

        /**
         * Writes the given value to the EPP stream.
         * @param value The value to write.
         * @return Returns true if there is a stream to write to or false if not.
         */
        bool epp_write(runtime::values::value const& value);

        /**
         * Writes the given string to the EPP stream.
         * @param string The string to write.
         * @return Returns true if there is a stream to write to or false if not.
         */
        bool epp_write(std::string const& string);

        /**
         * Logs a message.
         * @param level The log level.
         * @param message The message to log.
         * @param context The AST context for the message.
         */
        void log(logging::level level, std::string const& message, ast::context const* context = nullptr);

        /**
         * Declares a class.
         * @param name The name of the class to declare (e.g. 'foo::bar').
         * @param context The AST context where the class is being declared.
         * @return Returns a pointer to the resource representing the class.
         */
        compiler::resource* declare_class(std::string name, ast::context const& context);

        /**
         * Finds a class definition by name.
         * @param name The name of the class to find.
         * @param import Specifies whether or not an attempt to import the class should be made.
         * @return Returns the class definitions or nullptr if the class is not defined.
         */
        std::vector<klass> const* find_class(std::string name, bool import = true);

        /**
         * Finds a defined type definition by name.
         * @param name The name of the defined type to find.
         * @param import Specifies whether or not an attempt to import the defined type should be made.
         * @return Returns the defined type or nullptr if the defined type is not defined.
         */
        compiler::defined_type const* find_defined_type(std::string name, bool import = true);

        /**
         * Determines if the given name is defined.
         * @param name The name to check.
         * @param klass True if classes should be checked or false if not.
         * @param defined_type True if defined types should be checked or false if not.
         * @return Returns true if the name is defined or false i fnot.
         */
        bool is_defined(std::string name, bool klass = true, bool defined_type = true);

        /**
         * Adds a resource relationship to the evaluation context.
         * Resource relationships are evaluated when the context is finalized.
         * @param relationship The resource relationship to add.
         */
        void add(resource_relationship relationship);

        /**
         * Adds a resource override to the evaluation context.
         * Resource overrides are applied immediately, upon resource declaration, or during evaluation context finalization.
         * @param override The resource override to add.
         */
        void add(resource_override override);

        /**
         * Adds a declared defined type to the context.
         * Declared defined types are evaluated upon evaluation context finalization.
         * @param defined_type The defined type that was declared.
         */
        void add(declared_defined_type defined_type);

        /**
         * Adds a collector to the evaluation context.
         * Collectors are evaluated when the context is finalized.
         * @param collector The collector to add.
         */
        void add(std::shared_ptr<collectors::collector> collector);

        /**
         * Evaluates any existing resource overrides for the given resource.
         * @param resource The resource to evaluate any existing resource overrides for.
         */
        void evaluate_overrides(runtime::types::resource const& resource);

        /**
         * Finalizes the context by evaluating delayed expressions.
         * Evaluates collectors and defined types, populates resource relationships, and sets resource overrides.
         */
        void finalize();

     private:
        friend struct match_scope;
        friend struct local_scope;
        friend struct node_scope;
        friend struct local_epp_stream;

        context(context&) = delete;
        context& operator=(context&) = delete;
        void evaluate_defined_types(size_t& index, std::vector<declared_defined_type*>& virtualized);

        compiler::node& _node;
        compiler::catalog& _catalog;
        std::unordered_map<std::string, std::shared_ptr<evaluation::scope>> _scopes;
        std::vector<std::shared_ptr<scope>> _scope_stack;
        std::shared_ptr<scope> _node_scope;
        std::vector<std::shared_ptr<std::vector<std::shared_ptr<runtime::values::value const>>>> _match_stack;
        std::unordered_set<std::string> _classes;
        std::vector<declared_defined_type> _defined_types;
        std::unordered_multimap<runtime::types::resource, resource_override, boost::hash<runtime::types::resource>> _overrides;
        std::vector<resource_relationship> _relationships;
        std::vector<std::shared_ptr<collectors::collector>> _collectors;
        std::vector<std::ostream*> _stream_stack;
    };

}}}  // namespace puppet::compiler::evaluation
