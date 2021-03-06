#include <puppet/runtime/values/value.hpp>
#include <puppet/cast.hpp>
#include <boost/functional/hash.hpp>
#include <boost/algorithm/string.hpp>
#include <unordered_set>
#include <regex>

using namespace std;

namespace puppet { namespace runtime { namespace types {

    resource::resource(std::string type_name, std::string title) :
        _type_name(rvalue_cast(type_name)),
        _title(rvalue_cast(title))
    {
        // Make the type name lowercase
        boost::to_lower(_type_name);

        // Now uppercase every start of a type name
        boost::split_iterator<std::string::iterator> end;
        for (auto it = boost::make_split_iterator(_type_name, boost::first_finder("::", boost::is_equal())); it != end; ++it) {
            if (!*it) {
                continue;
            }
            auto range = boost::make_iterator_range(it->begin(), it->begin() + 1);
            boost::to_upper(range);
        }
    }

    std::string const& resource::type_name() const
    {
        return _type_name;
    }

    std::string const& resource::title() const
    {
        return _title;
    }

    bool resource::fully_qualified() const
    {
        return !_type_name.empty() && !_title.empty();
    }

    bool resource::is_class() const
    {
        return _type_name == "Class";
    }

    bool resource::is_stage() const
    {
        return _type_name == "Stage";
    }

    bool resource::is_builtin() const
    {
        // TODO: remove this member once built-in types can be defined
        static const unordered_set<std::string> builtin_types = {
            "Augeas",
            "Class",
            "Computer",
            "Cron",
            "Exec",
            "File",
            "Filebucket",
            "Group",
            "Host",
            "Interface",
            "5klogin",
            "Macauthorization",
            "Mailalias",
            "Maillist",
            "Mcx",
            "Mount",
            "Nagios_command",
            "Nagios_contact",
            "Nagios_contactgroup",
            "Nagios_host",
            "Nagios_hostdependency",
            "Nagios_hostescalation",
            "Nagios_hostextinfo",
            "Nagios_hostgroup",
            "Nagios_service",
            "Nagios_servicedependency",
            "Nagios_serviceescalation",
            "Nagios_serviceextinfo",
            "Nagios_servicegroup",
            "Nagios_timeperiod",
            "Node",
            "Notify",
            "Package",
            "Resources",
            "Router",
            "Schedule",
            "Scheduled_task",
            "Selboolean",
            "Selmodule",
            "Service",
            "Ssh_authorized_key",
            "Sshkey",
            "Stage",
            "Tidy",
            "User",
            "Vlan",
            "Yumrepo",
            "Zfs",
            "Zone",
            "Zpool"
        };
        return builtin_types.count(_type_name) > 0;
    }

    char const* resource::name()
    {
        return "Resource";
    }

    bool resource::is_instance(values::value const& value) const
    {
        // Check for type
        auto ptr = value.as<values::type>();
        if (!ptr) {
            return false;
        }
        // Check for resource type
        auto resource_ptr = boost::get<resource>(ptr);
        if (!resource_ptr) {
            return false;
        }
        // If no type, the given value is a 'resource'
        if (_type_name.empty()) {
            return true;
        }
        // Check type name
        if (_type_name != resource_ptr->type_name()) {
            return false;
        }
        return _title.empty() || _title == resource_ptr->title();
    }

    bool resource::is_specialization(values::type const& other) const
    {
        // Check that the other Resource is specialized
        auto resource = boost::get<types::resource>(&other);
        if (!resource) {
            // Not the same type
            return false;
        }
        // If this resource has no type name, the other is specialized if it does have one
        if (_type_name.empty()) {
            return !resource->type_name().empty();
        }
        // Otherwise, the types need to be the same
        if (_type_name != resource->type_name()) {
            return false;
        }
        // Otherwise, the other one is a specialization if this does not have a title but the other one does
        return _title.empty() && !resource->title().empty();
    }

    boost::optional<resource> resource::parse(std::string const& specification)
    {
        static std::regex specification_regex("^((?:(?:::)?[A-Z]\\w*)+)\\[([^\\]]+)\\]$");

        smatch matches;
        if (!regex_match(specification, matches, specification_regex) || matches.size() != 3) {
            return boost::none;
        }

        auto title = matches[2].str();
        boost::trim(title);
        // Strip quotes if present in the title
        if (!title.empty()) {
            if ((title.front() == '"' && title.back() == '"') ||
                (title.front() == '\'' && title.back() == '\'')) {
                title = title.substr(1, title.size() - 2);
            }
        }
        return resource(matches[1].str(), rvalue_cast(title));
    }

    ostream& operator<<(ostream& os, resource const& type)
    {
        if (type.type_name().empty()) {
            os << resource::name();
            return os;
        }
        os << type.type_name();
        if (type.title().empty()) {
            return os;
        }
        os << "[" << type.title() << "]";
        return os;
    }

    bool operator==(resource const& left, resource const& right)
    {
        return left.type_name() == right.type_name() && left.title() == right.title();
    }

    bool operator!=(resource const& left, resource const& right)
    {
        return !(left == right);
    }

    size_t hash_value(resource const& type)
    {
        static const size_t name_hash = boost::hash_value(resource::name());

        size_t seed = 0;
        boost::hash_combine(seed, name_hash);
        boost::hash_combine(seed, type.type_name());
        boost::hash_combine(seed, type.title());
        return seed;
    }

}}}  // namespace puppet::runtime::types
