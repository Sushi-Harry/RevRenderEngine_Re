#include "opengl_vertex_array.hpp"

static GLenum ShaderDataTypeToOGLType(ShaderDataType type){
    switch (type) {
        case ShaderDataType::FLOAT:
            return GL_FLOAT;
        case ShaderDataType::FLOAT2:
            return GL_FLOAT;
        case ShaderDataType::FLOAT3:
            return GL_FLOAT;
        case ShaderDataType::FLOAT4:
            return GL_FLOAT;
        case ShaderDataType::INT:
            return GL_INT;
        case ShaderDataType::INT2:
            return GL_INT;
        case ShaderDataType::INT3:
            return GL_INT;
        case ShaderDataType::INT4:
            return GL_INT;
        case ShaderDataType::BOOL:
            return GL_BOOL;
        case ShaderDataType::MAT2X2:
            return GL_FLOAT;
        case ShaderDataType::MAT3X3:
            return GL_FLOAT;
        case ShaderDataType::MAT4X4:
            return GL_FLOAT;
    }
    return 0;
}

std::shared_ptr<VertexArray> VertexArray::Create() {
    return std::make_shared<opengl_vertex_array>();
}

opengl_vertex_array::opengl_vertex_array(){
    glGenVertexArrays(1, &_vao);
}

void opengl_vertex_array::bind() const {
    glBindVertexArray(_vao);
}

void opengl_vertex_array::unbind() const {
    glBindVertexArray(0);
}

void opengl_vertex_array::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vb){
    glBindVertexArray(_vao);
    vb->bind();

    const auto& layout = vb->getLayout();
    for(const auto& elem : layout){
        switch (elem._type) {
            case ShaderDataType::FLOAT:
            case ShaderDataType::FLOAT2:
            case ShaderDataType::FLOAT3:
            case ShaderDataType::FLOAT4:
            {
                glEnableVertexAttribArray(_vb_idx);
                glVertexAttribPointer(_vb_idx, elem.getElementCount(), ShaderDataTypeToOGLType(elem._type), elem._normalized ? GL_TRUE : GL_FALSE, layout.getStride(), (const void*)elem._offset);
                _vb_idx++;
                break;
            }
            case ShaderDataType::INT:
            case ShaderDataType::INT2:
            case ShaderDataType::INT3:
            case ShaderDataType::INT4:
            case ShaderDataType::BOOL:
            {
                glEnableVertexAttribArray(_vb_idx);
                glVertexAttribIPointer(_vb_idx, elem.getElementCount(), ShaderDataTypeToOGLType(elem._type), layout.getStride(), (const void*)elem._offset);
                _vb_idx++;
                break;
            }
            case ShaderDataType::MAT2X2:
            case ShaderDataType::MAT3X3:
            case ShaderDataType::MAT4X4:
            {
                uint8_t cnt = elem.getElementCount();
                for(uint8_t i = 0; i < cnt; i++){
                    glEnableVertexAttribArray(_vb_idx);
                    glVertexAttribPointer(_vb_idx, cnt, ShaderDataTypeToOGLType(elem._type), elem._normalized ? GL_TRUE : GL_FALSE, layout.getStride(), (const void*)(elem._offset + (sizeof(float) * cnt * i)));
                    //   | | TO FUTURE ME:
                    //   | |   PAY ATTENTION TO THIS PART WHEN YOU COME BACK TO INSTANCED RENDERING!!!!!!!
                    //   | |
                    //   | | I REPEAT!, DO NOT FORGET THIS PART!!!
                    glVertexAttribDivisor(_vb_idx, 0);
                    _vb_idx++;
                }
                break;
            }
            default:
                std::cout << "UNKNOWN_DATA_TYPE";
                break;
        }
    }

    _vertex_buffers.push_back(vb);
}

void opengl_vertex_array::setElementBuffer(const std::shared_ptr<ElementBuffer>& eb){
    glBindVertexArray(_vao);
    eb->bind();
    _element_buffer = eb;
}

opengl_vertex_array::~opengl_vertex_array(){
    glDeleteVertexArrays(1, &_vao);
}

const std::vector<std::shared_ptr<VertexBuffer>>& opengl_vertex_array::getVertexBuffers() const { return _vertex_buffers; }

const std::shared_ptr<ElementBuffer>& opengl_vertex_array::getElementBuffer() const { return _element_buffer; }
