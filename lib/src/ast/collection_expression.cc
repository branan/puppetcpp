#include <puppet/ast/expression_def.hpp>
#include <puppet/ast/utility.hpp>
#include <puppet/cast.hpp>

using namespace std;
using boost::optional;

namespace puppet { namespace ast {

    ostream& operator<<(ostream& os, attribute_query_operator const& op)
    {
        switch (op) {
            case attribute_query_operator::equals:
                os << "==";
                break;

            case attribute_query_operator::not_equals:
                os << "!=";
                break;

            default:
                throw runtime_error("invalid attribute operator.");
        }
        return os;
    }

    query::query() :
        op(attribute_query_operator::none)
    {
    }

    query::query(name attribute, attribute_query_operator op, basic_expression value) :
        attribute(rvalue_cast(attribute)),
        op(op),
        value(rvalue_cast(value))
    {
    }

    ostream& operator<<(ostream& os, ast::query const& query)
    {
        if (query.attribute.value.empty()) {
            return os;
        }
        os << query.attribute << " " << query.op << " " << query.value;
        return os;
    }

    ostream& operator<<(ostream& os, binary_query_operator const& op)
    {
        switch (op) {
            case binary_query_operator::logical_and:
                os << "and";
                break;

            case binary_query_operator::logical_or:
                os << "or";
                break;

            default:
                throw runtime_error("invalid attribute operator.");
        }
        return os;
    }

    binary_query_expression::binary_query_expression() :
        op(binary_query_operator::none)
    {
    }

    binary_query_expression::binary_query_expression(binary_query_operator op, query operand) :
        op(op),
        operand(rvalue_cast(operand))
    {
    }

    ostream& operator<<(ostream& os, binary_query_expression const& expr)
    {
        if (expr.op == binary_query_operator::none) {
            return os;
        }

        os << expr.op << " " << expr.operand;
        return os;
    }

    collection_expression::collection_expression() :
        kind(collection_kind::none)
    {
    }

    collection_expression::collection_expression(collection_kind kind, ast::type type, optional<query> first, vector<binary_query_expression> remainder) :
        kind(kind),
        type(rvalue_cast(type)),
        first(rvalue_cast(first)),
        remainder(rvalue_cast(remainder))
    {
    }

    lexer::position const& collection_expression::position() const
    {
        return type.position;
    }

    ostream& operator<<(ostream& os, collection_expression const& expr)
    {
        if (expr.kind == collection_kind::none) {
            return os;
        }
        os << expr.type << " " << (expr.kind == collection_kind::all ? "<| " : "<<| ");
        if (expr.first) {
            os << *expr.first;
        }
        for (auto const& bexpr : expr.remainder) {
            os << bexpr;
        }
        os << (expr.kind == collection_kind::all ? " |>" : " |>>");
        return os;
    }

}}  // namespace puppet::ast

