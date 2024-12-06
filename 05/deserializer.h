#pragma once
#include "error.h"
#include <cstdint>
#include <istream>

class Deserializer {
    static constexpr char Separator = ' ';
    std::istream& in_;

public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.deserialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args)
    {
        return process(args...);
    }

private:
    Error process()
    {
        return Error::NoError;
    }

    template <class T, class... Args>
    Error process(T& val, Args&... args)
    {
        Error err = load_value(val);
        if (err != Error::NoError)
            return err;

        return process(args...);
    }

    Error load_value(uint64_t& val)
    {
        std::string text;
        in_ >> text;

        if (text.empty())
            return Error::CorruptedArchive;

        try {
            size_t pos;
            val = std::stoull(text, &pos);
            if (pos != text.size())
                return Error::CorruptedArchive;
        } catch (...) {
            return Error::CorruptedArchive;
        }

        return Error::NoError;
    }

    Error load_value(bool& val)
    {
        std::string text;
        in_ >> text;

        if (text == "true")
            val = true;
        else if (text == "false")
            val = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }
};
