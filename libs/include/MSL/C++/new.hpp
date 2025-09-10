#include <MSL/C++/exception.hpp>

namespace std {

    class bad_alloc : public exception {
    public:
        bad_alloc() throw() {}
        bad_alloc(const bad_alloc&) throw() {}

        bad_alloc& operator=(const bad_alloc&) throw() {
            return *this;
        }

        virtual ~bad_alloc() throw() {}

        virtual const char* what() const throw() {
            return "bad_alloc";
        }
    };

    struct nothrow_t {};

    extern char __throws_bad_alloc;
    extern void __throw_bad_alloc();
} // namespace std

using std::__throw_bad_alloc;
using std::bad_alloc;
using std::nothrow_t;
