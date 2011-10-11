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
#include "../../Utility/Resource.h"
 
//En la función "cMesh::Init" se hace referencia a la estructura aiMesh, por eso es necesario
// añadir una declaración forward de dicha clase:
struct aiMesh;
 
class cMesh : public cResource
{
   public:
      cMesh()                         { mbLoaded = false; }
 
      //Inicializa una malla desde Memoria.
	  virtual bool Init( const std::string &lacNameID, void * lpMemoryData, int luiTypeID );
      
	  //Libera los bufferes que se crean en el método Init.
	  virtual void Deinit();

      //Comprueba si la malla está cargada.
	  virtual bool IsLoaded()  { return mbLoaded; }
      
      //Renderiza la malla.	  
	  void RenderMesh();
   
	private:
		
      //Almacena el número de índices que contiene la malla y que se necesita
	  // para poder renderizar.
	  unsigned muiIndexCount;

	  //Booleano que indica si la malla está cargada o no.
      bool mbLoaded;

	  //Buffer de vértices (posiciones).
	  unsigned int mVboVertices;

	  //Buffer de normales.
	  unsigned int mVboNormals;

	  //Buffer de texturas.
	  unsigned int mVboTexture;

	  //Buffer de índices.
	  unsigned int mVboIndex;

	  //Booleano que indique si la malla cuenta o no con normales.
	  bool mbHasNormals;

};
#endif