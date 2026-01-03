#pragma once
#include <kernel/exception.hpp>
#include <kernel/function.hpp>
#include <kernel/object.hpp>
#include <kernel/runtime/state.hpp>
#include <kernel/typedef.hpp>
#include <kernel/variant.hpp>
#include <type_traits>

namespace kernel {
    template <typename T = Variant> T deserialize(const vector<byte> &bytes);

    template <typename T = Variant, typename Enabled = void>
    struct Deserialize {
        T operator()(vector<byte>::const_iterator &begin) const {
            throw NoImplementException(__FUNCTION__);
        }
    };

    template <typename T>
    struct Deserialize<
        T, typename std::enable_if<std::is_trivial<T>::value>::type> {
        T operator()(vector<byte>::const_iterator &begin) const {
            T result;
            for (uinteger i = 0; i < sizeof(T); ++i)
                (reinterpret_cast<byte *>(&result))[i] = *begin++;
            return result;
        }
    };
    template <typename T> struct Deserialize<Ref<T>> {
        Ref<T> operator()(vector<byte>::const_iterator &begin) const {
            return new T{Deserialize<T>()(begin)};
        }
    };

    template <> struct Deserialize<string> {
        string operator()(vector<byte>::const_iterator &begin) const {
            uinteger length = Deserialize<uinteger>()(begin);
            string result;
            result.resize(length);
            for (uinteger i = 0; i < length; ++i)
                result[i] = *begin++;
            return result;
        }
    };

    template <typename T> struct Deserialize<vector<T>> {
        vector<T> operator()(vector<byte>::const_iterator &begin) const {
            uinteger size = Deserialize<uinteger>()(begin);
            vector<T> result;
            result.resize(size);
            for (uinteger i = 0; i < size; ++i)
                result[i] = Deserialize<T>()(begin);
            return result;
        }
    };

    template <typename K, typename V, typename C, typename A>
    struct Deserialize<map<K, V, C, A>> {
        map<K, V, C, A> operator()(vector<byte>::const_iterator &begin) const {
            uinteger size = Deserialize<uinteger>()(begin);
            map<K, V, C, A> result;
            for (uinteger i = 0; i < size; ++i) {
                K key = Deserialize<K>()(begin);
                V value = Deserialize<V>()(begin);
                result.insert(std::make_pair(key, value));
            }
            return result;
        }
    };

    template <typename K, typename V, typename H, typename P, typename A>
    struct Deserialize<unordered_map<K, V, H, P, A>> {
        unordered_map<K, V, H, P, A>
        operator()(vector<byte>::const_iterator &begin) const {
            uinteger size = Deserialize<uinteger>()(begin);
            unordered_map<K, V, H, P, A> result;
            for (uinteger i = 0; i < size; ++i) {
                K key = Deserialize<K>()(begin);
                V value = Deserialize<V>()(begin);
                result.insert(std::make_pair(key, value));
            }
            return result;
        }
    };

    template <> struct Deserialize<Ref<Object>> {
        Ref<Object> operator()(vector<byte>::const_iterator &begin) const {
            string className = Deserialize<string>()(begin);
            map<string, Ref<Variant>> properties =
                Deserialize<map<string, Ref<Variant>>>()(begin);
            Ref<Object> result = Object::makeObj(className);
            result->properties(properties);
            return result;
        }
    };

    template <> struct Deserialize<Object> {
        Object operator()(vector<byte>::const_iterator &begin) const {
            return Object{*Deserialize<Ref<Object>>()(begin)};
        }
    };

    template <> struct Deserialize<State> {
        State operator()(vector<byte>::const_iterator &begin) const {
            Ref<State::gcp_t> gcp = Deserialize<Ref<State::gcp_t>>()(begin);
            CodeSeq codes = Deserialize<CodeSeq>()(begin);
            unordered_map<arg_t, arg_t> labels =
                Deserialize<unordered_map<arg_t, arg_t>>()(begin);
            vector<arg_t> lines = Deserialize<vector<arg_t>>()(begin);
            Ref<string> chunk = Deserialize<Ref<string>>()(begin);
            State result;
            result.setGCP(gcp);
            result.getCodes() = std::move(codes);
            result.labels(std::move(labels));
            result.lines(std::move(lines));
            result.chunk(std::move(chunk));
            return result;
        }
    };

    template <> struct Deserialize<Method> {
        Method operator()(vector<byte>::const_iterator &begin) const {
            State state = Deserialize<State>()(begin);
            vector<Ref<string>> binds =
                Deserialize<vector<Ref<string>>>()(begin);
            return Method{new State{std::move(state)},
                          new vector<Ref<string>>{std::move(binds)}};
        }
    };

    template <> struct Deserialize<Function> {
        Function operator()(vector<byte>::const_iterator &begin) const {
            byte flag = *begin++;
            // builtin
            if (flag == 0)
                throw NoImplementException(__FUNCTION__);
            else if (flag == 1) {
                Method method = Deserialize<Method>()(begin);
                map<uinteger, Ref<Variant>> binds =
                    Deserialize<map<uinteger, Ref<Variant>>>()(begin);
                map<string, Ref<Variant>> properties =
                    Deserialize<map<string, Ref<Variant>>>()(begin);
                Function result{method};
                result.binds(binds);
                result.properties(properties);
                return result;
            }
            throw NoImplementException(__FUNCTION__);
        }
    };

    template <> struct Deserialize<Variant> {
        Variant operator()(vector<byte>::const_iterator &begin) const {
            Variant::Type type = (Variant::Type)*begin++;
            switch (type) {
            case Variant::Type::NIL: {
                return Variant{};
            }
            case Variant::Type::INT: {
                return Deserialize<integer>()(begin);
            }
            case Variant::Type::REAL: {
                return Deserialize<real>()(begin);
            }
            case Variant::Type::BOOL: {
                return Deserialize<bool>()(begin);
            }
            case Variant::Type::STRING: {
                return Deserialize<string>()(begin);
            }
            case Variant::Type::ARRAY: {
                return Deserialize<array>()(begin);
            }
            case Variant::Type::DICTIONARY: {
                return Deserialize<dict>()(begin);
            }
            case Variant::Type::BORROWED_OBJECT:
            case Variant::Type::OBJECT: {
                return Deserialize<Object>()(begin);
            }
            case Variant::Type::FUNCTION: {
                return Deserialize<Function>()(begin);
            }
            case Variant::Type::STRING_VIEW: {
                string str = Deserialize<string>()(begin);
                return *Variant::fromStringView(std::string_view(str));
            }
            case Variant::Type::BYTE_ARRAY: {
                auto byteArray = Deserialize<std::vector<uint8_t>>()(begin);
                return *Variant::fromByteArray(byteArray);
            }
            case Variant::Type::DATE_TIME: {
                uinteger timestamp = Deserialize<uinteger>()(begin);
                auto timePoint = std::chrono::system_clock::time_point(
                    std::chrono::microseconds(timestamp));
                return *Variant::fromDateTime(timePoint);
            }
            case Variant::Type::REGEX: {
                string pattern = Deserialize<string>()(begin);
                try {
                    std::regex regex(pattern);
                    return *Variant::fromRegex(regex);
                } catch (...) {
                    throw RuntimeException(
                        "Invalid regex pattern during deserialization.");
                }
            }
            default:
                throw RuntimeException("Deserialize<Variant> bad branch.");
            }
        }
    };

    template <typename T> T deserialize(const vector<byte> &bytes) {
        auto temp = bytes.cbegin();
        return Deserialize<T>()(temp);
    }

} // namespace kernel