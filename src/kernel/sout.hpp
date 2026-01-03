#pragma once
#include <kernel/deserialize.hpp>
#include <kernel/serialize.hpp>

namespace kernel {
    class Sout {
      public:
        static const std::array<byte, 8> query;

        static bool isSout(const vector<byte> &bytes);
        static bool isSout(const string &path);
        static Function load(const vector<byte> &bytes);
        static Function load(const string &path);
        static vector<byte> dump(const Function &func);
    };
} // namespace kernel
