#include <iostream>
#include "parser.h"

void TokenParser::SetStartCallback(const std::function<void()>& callback) {
    StartCallback = callback;
}

void TokenParser::SetEndCallback(const std::function<void()>& callback) {
    EndCallback = callback;
}

void TokenParser::SetDigitTokenCallback(const std::function<void(const std::string&)>& callback) {
    DigitTokenCallback = callback;
}

void TokenParser::SetStringTokenCallback(const std::function<void(const std::string&)>& callback) {
    StringTokenCallback = callback;
}

void TokenParser::Parse(const std::string& input) {
    if (StartCallback != nullptr)
        StartCallback();

    for (auto token : input) {
        if (TokenParser::isNumber(&token))
            if (DigitTokenCallback != nullptr)
                DigitTokenCallback(&token);

        if (StringTokenCallback != nullptr)
            StringTokenCallback(&token);
    }

    if (EndCallback != nullptr)
        EndCallback();
}

bool TokenParser::isNumber(const std::string& token) {
    if (token.empty())
        return false;

    for (auto c : token) {
        if (!isdigit(c))
            return false;
    }
    return true;
}


int main()
{
    TokenParser parser;
    parser.SetDigitTokenCallback([](const std::string& token) {
        std::cout << "Digit token: " << token << "\n";
    });

    parser.SetStringTokenCallback([](const std::string& token) {
        std::cout << "String token: " << token << "\n";
    });


    std::string line;
    while (std::getline(std::cin, line)) {
        parser.Parse(line);
    }
}
