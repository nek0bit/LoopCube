#pragma once
#include <string>

class TextBuffer
{
private:
    std::string buf; // Starts at 0 
public:
    size_t col;
    
    TextBuffer(const std::string& initialBuffer = "");
    ~TextBuffer();

    inline const std::string& getText() const noexcept { return buf; }

    void push(const char data);
    void push(const std::string& data);
    void insert(const char data, bool increaseCol = true);
    void moveCursor(int move) noexcept;
};
