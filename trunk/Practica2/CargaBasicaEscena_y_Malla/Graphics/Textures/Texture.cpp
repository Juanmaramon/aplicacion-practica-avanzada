#include <cassert>
#include "Texture.h"
#include "../GLHeaders.h"
#include "SOIL/SOIL.h"

//Método que inicializa una textura desde un fichero indicando su ruta.
bool cTexture::Init( const std::string &lacNameID, const std::string &lacFile )
{
   macFile = lacFile;
   //La librería (SOIL) se encarga de cargar en OpenGL la textura automáticamente:
   // Se indica a la librería que se debe encargar de cargar la textura en OpenGL (SOIL_LOAD_AUTO),
   // que debe asignar un nuevo identificador de textura (SOIL_CREATE_NEW_ID),
   // que debe generar los mipmaps de la textura (SOIL_FLAG_MIPMAPS), 
   // convertir la textura a potencia de dos (SOIL_FLAG_POWER_OF_TWO)
   // y comprimir la textura a formato DDS (SOIL_FLAG_COMPRESS_TO_DXT).
   muiTextureHandle = SOIL_load_OGL_texture(
                                       macFile.c_str(),
                                       SOIL_LOAD_AUTO,
                                       SOIL_CREATE_NEW_ID,
                                       SOIL_FLAG_MIPMAPS |  
                                       SOIL_FLAG_POWER_OF_TWO |  
                                       SOIL_FLAG_COMPRESS_TO_DXT);
   assert( muiTextureHandle != 0 );
   return muiTextureHandle != 0;
}

//Método que libera la textura.
void cTexture::Deinit()
{
   //Se libera la textura usando las llamadas de OpenGL.
   glDeleteTextures( 1, &muiTextureHandle);
   muiTextureHandle = 0;
}
   
