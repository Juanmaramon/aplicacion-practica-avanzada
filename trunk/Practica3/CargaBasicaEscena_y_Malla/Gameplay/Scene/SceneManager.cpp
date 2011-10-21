#include "SceneManager.h"
#include "Scene.h"
 
//Método que carga la escena específica desde un fichero.
cResource * cSceneManager::LoadResourceInternal(  std::string lacNameID, const std::string &lacFile )
{
   //La escena vendrá definida por un fichero, crearemos la 
   // escena con ese fichero y si todo ha ido bien, devolveremos la escena recién cargada. De 
   // lo contrario, liberamos la memoria y devolvemos NULL.
   cScene * lpScene = new cScene();
   if (!lpScene->Init( lacNameID, lacFile ))
   {
      delete lpScene;
      return NULL;
   }
   return lpScene;
}