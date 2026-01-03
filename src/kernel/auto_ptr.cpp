#include "auto_ptr.hpp"

std::map<const void *, kernel::RefCount::counter_t> *kernel::RefCount::data_ptr;
