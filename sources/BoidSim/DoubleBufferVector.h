#pragma once
#include <vector>

template <typename T>
class DoubleBufferVector
{
private:
    std::vector<T> mCurrentVector; // Reads out
    std::vector<T> mNextVector; // Write in
public:
    DoubleBufferVector() = default;
    
    [[nodiscard]] const T& readCurrent(size_t index) const { return mCurrentVector[index]; }
    [[nodiscard]] const T& readNext(size_t index) const { return mNextVector[index]; }
    
    void writeCurrent(size_t index, const T& value) { mNextVector[index] = value; }
    void writeNext(size_t index, const T& value) { mNextVector[index] = value; }

    [[nodiscard]] const std::vector<T>& readVector() const { return mCurrentVector; }
    
    void swap() { mCurrentVector.swap(mNextVector); }
    
    void copy()
    {
        for (size_t i = 0; i < mNextVector.size(); ++i)
        {
            mCurrentVector[i] = mNextVector[i];
        }
    }
    
    [[nodiscard]] size_t size() const { return mCurrentVector.size(); }
    void resize(size_t newSize)
    {
        mCurrentVector.resize(newSize);
        mNextVector.resize(newSize);
    }
    
};
