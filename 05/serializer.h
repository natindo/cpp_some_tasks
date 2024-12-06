#pragma once
#include "error.h"
#include <cstdint>
#include <ostream>

class Serializer {
    static constexpr char Separator = ' ';
    std::ostream& out_;

public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }

private:
    Error process()
    {
        return Error::NoError;
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        Error err = save_value(std::forward<T>(val));
        if (err != Error::NoError)
            return err;

        if constexpr (sizeof...(Args) > 0)
            out_ << Separator;

        return process(std::forward<Args>(args)...);
    }

    Error save_value(uint64_t val)
    {
        out_ << val;
        return Error::NoError;
    }

    Error save_value(bool val)
    {
        out_ << (val ? "true" : "false");
        return Error::NoError;
    }
};
