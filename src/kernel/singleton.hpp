#pragma once
#include <kernel/typedef.hpp>
#include <typeinfo>

namespace kernel {
    template <typename T> class Singleton {
      public:
        using value_t = T;

      public:
        Singleton() = delete;
        Singleton(const Singleton &) = delete;
        Singleton(Singleton &&) = delete;
        ~Singleton() = delete;
        Singleton &operator=(const Singleton &) = delete;
        Singleton &operator=(Singleton &&) = delete;

        inline static T *instance() {
            static Owner<T> _M_instance;
            if (!_M_instance)
                _M_instance.reset(new T());
            return _M_instance.data();
        }
    };
} // namespace kernel
