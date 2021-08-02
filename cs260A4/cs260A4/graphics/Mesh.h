#pragma once

#include "PCH.h"
#include "Math/Math.h"

namespace Graphics
{
  class Mesh
  {
    class VertexBufferLayout;

    class VertexBuffer
    {
    public:
      unsigned rendererID;
      VertexBuffer();
      ~VertexBuffer();

      void InitData(const float* data, unsigned size, bool dynamic = false);
      void InitSubData(const float* data, unsigned offset, unsigned size);

      void Bind() const;
      void Unbind() const;
    };

    class VertexArray
    {
      unsigned vertexAttribArrayCount = 0;
    public:
      unsigned rendererID;
      VertexArray();
      ~VertexArray();

      void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, bool instanced = false, int startCount = -1);

      void Bind() const;
      void Unbind() const;
    };

    class IndexBuffer
    {
    public:
      unsigned rendererID;
      unsigned count;
      IndexBuffer();
      ~IndexBuffer();

      void InitData(const unsigned* data, unsigned size, bool dynamic = false);

      void Bind() const;
      void Unbind() const;

      unsigned GetCount() const;
    };

    VertexArray vao;
    VertexBuffer vbo, ivbo;
    IndexBuffer ibo;

    const unsigned MAX_VERTICES = 65534;

    unsigned verticesCount;

    unsigned instances;
    std::vector<float> instanceData;

    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    std::vector<Vector2> texCoords;
    std::vector<Vector3> normals;
    std::vector<unsigned> indices;

    bool isDynamic = false;
    bool useInstancing = false;
    bool useSubdata = false;

    std::vector<float> VerticesDataFormat();
    void AssertProperties();
  public:
    Mesh();

    void SetVertices(std::vector<Vector3> vertices);
    void SetColors(std::vector<Color> colors);
    void SetUVs(std::vector<Vector2> uvs);
    void SetNormals(std::vector<Vector3> normals);
    void SetIndices(std::vector<unsigned> indices);

    void Draw();

    // init primitives
    static void Init();
    // delete primitives
    static void Exit();

    static void DrawQuad(
      Vector2 offset = Vector2(0, 0),
      Vector2 tiling = Vector2(1, 1),
      Vector2 pivot = Vector2(.5f, .5f),
      bool wireframe = false);

    static void DrawCube(
      Vector3 offset = Vector3(0, 0, 0),
      Vector3 tiling = Vector3(1, 1, 1),
      Vector3 pivot = Vector3(.5f, .5f, .5f),
      bool wireframe = false);
  };
}
