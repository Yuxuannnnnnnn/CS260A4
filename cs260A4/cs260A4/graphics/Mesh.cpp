#include "Mesh.h"
#include "OpenGLErrorCheck.h"

namespace Graphics
{
  class Mesh::VertexBufferLayout
  {
  public:
    struct VertexBufferElement
    {
      unsigned type;
      unsigned count;
      unsigned char normalized;

      static unsigned GetSizeOfType(unsigned type)
      {
        switch (type)
        {
        case GL_FLOAT: return 4;
        case GL_UNSIGNED_INT: return 4;
        case GL_UNSIGNED_BYTE: return 1;
        }
        ASSERT(false);
        return 0;
      }
    };

  private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned m_Stride;
  public:
    VertexBufferLayout() : m_Stride{ 0 }
    {
    }

    template <typename T>
    void Push(unsigned count)
    {
      static_assert(false);
    }

    template <>
    void Push<float>(unsigned count)
    {
      m_Elements.push_back({ GL_FLOAT, count, false });
      m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    template <>
    void Push<unsigned>(unsigned count)
    {
      m_Elements.push_back({ GL_UNSIGNED_INT, count, false });
      m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template <>
    void Push<unsigned char>(unsigned count)
    {
      m_Elements.push_back({ GL_UNSIGNED_BYTE, count, true });
      m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    const std::vector<VertexBufferElement> GetElements() const
    {
      return m_Elements;
    }

    unsigned GetStride() const
    {
      return m_Stride;
    }

    void Clear()
    {
      m_Elements.clear();
      m_Stride = 0;
    }
  };

  Mesh::VertexBuffer::VertexBuffer()
  {
    GLCall(glGenBuffers(1, &rendererID));
  }

  Mesh::VertexBuffer::~VertexBuffer()
  {
    GLCall(glDeleteBuffers(1, &rendererID));
  }

  void Mesh::VertexBuffer::InitData(const float* data, unsigned size, bool dynamic)
  {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
  }

  void Mesh::VertexBuffer::InitSubData(const float* data, unsigned offset, unsigned size)
  {
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
  }

  void Mesh::VertexBuffer::Bind() const
  {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
  }

  void Mesh::VertexBuffer::Unbind() const
  {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  }

  Mesh::VertexArray::VertexArray()
  {
    GLCall(glGenVertexArrays(1, &rendererID));
  }

  Mesh::VertexArray::~VertexArray()
  {
    GLCall(glDeleteVertexArrays(1, &rendererID));
  }

  void Mesh::VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, bool instanced, int startCount)
  {
    if (startCount == -1)
      startCount = vertexAttribArrayCount;
    else
      vertexAttribArrayCount = startCount;

    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned offset = 0;
    for (unsigned i = 0; i < elements.size(); ++i)
    {
      const auto& element = elements[i];
      GLCall(glEnableVertexAttribArray(i + startCount));
      void const* pointer = static_cast<char const*>(nullptr) + offset;
      GLCall(
        glVertexAttribPointer(
          i + startCount,
          element.count,
          element.type,
          element.normalized,
          layout.GetStride(),
          static_cast<const void*>(pointer)));
      offset += element.count * VertexBufferLayout::VertexBufferElement::GetSizeOfType(element.type);
      GLCall(glVertexAttribDivisor(i + startCount, instanced)); // tell OpenGL this is an instanced vertex attribute
    }
    vertexAttribArrayCount += static_cast<unsigned>(elements.size());
  }

  void Mesh::VertexArray::Bind() const
  {
    GLCall(glBindVertexArray(rendererID));
  }

  void Mesh::VertexArray::Unbind() const
  {
    GLCall(glBindVertexArray(0));
  }

  Mesh::IndexBuffer::IndexBuffer()
    : count{ 0 }
  {
    GLCall(glGenBuffers(1, &rendererID));
  }

  Mesh::IndexBuffer::~IndexBuffer()
  {
    GLCall(glDeleteBuffers(1, &rendererID));
  }

  void Mesh::IndexBuffer::InitData(const unsigned* data, unsigned size, bool dynamic)
  {
    count = size / sizeof(unsigned);
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
    GLCall(
      glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        size,
        data,
        dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
  }

  void Mesh::IndexBuffer::Bind() const
  {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
  }

  void Mesh::IndexBuffer::Unbind() const
  {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  }

  unsigned Mesh::IndexBuffer::GetCount() const
  {
    return count;
  }

  std::vector<float> Mesh::VerticesDataFormat()
  {
    AssertProperties();
    std::vector<float> coords;
    for (size_t i = 0; i < verticesCount; ++i)
    {
      //vertex position
      coords.push_back(vertices[i].x);
      coords.push_back(vertices[i].y);
      coords.push_back(vertices[i].z);

      //color
      if (colors.size())
      {
        coords.push_back(colors[i].r);
        coords.push_back(colors[i].g);
        coords.push_back(colors[i].b);
        coords.push_back(colors[i].a);
      }
      else
      {
        coords.push_back(1);
        coords.push_back(1);
        coords.push_back(1);
        coords.push_back(1);
      }
      //texture coordinates
      if (texCoords.size())
      {
        coords.push_back(texCoords[i].x);
        coords.push_back(texCoords[i].y);
      }
      else
      {
        coords.push_back(0);
        coords.push_back(0);
      }
      //normals
      if (normals.size())
      {
        coords.push_back(normals[i].x);
        coords.push_back(normals[i].y);
        coords.push_back(normals[i].z);
      }
      else
      {
        coords.push_back(0);
        coords.push_back(0);
        coords.push_back(0);
      }
    }
    return coords;
  }

  void Mesh::AssertProperties()
  {
    verticesCount = static_cast<unsigned>(vertices.size());
    //ensure there are enough colors for each vertex
    if (/*colors.size() &&*/ colors.size() < verticesCount)
    {
      for (size_t i = colors.size(); i < verticesCount; ++i)
      {
        colors.push_back(Color());
      }
    }
    //ensure there are enough texcoords for each vertex
    if (/*texCoords.size() &&*/ texCoords.size() < verticesCount)
    {
      for (size_t i = texCoords.size(); i < verticesCount; ++i)
      {
        texCoords.push_back(Vector2());
      }
    }
    //ensure there are enough normals for each vertex
    if (normals.size() && normals.size() < verticesCount)
    {
      for (size_t i = normals.size(); i < verticesCount; ++i)
      {
        normals.push_back(Vector3());
      }
    }
  }

  Mesh::Mesh()
  {
    AssertProperties();

    vao.Bind();
    vbo.InitData(
      VerticesDataFormat().data(),
      static_cast<unsigned>(vertices.size() * 12 * sizeof(float)),
      isDynamic);

    if (indices.size())
      ibo.InitData(indices.data(), static_cast<unsigned>(indices.size() * sizeof(unsigned)), isDynamic);

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(4);
    layout.Push<float>(2);
    layout.Push<float>(3);
    vao.AddBuffer(vbo, layout);

    if (indices.size())
      ibo.Bind();

    vao.Unbind();
    vbo.Unbind();
    if (indices.size())
      ibo.Unbind();
  }

  void Mesh::SetVertices(std::vector<Vector3> v)
  {
    vertices = v;
    AssertProperties();
    vbo.InitData(VerticesDataFormat().data(), static_cast<unsigned>(vertices.size() * 12 * sizeof(float)));
  }

  void Mesh::SetColors(std::vector<Color> c)
  {
    colors = c;
    AssertProperties();
    vbo.InitData(VerticesDataFormat().data(), static_cast<unsigned>(vertices.size() * 12 * sizeof(float)));
  }

  void Mesh::SetUVs(std::vector<Vector2> v)
  {
    texCoords = v;
    AssertProperties();
    vbo.InitData(VerticesDataFormat().data(), static_cast<unsigned>(vertices.size() * 12 * sizeof(float)));
  }

  void Mesh::SetNormals(std::vector<Vector3> v)
  {
    normals = v;
    AssertProperties();
    vbo.InitData(VerticesDataFormat().data(), static_cast<unsigned>(vertices.size() * 12 * sizeof(float)));
  }

  void Mesh::SetIndices(std::vector<unsigned> i)
  {
    indices = i;

    if (indices.size())
      ibo.InitData(indices.data(), static_cast<unsigned>(indices.size() * sizeof(unsigned)));

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(4);
    layout.Push<float>(2);
    layout.Push<float>(3);
    vao.AddBuffer(vbo, layout);

    if (indices.size())
      ibo.Bind();
  }

  void Mesh::Draw()
  {
    if (indices.size())
    {
      ibo.InitData(indices.data(), static_cast<unsigned>(indices.size() * sizeof(unsigned)), isDynamic);
      ibo.Bind();
    }

    vbo.InitData(
      VerticesDataFormat().data(),
      static_cast<unsigned>(vertices.size() * 12 * sizeof(float)),
      isDynamic);
    vbo.Bind();
    vbo.Unbind();

    if (useInstancing)
    {
      ivbo.InitData(
        instanceData.data(),
        static_cast<unsigned>(instanceData.size() * sizeof(float)),
        isDynamic);
      ivbo.Bind();
      ivbo.Unbind();
    }

    vao.Bind();

    if (indices.size())
    {
      if (useInstancing)
      {
        GLCall(glDrawElementsInstanced(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr, instances));
      }
      else
      {
        GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
      }
    }
    else
    {
      if (useInstancing)
      {
        GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, static_cast<unsigned>(vertices.size()), instances));
      }
      else
      {
        GLCall(glDrawArrays(GL_TRIANGLES, 0, static_cast<unsigned>(vertices.size())));
      }
    }

    vao.Unbind();
    if (indices.size())
      ibo.Unbind();
  }

  Mesh* quad;
  Mesh* cube;
  void Mesh::Init()
  {
    std::vector<unsigned> inds;

    quad = new Mesh;
    cube = new Mesh;

    quad->vertices.resize(4);
    quad->texCoords.resize(4);
    quad->colors.resize(4);
    quad->normals.resize(4);
    
    inds.push_back(0); inds.push_back(1); inds.push_back(2);
    inds.push_back(2); inds.push_back(3); inds.push_back(0);
    quad->SetIndices(inds);
    
    inds.clear();

    cube->vertices.resize(24);
    cube->texCoords.resize(24);
    cube->colors.resize(24);
    cube->normals.resize(24);

    inds.push_back(0); inds.push_back(1); inds.push_back(2);
    inds.push_back(2); inds.push_back(3); inds.push_back(0);
    
    inds.push_back(4); inds.push_back(5); inds.push_back(6);
    inds.push_back(6); inds.push_back(7); inds.push_back(4);
    
    inds.push_back(8); inds.push_back(9); inds.push_back(10);
    inds.push_back(10); inds.push_back(11); inds.push_back(8);
    
    inds.push_back(12); inds.push_back(13); inds.push_back(14);
    inds.push_back(14); inds.push_back(15); inds.push_back(12);
    
    inds.push_back(16); inds.push_back(17); inds.push_back(18);
    inds.push_back(18); inds.push_back(19); inds.push_back(16);
    
    inds.push_back(20); inds.push_back(21); inds.push_back(22);
    inds.push_back(22); inds.push_back(23); inds.push_back(20);
    cube->SetIndices(inds);
  }
  void Mesh::Exit()
  {
    delete quad;
    delete cube;
  }

  void Mesh::DrawQuad(Vector2 offset, Vector2 tiling, Vector2 pivot, bool wireframe)
  {
    quad->vertices[0] = Vector3(0.f, 1.f, 0.f) - pivot;
    quad->vertices[1] = Vector3(1.f, 1.f, 0.f) - pivot;
    quad->vertices[2] = Vector3(1.f, 0.f, 0.f) - pivot;
    quad->vertices[3] = Vector3(0.f, 0.f, 0.f) - pivot;

    quad->texCoords[0] = Vector2(offset.x, offset.y);
    quad->texCoords[1] = Vector2(offset.x + tiling.x, offset.y);
    quad->texCoords[2] = Vector2(offset.x + tiling.x, offset.y + tiling.y);
    quad->texCoords[3] = Vector2(offset.x, offset.y + tiling.y);

    wireframe ? glDisable(GL_BLEND) : glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    quad->Draw();
  }

  void Mesh::DrawCube(Vector3 offset, Vector3 tiling, Vector3 pivot, bool wireframe)
  {
    // front
    cube->vertices[0] = Vector3(0.f, 1.f, 1.f) - pivot;
    cube->vertices[1] = Vector3(1.f, 1.f, 1.f) - pivot;
    cube->vertices[2] = Vector3(1.f, 0.f, 1.f) - pivot;
    cube->vertices[3] = Vector3(0.f, 0.f, 1.f) - pivot;
    // top
    cube->vertices[4] = Vector3(0.f, 1.f, 1.f) - pivot;
    cube->vertices[5] = Vector3(1.f, 1.f, 1.f) - pivot;
    cube->vertices[6] = Vector3(1.f, 1.f, 0.f) - pivot;
    cube->vertices[7] = Vector3(0.f, 1.f, 0.f) - pivot;
    // back
    cube->vertices[8]  = Vector3(0.f, 1.f, 0.f) - pivot;
    cube->vertices[9]  = Vector3(1.f, 1.f, 0.f) - pivot;
    cube->vertices[10] = Vector3(1.f, 0.f, 0.f) - pivot;
    cube->vertices[11] = Vector3(0.f, 0.f, 0.f) - pivot;
    // down
    cube->vertices[12] = Vector3(0.f, 0.f, 0.f) - pivot;
    cube->vertices[13] = Vector3(1.f, 0.f, 0.f) - pivot;
    cube->vertices[14] = Vector3(1.f, 0.f, 1.f) - pivot;
    cube->vertices[15] = Vector3(0.f, 0.f, 1.f) - pivot;
    // left
    cube->vertices[16] = Vector3(0.f, 1.f, 1.f) - pivot;
    cube->vertices[17] = Vector3(0.f, 1.f, 0.f) - pivot;
    cube->vertices[18] = Vector3(0.f, 0.f, 0.f) - pivot;
    cube->vertices[19] = Vector3(0.f, 0.f, 1.f) - pivot;
    // right
    cube->vertices[20] = Vector3(1.f, 1.f, 0.f) - pivot;
    cube->vertices[21] = Vector3(1.f, 1.f, 1.f) - pivot;
    cube->vertices[22] = Vector3(1.f, 0.f, 1.f) - pivot;
    cube->vertices[23] = Vector3(1.f, 0.f, 0.f) - pivot;


    cube->normals[0] = Vector3::Back();
    cube->normals[1] = Vector3::Back();
    cube->normals[2] = Vector3::Back();
    cube->normals[3] = Vector3::Back();

    cube->normals[4] = Vector3::Up();
    cube->normals[5] = Vector3::Up();
    cube->normals[6] = Vector3::Up();
    cube->normals[7] = Vector3::Up();

    cube->normals[8] =  Vector3::Forward();
    cube->normals[9] =  Vector3::Forward();
    cube->normals[10] = Vector3::Forward();
    cube->normals[11] = Vector3::Forward();

    cube->normals[12] = Vector3::Down();
    cube->normals[13] = Vector3::Down();
    cube->normals[14] = Vector3::Down();
    cube->normals[15] = Vector3::Down();

    cube->normals[16] = Vector3::Left();
    cube->normals[17] = Vector3::Left();
    cube->normals[18] = Vector3::Left();
    cube->normals[19] = Vector3::Left();

    cube->normals[20] = Vector3::Right();
    cube->normals[21] = Vector3::Right();
    cube->normals[22] = Vector3::Right();
    cube->normals[23] = Vector3::Right();

    wireframe ? glDisable(GL_BLEND) : glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    cube->Draw();
  }
}
