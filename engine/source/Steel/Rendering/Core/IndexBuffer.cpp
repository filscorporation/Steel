#include "IndexBuffer.h"

void IndexBuffer::Create(uint32_t* data, uint32_t size)
{
    Clear();
    Data = data;
    Size = size;
}

void IndexBuffer::Clear()
{
    delete[] Data;
    Data = nullptr;
    Size = 0;
}

bool IndexBuffer::IsEmpty()
{
    return Size == 0;
}

void IndexBuffer::Bind()
{

}

void IndexBuffer::Unbind()
{

}
