#ifndef SJTU_EXCEPTIONS_HPP
#define SJTU_EXCEPTIONS_HPP

#include <cstddef>
#include <cstring>
#include <string>

/*
 * You don't have to implement exceptions.hpp.
 * Just remember to throw exception when needed.
 */
namespace sjtu {
    class exception {
    protected:
        const std::string variant;
        std::string detail;

    public:
        exception() = default;

        exception(const exception &ec) = default;

        virtual std::string what() {
            return variant + " " + detail;
        }

        virtual ~exception() = default;
    };

    class index_out_of_bound : public exception {
        const std::string msg;

    public:
        index_out_of_bound(std::string message) : msg(std::move(message)) {
        }

        std::string what() override {
            return msg;
        } /* __________________________ */
    };

    class runtime_error : public exception {
        const std::string msg;

    public:
        runtime_error(std::string message) : msg(std::move(message)) {
        }

        std::string what() override {
            return msg;
        }

        /* __________________________ */
    };

    class invalid_iterator : public exception {
        const std::string msg;

    public:
        invalid_iterator(std::string message) : msg(std::move(message)) {
        }

        std::string what() override {
            return msg;
        }

        /* __________________________ */
    };

    class container_is_empty : public exception {
        const std::string msg;

    public:
        container_is_empty(std::string message) : msg(std::move(message)) {
        }

        std::string what() override {
            return msg;
        }

        /* __________________________ */
    };
}

#endif
