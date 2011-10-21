/*Esta Clase implementa la interfaz cResourceManager. Es un gestor de texturas.
Esta clase es HIJA de cResourceManager (PADRE) y por tanto heredará de ella.
De los 2 métodos virtuales, implementa el "LoadResourceInternal" que carga el
 recurso desde un fichero.
*/

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
 
#include "../../Utility/ResourceManager.h"
#include "../../Utility/Singleton.h"
 
//Esta clase es un Singleton. 
class cTextureManager : public cResourceManager, public cSingleton<cTextureManager>
{
   public: 
      friend class cSingleton<cTextureManager>;

   protected:
      cTextureManager() { ; } // Protected constructor
 
   private:
      //Carga la textura específica desde un fichero.
      virtual  cResource * LoadResourceInternal( std::string lacNameID, const std::string &lacFile );
};
 
#endif