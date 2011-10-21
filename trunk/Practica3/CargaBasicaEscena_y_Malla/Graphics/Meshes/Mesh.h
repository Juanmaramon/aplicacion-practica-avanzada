/*Esta Clase (malla) implementa la interfaz cResource.
Esta clase es HIJA de cResource (PADRE) y por tanto heredará de ella e 
implementa las funciones virtuales correspondientes. 

Se encarga de cargar la malla desde la estructura (aiMesh) que nos proporciona 
la librería Assimp para posteriormente almacenar la información de mallas en la 
tarjeta gráfica (mirar método cMesh::Init).
*/


#ifndef MESH_H
#define MESH_H
 
#include <string>
#include <vector>
#include "../../Utility/Resource.h"
#include "../../Utility/ResourceHandle.h"

// This constants allow us to differents  between an static mash and a skeletal one
static int kuiStaticMesh = 0;
static int kuiSkeletalMesh = 1;

//En la función "cMesh::Init" se hace referencia a la estructura aiMesh, por eso es necesario
// añadir una declaración forward de dicha clase:
struct aiMesh;
 
class cMesh : public cResource
{
   public:
      cMesh() { mbLoaded = false; }
 
      //Inicializa una malla desde Memoria.
	  virtual bool Init( const std::string &lacNameID, void * lpMemoryData, int luiTypeID );

	  //Reloads the mesh
	  void Reload( void * lpMemoryData );
      
	  //Libera los bufferes que se crean en el método Init.
	  virtual void Deinit();

      //Comprueba si la malla está cargada.
	  virtual bool IsLoaded()  { return mbLoaded; }
      
      //Renderiza la malla.	  
	  virtual void RenderMesh();

	  //This function will be update the mesh data
	  virtual void Update(float lfTimestep) {}
   
	  // Virtual function, in cSkeletalMesh will set shader animation matrix
	  virtual void PrepareRender(cResourceHandle lMaterial) { ; }

	protected:
	
	  // Mesh name
	  std::string macFile;

      //Almacena el número de índices que contiene la malla y que se necesita
	  // para poder renderizar.
	  unsigned muiIndexCount;

	  //Array with multiple texture coordinates 
	  std::vector<unsigned> maVboTexture;

	  //Booleano que indica si la malla está cargada o no.
      bool mbLoaded;

	  //Buffer de vértices (posiciones).
	  unsigned int mVboVertices;

	  //Buffer de normales.
	  unsigned int mVboNormals;

	  //Buffer de índices.
	  unsigned int mVboIndex;

	  //Booleano que indique si la malla cuenta o no con normales.
	  bool mbHasNormals;

};
#endif