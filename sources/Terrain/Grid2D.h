#pragma once

#include <vector>
#include "Util.h"

/**
 * Class abstracting a vector to store a 2d grid. As y/row major
 */
template<typename T>
class Grid2D
{
private:
    std::vector<T> mGrid;
    Vec2I mSize;
    size_t mItSize;
    
public:
    explicit Grid2D(const Vec2I size) : mGrid(size.x * size.y), mSize(size), mItSize(size.x * size.y) {}
    ~Grid2D() = default;
    
    constexpr void resize(const Vec2I size, T clearValue) noexcept { mSize = size; mItSize = (size_t)size.x * size.y; clear(clearValue); }
    constexpr void clear(T clearValue) noexcept { mGrid.assign(mItSize, clearValue); }

    [[nodiscard]] constexpr Vec2I getSize() const noexcept { return mSize; }
    [[nodiscard]] constexpr size_t getItSize() const noexcept { return mItSize; }

    [[nodiscard]] constexpr size_t vec2ToIndex(const Vec2I vec) const noexcept { return vec.y * mSize.x + vec.x; }
    [[nodiscard]] constexpr size_t posToIndex(const int x, const int y) const noexcept { return y * mSize.x + x; }
    [[nodiscard]] constexpr Vec2I indexToVec2(const size_t index) const noexcept { return Vec2I{ (int)index % mSize.x, (int)index / mSize.x }; }
    
    constexpr T& operator[](const Vec2I pos) noexcept { return mGrid[vec2ToIndex(pos)]; }
    constexpr const T& operator[](const Vec2I pos) const noexcept { return mGrid[vec2ToIndex(pos)]; }
    
    constexpr T& at(const int x, const int y) noexcept { return mGrid[posToIndex(x, y)]; }
    constexpr const T& at(const int x, const int y) const noexcept { return mGrid[posToIndex(x, y)]; }
    
    constexpr T& operator[](const size_t index) noexcept { return mGrid[index]; }
    constexpr const T& operator[](const size_t index) const noexcept { return mGrid[index]; }
};
