#pragma once

#include <cstdint>
#include <ostream>

enum class Error { NoError,
    CorruptedArchive };

class Serializer {
    static constexpr char Separator = ' ';

public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    template <class T>
    Error save(T& object) { return object.serialize(*this); }

    template <class... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }
    Error process(bool&& arg);
    Error process(uint64_t&& arg);

private:
    // process использует variadic templates
    <> out_;
};

class Deserializer {
    static constexpr char Separator = ' ';

public:
    explicit Deserializer(std::ostream& in)
        : in_(in)
    {
    }

    template <class T>
    Error load(T& object) { return object.deserializer(*this); }

    template <class... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }
    Error process(bool&& arg);
    Error process(uint64_t&& arg);

private:
    // process использует variadic templates
    <> in_;
};
