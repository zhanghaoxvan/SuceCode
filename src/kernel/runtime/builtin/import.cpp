#include "import.hpp"
#include <kernel/kernel_entry.hpp>
#include <kernel/kernel_handler.hpp>
#include <kernel/runtime/builtin/modules/filesystem.hpp>
#include <kernel/runtime/builtin/modules/module_db.hpp>
#include <kernel/runtime/builtin/modules/string.hpp>

namespace kernel {
    Ref<Variant> try_import(const string &path) {
        if (!modules::FileSystemLib::instance->is_file(path))
            return nullptr;
        return doFile(path, {}, path);
    }

#define HANDLE_IMPORT(base)                                                    \
    if (Ref<Variant> res =                                                     \
            try_import(StringLib::instance->absolute(base, name)))             \
    return res

    static map<string, Ref<Variant>> cache;

    Ref<Variant> import(const string &name, const string &chunk) {
        if (cache.find(name) != cache.end())
            return cache[name];
        using namespace modules;
        if (Singleton<ModuleDB>::instance()->has(name))
            return new Variant{
                Singleton<ModuleDB>::instance()->get(name).borrow()};

        HANDLE_IMPORT(StringLib::instance->get_basedir(chunk));
        HANDLE_IMPORT(getExe());
        throw RuntimeException("No such a module named \"%s\".", name.c_str());
    }
} // namespace kernel
