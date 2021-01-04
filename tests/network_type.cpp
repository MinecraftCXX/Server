#include <array>
#include <cstdint>
#include <string>

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
    unsigned char testBuffer = 0x01;

    minecraft::net::boolean_type integer;
    integer.read(&testBuffer, 1);
    EXPECT_TRUE(integer == true);

    ArrayWrapper<unsigned char, 1> outBuffer;
    integer.write(outBuffer);
    EXPECT_TRUE(testBuffer == outBuffer.buffer[0]);
}

template <typename IntegerType>
inline void testIngeger() {
    IntegerType integer = -1;
    ArrayWrapper<unsigned char, sizeof(IntegerType)> outBuffer;
    integer.write(outBuffer);
    integer.read(outBuffer.buffer.data(), outBuffer.buffer.size());
    EXPECT_TRUE(integer == static_cast<typename IntegerType::value_type>(-1));
}

TEST(NetworkSerialization, Integer) {
    testIngeger<minecraft::net::uint8_type>();
    testIngeger<minecraft::net::int8_type>();
    testIngeger<minecraft::net::uint16_type>();
    testIngeger<minecraft::net::int16_type>();
    testIngeger<minecraft::net::int32_type>();
    testIngeger<minecraft::net::int64_type>();
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

TEST(NetworkSerialization, String) {
    const char* testBuffer = "Test";
    minecraft::net::string_type string = testBuffer;
    ArrayWrapper<unsigned char, 5 + 4> outBuffer;
    string.write(outBuffer);
    string.read(outBuffer.buffer.data(), outBuffer.buffer.size());
}