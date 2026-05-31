#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

// I didn't really know how else to write a buffer layout class or the fact that I should keep it separate from the rest of the classes that use it so I'm just using Hazel's implementation,
// Will definitely try to personalize it according to what I think the scope of this project should be. It's definitely not gonna be as large as hazel so I'll see what I can do

// This enum contains the data types of variables that can be in a buffer layout
enum class ShaderDataType : uint8_t{
    NONE,
    FLOAT,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    INT,
    INT2,
    INT3,
    INT4,
    BOOL,
    MAT2X2,
    MAT3X3,
    MAT4X4
};

enum class BufferUsageType : uint8_t{
    STATIC, // For stuff that is updated infrequently
    DYNAMIC,// For frequently changing stuff
    STREAM //For stuff being uploaded every single frame
};

static uint32_t ShaderDataTypeSize(ShaderDataType type){
    switch (type) {
        case ShaderDataType::FLOAT:
            return 4;
        case ShaderDataType::FLOAT2:
            return 4*2;
        case ShaderDataType::FLOAT3:
            return 4*3;
        case ShaderDataType::FLOAT4:
            return 4*4;
        case ShaderDataType::INT:
            return 4;
        case ShaderDataType::INT2:
            return 4*2;
        case ShaderDataType::INT3:
            return 4*3;
        case ShaderDataType::INT4:
            return 4*4;
        case ShaderDataType::BOOL:
            return 1;
        case ShaderDataType::MAT2X2:
            return 4*2*2;
        case ShaderDataType::MAT3X3:
            return 4*3*3;
        case ShaderDataType::MAT4X4:
            return 4*4*4;
        default:
            std::cout << "UNKNOWN_DATA_TYPE\n";
            return 0;
    }
    // Apparently just returning numbers like 12 and 16 is not ideal according to clang-tidy cause they're "magic numbers".
    // Looked it up on google and magic numbers basically make it inconvenient to maintain consistency since you're basically hardcoding stuff and not really providing an explanation for why the function, for example, is returning a number like 12
}

struct BufferElement{
    size_t _offset;
    uint32_t _size;
    std::string _name;
    ShaderDataType _type;
    bool _normalized;

    BufferElement() = default;
    BufferElement(std::string name, ShaderDataType type, bool normalized = false) : _name(std::move(name)), _type(type), _normalized(normalized), _offset(0), _size(ShaderDataTypeSize(type)) {}

    unsigned int getElementCount() const {
        switch (_type) {
            case ShaderDataType::FLOAT:
                return 1;
            case ShaderDataType::FLOAT2:
                return 2;
            case ShaderDataType::FLOAT3:
                return 3;
            case ShaderDataType::FLOAT4:
                return 4;
            case ShaderDataType::INT:
                return 1;
            case ShaderDataType::INT2:
                return 2;
            case ShaderDataType::INT3:
                return 3;
            case ShaderDataType::INT4:
                return 4;
            case ShaderDataType::BOOL:
                return 1;
            case ShaderDataType::MAT2X2:
                // So initially I was just gonna return 2*2 but looked at Hazel and saw I could just do 2*FLOAT2, 3*FLOAT3 etc.
                return 2;
            case ShaderDataType::MAT3X3:
                return 3;
            case ShaderDataType::MAT4X4:
                return 4;
            default:
                return 0;
        }
        return 0;
    }


};

class BufferLayout{
public:
    BufferLayout() = default;

    BufferLayout(std::vector<BufferElement> elements) : _elements(std::move(elements)) {
        calculate_offset_stride();
    }
    BufferLayout(std::initializer_list<BufferElement> elements) : _elements(elements){
        calculate_offset_stride();
    }

    uint32_t getStride() const { return _stride; }

    std::vector<BufferElement>::iterator begin() { return _elements.begin(); }
    std::vector<BufferElement>::iterator end() { return _elements.end(); }
    std::vector<BufferElement>::const_iterator begin() const { return _elements.begin(); }
    std::vector<BufferElement>::const_iterator end() const { return _elements.end(); }

    const std::vector<BufferElement>& getElements() const { return _elements; }

private:
    void calculate_offset_stride(){
        size_t offset = 0;
        _stride = 0;
        for(auto& elem : _elements){
            elem._offset = offset;
            offset += elem._size;
            _stride += elem._size;
        }
    }

    std::vector<BufferElement> _elements;
    uint32_t _stride = 0;
};

// VBO Wrapper
class VertexBuffer{
public:
    virtual ~VertexBuffer() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void setData(const void* data, uint32_t size) = 0;

    virtual const BufferLayout& getLayout() const = 0;
    virtual void setLayout(const BufferLayout& layout) = 0;

    static std::shared_ptr<VertexBuffer> Create(const void* vertices, uint32_t size, BufferUsageType _utype);
};

// EBO Wrapper
class ElementBuffer{
public:
    virtual ~ElementBuffer() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual uint32_t getCount() const = 0;

    static std::shared_ptr<ElementBuffer> Create(uint32_t* indices, uint32_t count, BufferUsageType _utype);
};
