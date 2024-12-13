#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cassert>
#include <sstream>
#include <utility>

class FormatException : public std::runtime_error {
public:
    FormatException(const std::string& error)
        : std::runtime_error(error)
    {
    }
};

template <class T>
concept Streamable = requires(std::ostream& out, const T& a)
{
    out << a;
};

template <Streamable... Args>
std::vector<std::function<void(std::ostream&)>> makeFuncs(Args&&... args)
{
    std::vector<std::function<void(std::ostream&)>> funcs;
    funcs.reserve(sizeof...(Args));
    (funcs.emplace_back([arg = std::forward<Args>(args)](std::ostream& os) { os << arg; }), ...);

    return funcs;
}

template <Streamable... Args>
std::string format(const std::string& stringToFormat, Args... args)
{
    auto argFuncs = makeFuncs(std::forward<Args>(args)...);
    std::ostringstream oss;
    size_t i = 0;
    while(i < stringToFormat.length()) {
        if (stringToFormat[i] == '{') {
            size_t braceStart = i;
            ++i;
            if (i >= stringToFormat.length()) {
                throw FormatException("Unmatched { at position " + std::to_string(braceStart));
            }
            if (std::isdigit(stringToFormat[i])) {
                size_t numStart = i;
                while (i < stringToFormat.length() && std::isdigit(stringToFormat[i])) {
                    ++i;
                }
                std::string numStr = stringToFormat.substr(numStart, i - numStart);
                if (i >= stringToFormat.length() || stringToFormat[i] != '}') {
                    throw FormatException("Expected } at position" + std::to_string(i));
                }
                ++i; // skip '}'
                size_t argIndex = std::stoul(numStr);
                if (argIndex >= argFuncs.size()) {
                    throw FormatException("Argument index out of range" + std::to_string(i));
                }
                argFuncs[argIndex](oss);
            } else {
                throw FormatException("Expected digit after { at position " + std::to_string(braceStart));
            }
        } else if (stringToFormat[i] == '}') {
            throw FormatException("Unmatched } at position " + std::to_string(i));
        } else {
            oss << stringToFormat[i];
            ++i;
        }
    }
    return oss.str();
}
