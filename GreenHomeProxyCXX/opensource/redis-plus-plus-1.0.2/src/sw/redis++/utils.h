/**************************************************************************
   Copyright (c) 2017 sewenew

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 *************************************************************************/

#ifndef SEWENEW_REDISPLUSPLUS_UTILS_H
#define SEWENEW_REDISPLUSPLUS_UTILS_H

#include <cstring>
#include <string>
#include <type_traits>

#ifdef HAVE_STRING_VIEW

#include <string_view>

#endif

#ifdef HAVE_OPTIONAL

#include <optional>

#endif

namespace sw {

namespace redis {

#ifdef HAVE_STRING_VIEW

using StringView = std::string_view;

#else

// By now, not all compilers support std::string_view,
// so we make our own implementation.
class StringView {
public:
    constexpr StringView() noexcept = default;

    constexpr StringView(const char *data, std::size_t size) : _data(data), _size(size) {}

    StringView(const char *data) : _data(data), _size(std::strlen(data)) {}

    StringView(const std::string &str) : _data(str.data()), _size(str.size()) {}

    constexpr StringView(const StringView &) noexcept = default;

    StringView& operator=(const StringView &) noexcept = default;

    constexpr const char* data() const noexcept {
        return _data;
    }

    constexpr std::size_t size() const noexcept {
        return _size;
    }

private:
    const char *_data = nullptr;
    std::size_t _size = 0;
};

#endif

#ifdef HAVE_OPTIONAL

template <typename T>
using Optional = std::optional<T>;

#else

template <typename T>
class Optional {
public:
    Optional() = default;

    Optional(const Optional &) = default;
    Optional& operator=(const Optional &) = default;

    Optional(Optional &&) = default;
    Optional& operator=(Optional &&) = default;

    ~Optional() = default;

    template <typename ...Args>
    explicit Optional(Args &&...args) : _value(true, T(std::forward<Args>(args)...)) {}

    explicit operator bool() const {
        return _value.first;
    }

    T& value() {
        return _value.second;
    }

    const T& value() const {
        return _value.second;
    }

    T* operator->() {
        return &(_value.second);
    }

    const T* operator->() const {
        return &(_value.second);
    }

    T& operator*() {
        return _value.second;
    }

    const T& operator*() const {
        return _value.second;
    }

private:
    std::pair<bool, T> _value;
};

#endif

using OptionalString = Optional<std::string>;

using OptionalLongLong = Optional<long long>;

using OptionalDouble = Optional<double>;

using OptionalStringPair = Optional<std::pair<std::string, std::string>>;

template <typename ...>
struct IsKvPair : std::false_type {};

template <typename T, typename U>
struct IsKvPair<std::pair<T, U>> : std::true_type {};

template <typename ...>
using Void = void;

template <typename T, typename U = Void<>>
struct IsInserter : std::false_type {};

template <typename T>
struct IsInserter<T, Void<typename T::container_type>> : std::true_type {};

template <typename Iter, typename T = Void<>>
struct IterType {
    using type = typename std::iterator_traits<Iter>::value_type;
};

template <typename Iter>
//struct IterType<Iter, Void<typename Iter::container_type>> {
struct IterType<Iter,
    typename std::enable_if<std::is_void<typename Iter::value_type>::value>::type> {
    using type = typename std::decay<typename Iter::container_type::value_type>::type;
};

template <typename Iter, typename T = Void<>>
struct IsIter : std::false_type {};

template <typename Iter>
struct IsIter<Iter,
                Void<typename std::iterator_traits<Iter>::iterator_category>> : std::true_type {};

template <typename T>
struct IsKvPairIter : IsKvPair<typename IterType<T>::type> {};

template <typename T, typename Tuple>
struct TupleWithType : std::false_type {};

template <typename T>
struct TupleWithType<T, std::tuple<>> : std::false_type {};

template <typename T, typename U, typename ...Args>
struct TupleWithType<T, std::tuple<U, Args...>> : TupleWithType<T, std::tuple<Args...>> {};

template <typename T, typename ...Args>
struct TupleWithType<T, std::tuple<T, Args...>> : std::true_type {};

uint16_t crc16(const char *buf, int len);

}

}

#endif // end SEWENEW_REDISPLUSPLUS_UTILS_H
