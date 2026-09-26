#pragma once
#include <vector>

#include "utils.h"

struct DynamicBitset {
    std::vector<uint8_t> bits;
    int nBits = 0;
    void resize(int n) {
        nBits = n;
        bits.resize(((n-1) >> 3)+1);
    }
    [[nodiscard]] uint32_t slice(int start, int n) const {
        int end = start + n - 1;

        int startChunk = start >> 3;
        int startBits = 8 - (start & 7);
        auto startMask = Utils::bitmask<uint8_t>(startBits);

        int endChunk = end >> 3;
        int endBits = (end & 7) + 1;
        auto endMask = Utils::bitmask<uint8_t>(endBits);

        uint32_t res = bits[startChunk] & startMask;
        for (int i=startChunk + 1; i<endChunk; i++) {
            res <<= 8;
            res |= bits[i];
        }

        int trim = 8 - endBits;
        if (startChunk == endChunk) {
            res >>= trim;
            return res;
        }
        res <<= endBits;
        res |= bits[endChunk] >> (8-endBits) & endMask;
        return res;
    }

    static uint8_t hexToInt(char ch) {

        uint8_t val;
        if (ch >= 'A' && ch <= 'F') {
            return ch - 'A' + 10;
        }
        if (ch >= 'a' && ch <= 'f') {
            return ch - 'a' + 10;
        }
        if (ch >= '0' && ch <= '9') {
            return ch - '0';
        }
        return 0;
    }
    static char intToHex(uint8_t val) {
        if (val <= 9) {
            return (char)(val + '0');
        }
        return (char)(val - 10 + 'A');
    }
    template <std::unsigned_integral T>
    void setData (T val, int start, int valBits = std::numeric_limits<T>::digits) {

        int end = start + valBits - 1;
        bool startAligns = (start & 7) == 0;
        bool endAligns = (end & 7) == 7;

        int startChunk = start >> 3;
        int startBits = 8 - (start & 7);
        auto startMask = Utils::bitmask<uint8_t>(startBits);

        int endChunk = end >> 3;
        int endBits = (end & 7) + 1;
        auto endMask = Utils::bitmask<uint8_t>(endBits);

        if (startChunk == endChunk) {
            if (startAligns && endAligns) {
                bits[startChunk] = val;
                return;
            }
            int trim = 8 - endBits;
            startMask <<= trim;
            bits[startChunk] &= ~startMask;
            bits[startChunk] |= startMask & (val << trim);
            return;
        }

        if (startAligns) bits[startChunk] = val >> (valBits - startBits);
        else {
            bits[startChunk] &= ~startMask;
            bits[startChunk] |= startMask & (val >> (valBits - startBits));
        }

        for (int i=startChunk + 1; i<endChunk; i++) {
            bits[i] = val >> (endBits + ((endChunk - i - 1) << 3));
        }

        if (endAligns) bits[endChunk] = val >> (valBits - endBits);
        else {
            bits[endChunk] &= ~ (endMask << (8 - endBits));
            bits[endChunk] |= (val & endMask) << (8 - endBits);
        }
    }
    void setByte(uint8_t val, int byteCell) {
        bits[byteCell] = val;
    }
    void setAtByteStart(uint8_t val, int byteCell, int valBits) {
        uint8_t& cell = bits[byteCell];
        if (cell == 0) cell = val;
        else {
            auto mask = Utils::bitmask<uint8_t>(valBits);
            cell &= ~mask;
            cell |= val & mask;
        }
    }
    void setAtByteEnd(uint8_t val, int byteCell, int valBits) {
        uint8_t& cell = bits[byteCell];
        val <<= 8-valBits;
        if (cell == 0) cell = val;
        else {
            auto mask = Utils::bitmask<uint8_t>(valBits) << (8-valBits);
            cell &= ~mask;
            cell |= val & mask;
        }
    }
    void setHex(uint8_t val, int pos) {
        setData(val, pos, 4);
    }
    void setCharHex(char val, int pos) {
        setData(hexToInt(val), pos, 4);
    }
    void pushCharHex(char val) {
        uint8_t num = hexToInt(val);
        if ((nBits & 7) == 0) {
            bits.emplace_back();
            nBits+=4;
            setAtByteEnd(num, bits.size() - 1, 4);
            return;
        }
        if ((nBits & 7) == 4) {
            nBits+=4;
            setAtByteStart(num, bits.size() - 1, 4);
            return;
        }
        int start = nBits;
        resize(nBits+4);
        setData(num, start, 4);
    }

    void setBit(int pos, bool val) {
        uint8_t mask = 1 << ((7 - pos) & 7);
        val ? (bits[pos >> 3] |= mask) : (bits[pos >> 3] &= ~mask);
    }
    uint8_t operator[](int pos) const {
        return (bits[pos >> 3] >> ((7 - pos) & 7)) & 1;
    }

    void printBin() const {
        for (int i=0; i<nBits; i++) {
            int offset = 7 - (i & 7);
            auto val = bits[i >> 3];
            std::cout << ((val >> offset) & 1);
        }
        std::cout << std::endl;
    }
    void printHex() const {
        int nChars = (nBits-1)/4 + 1;
        for (int i=0; i<nChars; i++) {
            auto val = bits[i >> 1];
            if ((i & 1) == 1) {
                std::cout << intToHex(val & 15);
            }
            else {
                std::cout << intToHex(val >> 4);
            }
        }
        std::cout << std::endl;
    }
};
