#include "global.hpp"

namespace kernel {
    static map<string, Ref<Variant>> kernelGlobal;

    map<string, Ref<Variant>> &getGlobals() { return kernelGlobal; }
    void setGlobal(const string &name, Ref<Variant> value) {
        kernelGlobal[name] = value;
    }
    Ref<Variant> getGlobal(const string &name) { return kernelGlobal[name]; }
    Borrower<std::pair<const string, Ref<Variant>>>
    getGlobalPair(const string &name) {
        return &*kernelGlobal.find(name);
    }
    bool hasGlobal(const string &name) {
        return kernelGlobal.find(name) != kernelGlobal.end();
    }
} // namespace kernel
