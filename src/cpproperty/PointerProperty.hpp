#pragma once

namespace cpproperty {

    template<typename T>
    class PointerProperty {
    private:
        T* value;

    public:
        T& operator=(T* val) {
            return value = this->set(*val);
        };

        T& operator=(T& val) {
            return value = this->set(val);
        };

    protected:
        T& set(T& val) {
            return val;
        };

        const T& operator T() {
            return this->get(*value);
        };

        ~PointerProperty() {
            delete value;
        };
    };
}
