#ifndef GAMESERVER_TYPE_HPP
#define GAMESERVER_TYPE_HPP

#include <cstdint>
#include <stdexcept>
#include <string>

#include "endian.hpp"

namespace minecraft::net {
template<typename T>
struct numeric_type {
    using value_type = T;

    value_type val;

    numeric_type() : val() {}

    numeric_type(value_type val) : val(val) {}

    numeric_type &operator=(T newVal) {
        val = newVal;
    }

    size_t read(const unsigned char *in, size_t length) = delete;

    template<typename Buffer>
    void write(Buffer &out) = delete;

    operator value_type &() {
        return val;
    }
};

template<>
size_t numeric_type<bool>::read(const unsigned char *in, size_t length) {
    val = in[0];
    return 1;
}

template<>
template<typename Buffer>
void numeric_type<bool>::write(Buffer &out) {
    out << (val ? 1 : 0);
}

template<>
size_t numeric_type<uint8_t>::read(const unsigned char *in, size_t length) {
    val = in[0];
    return 1;
}

template<>
template<typename Buffer>
void numeric_type<uint8_t>::write(Buffer &out) {
    out << reinterpret_cast<unsigned char*>(&val)[0];
}

template<>
size_t numeric_type<int8_t>::read(const unsigned char *in, size_t length) {
    val = in[0];
    return 1;
}

template<>
template<typename Buffer>
void numeric_type<int8_t>::write(Buffer &out) {
    out << reinterpret_cast<unsigned char*>(&val)[0];
}

template<>
size_t numeric_type<uint16_t>::read(const unsigned char *in, size_t length) {
    val = ((in[1] << 0u) | (in[0] << 8u));
    return 2;
}

template<>
template<typename Buffer>
void numeric_type<uint16_t>::write(Buffer &out) {
    uint32_t u_val = hostToNetworkInt(val);
    auto *buffer = reinterpret_cast<unsigned char *>(&u_val);
    out << buffer[2] << buffer[3];  // Last 2 bytes of unsigned 32 bit big-endian int
}

template<>
size_t numeric_type<int16_t>::read(const unsigned char *in, size_t length) {
    val = ((in[1] << 0u) | (in[0] << 8u));
    return 2;
}

template<>
template<typename Buffer>
void numeric_type<int16_t>::write(Buffer &out) {
    uint32_t u_val = hostToNetworkInt(val);
    auto *buffer = reinterpret_cast<unsigned char *>(&u_val);
    out << buffer[2] << buffer[3];
}

template<>
size_t numeric_type<int32_t>::read(const unsigned char *in, size_t length) {
    val = ((in[3] << 0u) | (in[2] << 8u) | (in[1] << 16u) | (in[0] << 24u));
    return 4;
}

template<>
template<typename Buffer>
void numeric_type<int32_t>::write(Buffer &out) {
    uint32_t u_val = hostToNetworkLong(val);
    char *buffer = reinterpret_cast<char *>(&u_val);
    out << buffer[0] << buffer[1] << buffer[2] << buffer[3];
}

template<>
size_t numeric_type<int64_t>::read(const unsigned char *in, size_t length) {
#define toLong(x) static_cast<uint64_t>(x)
    val = ((toLong(in[7]) << 0ul)
        | (toLong(in[6]) << 8ul)
        | (toLong(in[5]) << 16ul)
        | (toLong(in[4]) << 24ul)
        | (toLong(in[3]) << 32ul)
        | (toLong(in[2]) << 40ul)
        | (toLong(in[1]) << 48ul)
        | (toLong(in[0]) << 56ul));
    return 8;
}

template<>
template<typename Buffer>
void numeric_type<int64_t>::write(Buffer &out) {
    uint64_t u_val = hostToNetworkLong(val);
    auto *buffer = reinterpret_cast<unsigned char*>(&u_val);
    out << buffer[0]
        << buffer[1]
        << buffer[2]
        << buffer[3]
        << buffer[4]
        << buffer[5]
        << buffer[6]
        << buffer[7];
}

template<typename T>
struct var_type {
    using value_type = T;
    value_type val;

    var_type() : val() {}

    var_type(value_type val) : val(val) {}

    var_type &operator=(value_type newVal) {
        val = newVal;
    }

    size_t read(const unsigned char *in, size_t length) = delete;

    template<typename Buffer>
    void write(Buffer &out) = delete;

    operator value_type &() {
        return val;
    }
};

template<>
size_t var_type<int32_t>::read(const unsigned char *in, size_t length) {
    uint8_t index = 0;
    uint32_t u_val = 0;

    while (true) {
        if (index + 1 > length) throw std::runtime_error("invalid var-int");

        uint8_t byte = in[index];

        uint32_t value = byte & (~0b10000000);
        u_val |= (value << (7 * index));

        // Last bit of group is not set
        if ((byte & 0b10000000) == 0) break;

        if (++index >= 5) {
            throw std::runtime_error("invalid var-int");
        }
    }

    val = u_val;
    return index + 1;
}

template<>
template<typename Buffer>
void var_type<int32_t>::write(Buffer &out) {
    uint32_t u_val = val;
    do {
        unsigned char value = u_val & 0b01111111;
        u_val >>= 7;
        if (u_val != 0) value |= 0b10000000;
        out << value;
    } while (u_val != 0);
}

template<>
size_t var_type<int64_t>::read(const unsigned char *in, size_t length) {
    uint8_t index = 0;
    uint64_t u_val = 0;

    while (true) {
        if (index + 1 > length) throw std::runtime_error("invalid var-int");

        uint8_t byte = in[index];

        uint64_t value = byte & (~0b10000000);
        u_val |= (value << (7 * index));

        // Last bit of group is not set
        if ((byte & 0b10000000) == 0) break;

        if (++index >= 10) {
            throw std::runtime_error("invalid var-int");
        }
    }

    val = u_val;
    return index + 1;
}

template<>
template<typename Buffer>
void var_type<int64_t>::write(Buffer &out) {
    uint64_t u_val = val;
    do {
        unsigned char value = u_val & 0b01111111;
        u_val >>= 7;
        if (u_val != 0) value |= 0b10000000;
        out << value;
    } while (u_val != 0);
}

struct string_type {
    std::string val;

    string_type() : val() {}
    string_type(const char* val) : val(val) {}
    string_type(std::string &&val) : val(std::move(val)) {}

    string_type& operator=(const char* rhs) {
        val = std::string(rhs);
    }

    string_type& operator=(std::string&& rhs) {
        val = std::move(rhs);
    }

    size_t read(const unsigned char *in, size_t length) {
        var_type<int32_t> header;
        size_t read = header.read(in, length);
        in += read;

        val.resize(header + 1); // Termination Character
        memcpy(val.data(), in, header);
        val[val.size()] = '\0';
        return read + header;
    }

    template<typename Buffer>
    void write(Buffer &out) {
        var_type<int32_t> header = val.size();
        header.template write(out);
        out << val.c_str();
    }

    operator std::string &() {
        return val;
    }
};


using boolean_type = numeric_type<bool>;
using uint8_type = numeric_type<uint8_t>;
using int8_type = numeric_type<int8_t>;
using uint16_type = numeric_type<uint16_t>;
using int16_type = numeric_type<int16_t>;
using int32_type = numeric_type<int32_t>;
using int64_type = numeric_type<int64_t>;
using varint64_type = var_type<int64_t>;
using varint32_type = var_type<int32_t>;
}

#endif //GAMESERVER_TYPE_HPP
