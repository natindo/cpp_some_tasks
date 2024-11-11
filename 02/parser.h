#ifndef PARSER_H
#define PARSER_H

#include <cstddef>
#include <functional>
#include <string>
#include <cstdint>

class TokenParser
{
public:
    TokenParser() = default;

    void SetStartCallback(const std::function<void()>& callback);
    void SetEndCallback(const std::function<void()>& callback);
    void SetDigitTokenCallback(const std::function<void(const std::string&)>& callback);
    void SetStringTokenCallback(const std::function<void(const std::string&)>& callback);

    void Parse(const std::string &);

private:
    std::function<void()> StartCallback;
    std::function<void()> EndCallback;
    std::function<void(const std::string&)> DigitTokenCallback;
    std::function<void(const std::string&)> StringTokenCallback;

    bool isNumber(const std::string& token);

};

#endif //PARSER_H
