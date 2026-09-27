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
    
    constexpr void resize(const Vec2I size) { mSize = size; mItSize = (size_t)size.x * size.y; clear(); }
    constexpr void clear() { mGrid.clear(); mGrid.resize(mItSize); }

    [[nodiscard]] constexpr Vec2I getSize() const noexcept { return mSize; }
    [[nodiscard]] constexpr size_t getItSize() const noexcept { return mItSize; }

    [[nodiscard]] constexpr size_t vec2ToIndex(const Vec2I vec) const noexcept { return vec.y * mSize.x + vec.x; }
    [[nodiscard]] constexpr Vec2I indexToVec2(const size_t index) const noexcept { return Vec2I{ (int)index % mSize.y, (int)index / mSize.y }; }
    
    constexpr T& operator[](const Vec2I pos) { return mGrid[vec2ToIndex(pos)]; }
    constexpr const T& operator[](const Vec2I pos) const { return mGrid[vec2ToIndex(pos)]; }
    
    constexpr T& operator[](const size_t index) noexcept { return mGrid[index]; }
    constexpr const T& operator[](const size_t index) const noexcept { return mGrid[index]; }
};
