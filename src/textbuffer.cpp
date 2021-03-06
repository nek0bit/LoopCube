#include "textbuffer.hpp"

TextBuffer::TextBuffer(const std::string& initialBuffer)
    : buf{initialBuffer},
      col{0}
{}

TextBuffer::~TextBuffer() {}

void TextBuffer::push(const char data)
{
    buf.push_back(data);
}

void TextBuffer::push(const std::string& data)
{
    // TODO
}

void TextBuffer::backspace()
{
    if (col == 0) return;
    buf.erase(col - 1, 1);
    col--;
}

void TextBuffer::insert(const char data, bool increaseCol)
{
    if (col > buf.size()) push(data);
    else buf.insert(buf.begin() + col, data);

    if (increaseCol) col++;
}

void TextBuffer::moveCursor(int move) noexcept
{
    int result = col + move;
    if (result < 0)
    {
        col = 0;
    }
    else if (static_cast<size_t>(result) > buf.size())
    {
        col = buf.size();
    }
    else
    {
        col = result;
    }
}
