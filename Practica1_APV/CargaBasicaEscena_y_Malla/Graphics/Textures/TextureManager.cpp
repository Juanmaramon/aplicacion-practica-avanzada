#include "TextureManager.h"
#include "Texture.h"

//Método que carga la textura específica desde un fichero.
cResource * cTextureManager::LoadResourceInternal( std::string lacNameID, const std::string &lacFile )
{
   //Se crea una instancia de cTexture y llama a su función Init con los parámetros asociados. 
   //Si la función devuelve error, libera la memoria y devuelve NULL. De lo contrario, construye la 
   // textura y devuelve el recurso (textura en este caso).
   cTexture * lpTexture = new cTexture();
   if (!lpTexture->Init( lacNameID, lacFile ))
   {
      delete lpTexture;
      return NULL;
   }
 
   return lpTexture;
}