/*Esta Clase implementa la interfaz cResource.
Esta clase es HIJA de cResource (PADRE) y por tanto heredará de ella e 
implementa las funciones virtuales correspondientes. 
*/

#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include "../../Utility/Resource.h"
#include "../../Utility/ResourceHandle.h"
#include "../Object/Object.h"


//Para poder añadir el prototipo de la función ProcessScene a la definición de esta clase es necesario 
// añadir una declaración forward de aiScene para poder referenciarla en dicha función: 
struct aiScene;
struct aiNode;

class cScene : public cResource
{
   public:
      cScene()                               { mbLoaded = false; }
 
	  //Inicializa una escena desde un fichero indicando su ruta.
      virtual bool Init( const std::string &lacNameID, const std::string &lacFile );

	  //Libera la escena recorriendo todas las mallas y llamando a los Deinit.
	  virtual void Deinit();

      //Comprueba si la escena está cargada.
      virtual bool IsLoaded()  { return mbLoaded; }

      void Update( float lfTimestep );

	  //Renderiza la escena recorriendo todas las mallas y llamando a los Render.
      void Render();

   private:  
	    
      //Extrae la información de la escena (en nuestro caso, se encargará de 
      // extraer la información de las mallas).
	  void ProcessScene( const aiScene* lpScene );

	  // This method converts three structure of the scene to a more plannar structure for optimize the render process. Scene will be static. 
	  void cScene::ConvertNodesToObjects( aiNode *lpNode, cMatrix lTransform );

	  //Cadena que almacena el nombre del fichero de escena.
      std::string macFile;

	  //Booleano que indica si la escena está cargada o no.
      bool mbLoaded;

	  typedef std::vector<cResourceHandle> cResourceHandleList;
	  //Iterador para recorrer el vector de los manejadores de malla.
	  typedef cResourceHandleList::iterator cResourceHandleListIt;
	  //Vector (de tipo cResourceHandleList) que contendrá los manejadores de malla de la escena.
	  cResourceHandleList mMeshList;
	  //Vector of materials (keeps info of the materials loaded)
	  cResourceHandleList mMaterialList;
	  //This data structure will keep the relationship between meshes indexes and materials
	  std::vector<unsigned> mMeshMaterialIndexList;
	  //Object keeps info of meshes, materials and world matrix of the scene
	  typedef std::vector<cObject *> cObjectList;
	  cObjectList mObjectList;
};

#endif