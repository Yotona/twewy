#include <typeinfo>

namespace __cxxabiv1 {

    // Type of the type_info of a class with no bases
    class __class_type_info : public std::type_info {
    public:
        virtual ~__class_type_info();
    };

    // Type of the type_info of a class with exactly one public, non-virtual base at offset 0
    class __si_class_type_info : public __class_type_info {
    public:
        virtual ~__si_class_type_info();

    private:
        const __class_type_info* __base_type;
    };
} // namespace __cxxabiv1

std::type_info::~type_info() {}

__cxxabiv1::__class_type_info::~__class_type_info() {}

__cxxabiv1::__si_class_type_info::~__si_class_type_info() {}
