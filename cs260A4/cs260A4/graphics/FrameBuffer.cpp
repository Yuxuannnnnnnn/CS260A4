//#include "FrameBuffer.h"
//
//#include "OpenGLErrorCheck.h"
//
//
//  FrameBuffer::FrameBuffer()
//  {
//    glGenFramebuffers(1, &rendererID);
//
//    Invalidate();
//  }
//
//  FrameBuffer::~FrameBuffer()
//  {
//    GLCall(glDeleteFramebuffers(1, &rendererID));
//    GLCall(glDeleteTextures(1, &colorAttachment));
//    GLCall(glDeleteTextures(1, &depthAttachment));
//  }
//
//  unsigned FrameBuffer::GetColorAttachment() const
//  {
//    return colorAttachment;
//  }
//
//  unsigned FrameBuffer::GetDepthAttachment() const
//  {
//    return depthAttachment;
//  }
//
//  void FrameBuffer::Bind() const
//  {
//    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, rendererID));
//    GLCall(glViewport(0, 0, width, height));
//  }
//
//  void FrameBuffer::Unbind()
//  {
//    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
//  }
//
//  void FrameBuffer::BindColorAsTexture(unsigned slot) const
//  {
//    glActiveTexture(GL_TEXTURE0 + slot);
//    glBindTexture(GL_TEXTURE_2D, colorAttachment);
//  }
//  void FrameBuffer::UnbindTexture()
//  {
//    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
//  }
//
//  
//  void FrameBuffer::Invalidate()
//  {
//    if (rendererID)
//    {
//      GLCall(glDeleteFramebuffers(1, &rendererID));
//      GLCall(glDeleteTextures(1, &colorAttachment));
//      GLCall(glDeleteTextures(1, &depthAttachment));
//    }
//
//    GLCall(glCreateFramebuffers(1, &rendererID));
//    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, rendererID));
//
//    GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachment));
//    GLCall(glBindTexture(GL_TEXTURE_2D, colorAttachment));
//    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
//    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
//    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
//
//    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0));
//
//    GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment));
//    GLCall(glBindTexture(GL_TEXTURE_2D, depthAttachment));
//    GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width, height));
//    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0));
//
//    ASSERT_ERROR(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
//
//    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
//  }
//  
//  void FrameBuffer::Resize(unsigned w, unsigned h)
//  {
//    if (w == 0 || h == 0 || w > s_MaxFramebufferSize || h > s_MaxFramebufferSize)
//    {
//      std::cerr << "Attempted to rezize framebuffer to " <<  w << ", " << h << std::endl;
//      return;
//    }
//    width = w;
//    height = h;
//    Invalidate();
//  }
