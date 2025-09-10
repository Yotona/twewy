#ifndef MSL_CPP_EXCEPTION_HPP
#define MSL_CPP_EXCEPTION_HPP

namespace std {

    typedef void (*unexpected_handler)();

    typedef void (*terminate_handler)();
    void terminate();
    void unexpected();

    class exception {
    public:
        exception() throw() {}
        exception(const exception&) throw() {}

        exception& operator=(const exception&) throw() {
            return *this;
        }

        virtual ~exception() throw() {}

        virtual const char* what() const throw() {
            return "exception";
        }
    };

    class bad_exception : public exception {
    public:
        bad_exception() throw() {}
        bad_exception(const bad_exception&) throw() {}

        bad_exception& operator=(const bad_exception&) throw() {
            return *this;
        }

        virtual ~bad_exception() throw() {}

        virtual const char* what() const throw() {
            return "bad_exception";
        }
    };
} // namespace std

using std::terminate;
using std::unexpected;

using std::bad_exception;

#endif // MSL_CPP_EXCEPTION_HPP