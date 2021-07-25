#include "Parser.h"

#include <cctype>
#include <regex>

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>

namespace p3::tokenizer
{

    pos name(pos input)
    {
        const char* ptr = input;
        while (std::isalpha(static_cast<unsigned char>(*ptr)))
            ++ptr;
        if (ptr == input)
            return input;
        while (*ptr == '_' || std::isalpha(static_cast<unsigned char>(*ptr)))
            ++ptr;
        return ptr;
    }

    namespace expression
    {
        std::regex floating_point(R"(^[-+]?([0-9]+(\.[0-9]*)?|\.[0-9]+)([eE][-+]?[0-9]+)?)");
    }
    pos floating_point(pos input)
    {
        std::cmatch match;
        if (std::regex_search(input, match, expression::floating_point))
            return match[0].second;
        return input;
    }

    namespace expression
    {
        std::regex comment(R"(\/\*([^*]|[\r\n]|(\*+([^*\/]|[\r\n])))*\*+\/)");
    }
    pos comment(pos input)
    {
        std::cmatch match;
        if (std::regex_match(input, match, expression::comment))
            return &input[match[0].length()];
        return input;
    }

    pos hex_color(pos pos)
    {
        if (*pos != '#')
            return pos;
        auto it = pos + 1;
        if (!std::isxdigit(*it))
            return pos;
        ++it;
        if (!std::isxdigit(*it))
            return pos;
        ++it;
        if (!std::isxdigit(*it))
            return pos;
        ++it;
        if (!std::isxdigit(*it))
            return it;
        ++it;
        if (!std::isxdigit(*it))
            return pos;
        ++it;
        if (!std::isxdigit(*it))
            return pos;
        ++it;
        if (!std::isxdigit(*it))
            return it;
        ++it;
        if (!std::isxdigit(*it))
            return pos;
        return ++it;
    }
}

namespace p3::parser
{

    pos skip_whitespace(pos it)
    {
        while (true)
            if (std::isspace(static_cast<unsigned char>(*it)))
                ++it;
            else if (auto temp = tokenizer::comment(it); temp != it)
                it = temp;
            else
                break;
        return it;
    }

    pos Rule<Cascade>::parse(pos begin, Cascade& cascade)
    {
        static auto const inherit = std::string("inherit");
        static auto const initial = std::string("initial");
        auto it = skip_whitespace(begin);
        if (std::strncmp(it, inherit.c_str(), inherit.size()) == 0)
        {
            cascade = Cascade::inherit;
            return &it[inherit.size()];
        }
        if (std::strncmp(it, initial.c_str(), initial.size()) == 0)
        {
            cascade = Cascade::initial;
            return &it[initial.size()];
        }
        return begin;
    }

    pos Rule<Length>::parse(pos begin, Length& length)
    {
        auto it = skip_whitespace(begin);
        float value;
        if (auto temp = tokenizer::floating_point(it); it != temp)
        {
            value = std::stof(std::string(it, temp));
            it = temp;
        }
        else
            return begin;
        if (std::strncmp(it, "px", 2) == 0)
        {
            length = Px{ value };
            return it + 2;
        }
        if (std::strncmp(it, "em", 2) == 0)
        {
            length = Em{ value };
            return it + 2;
        }
        if (std::strncmp(it, "rem", 3) == 0)
        {
            length = Rem{ value };
            return it + 3;
        }
        return begin;
    }

    pos Rule<Percentage>::parse(pos begin, Percentage& percentage)
    {
        auto it = skip_whitespace(begin);
        if (auto temp = tokenizer::floating_point(it); it != temp)
        {
            percentage.value = std::stof(std::string(it, temp));
            it = temp;
        }
        else
            return begin;
        return *it == '%' ? it + 1 : begin;
    }

    pos Rule<float>::parse(pos begin, float& value)
    {
        auto it = skip_whitespace(begin);
        if (auto temp = tokenizer::floating_point(it); temp != it)
        {
            value = std::stof(std::string(it, temp));
            return temp;
        }
        return begin;
    }

    pos Rule<LengthPercentage>::parse(pos begin, LengthPercentage& lpct)
    {
        Length length;
        if (auto it = parser::parse(begin, length); it != begin)
        {
            lpct = length;
            return it;
        }
        Percentage percentage;
        if (auto it = parser::parse(begin, percentage); it != begin)
        {
            lpct = percentage;
            return it;
        }
        return begin;
    }

    pos Rule<Color>::parse(pos begin, Color& color)
    {
        auto it = skip_whitespace(begin);
        if (auto temp = tokenizer::hex_color(it); it != temp)
        {
            color.value = std::stoi(std::string(it + 1, temp), 0, 16);
            return temp;
        }
        return begin;
    }

}
