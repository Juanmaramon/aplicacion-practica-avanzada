/*************************************************************************
TargaImage.h

Standard Targa loader.

Author   :   Dave Astle

Written for More OpenGL Game Programming
*************************************************************************/
#ifndef TARGAIMAGE_H_INCLUDED
#define TARGAIMAGE_H_INCLUDED


  class TargaImage
  {
  public:
    TargaImage();
    virtual ~TargaImage();

    // loading and unloading
    bool Load(const char* filename);
    void Release();

    // flips image vertically
    bool FlipVertical();

    unsigned short GetWidth() { return m_width; }
    unsigned short GetHeight() { return m_height; }
    unsigned char  GetImageFormat() { return m_imageDataFormat; }

    // converts RGB format to RGBA format and vice versa
    bool ConvertRGBAToRGB();
    bool ConvertRGBToRGBA(unsigned char alphaValue);

    // returns the current image data
    unsigned char* GetImage() { return m_pImageData; }

  private:
    // swap the red and blue components in the image data
    void SwapRedBlue();

    unsigned char   m_colorDepth;
    unsigned char   m_imageDataType;
    unsigned char   m_imageDataFormat;
    unsigned char*  m_pImageData;
    unsigned short  m_width;
    unsigned short  m_height;	
    unsigned long   m_imageSize;
  };

#endif // TARGAIMAGE_H_INCLUDED