#include <array>
#include <cstdint>

#include <gtest/gtest.h>

#include "minecraft/network/type.hpp"

template <typename T, size_t size>
struct ArrayWrapper {
    std::array<T, size> buffer{};
    size_t index = 0;

    ArrayWrapper& operator<<(T value) {
        buffer[index++] = value;
        return *this;
    }
};

TEST(NetworkSerialization, Boolean) {

}

TEST(NetworkSerialization, VarInt) {
    std::array<uint8_t, 5> testBuffer = {255, 255, 255, 255, 7};

    minecraft::net::varint32_type integer;
    integer.read(testBuffer.data(), testBuffer.size());

    EXPECT_TRUE(integer == 2147483647);

    ArrayWrapper<uint8_t, 5> outBuffer;
    integer.write(outBuffer);

    EXPECT_TRUE(outBuffer.buffer == testBuffer);
}

TEST(NetworkSerialization, VarLong) {
    std::array<uint8_t, 10> testBuffer = {255, 255, 255, 255, 255, 255, 255, 255, 255, 1};

    minecraft::net::varint64_type integer;
    integer.read(testBuffer.data(), testBuffer.size());

    EXPECT_TRUE(integer == -1);

    ArrayWrapper<uint8_t, 10> outBuffer;
    integer.write(outBuffer);

    EXPECT_TRUE(outBuffer.buffer == testBuffer);
}