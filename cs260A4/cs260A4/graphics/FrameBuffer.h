#pragma once


  class FrameBuffer
  {
    const unsigned s_MaxFramebufferSize = 8192;
    unsigned rendererID = 0;
    unsigned colorAttachment = 0, depthAttachment = 0;
    unsigned width = 1280, height = 720;


  public:
    FrameBuffer();
    ~FrameBuffer();
    unsigned GetColorAttachment() const;
    unsigned GetDepthAttachment() const;
    void Bind() const;
    static void Unbind();
    void BindColorAsTexture(unsigned slot = 0) const;
    static void UnbindTexture();
    void Invalidate();
    void Resize(unsigned width, unsigned height);
  };
