
#include "GraphicManager.h"
#include <assert.h>

//Función que se encarga de la inicialización de OpenGL.
//Recibe la ventana sobre la que OpenGL hará el render.
bool cGraphicManager::Init( cWindow * lpWindow )
{
   //Se crear el contexto de renderizado:
   bool lbRet = CreateContext( lpWindow );
   if ( lbRet )
   {
      //Se establece el estado de OpenGL:
      InitializeGLState();	
   }
   return lbRet;
}

//Función que crea el contexto de renderizado.
//Cada vez que se produce un error, se llama a la función Deinit 
// para liberar aquellos recursos que se hayan solicitado previamente.
bool cGraphicManager::CreateContext( cWindow * lpWindow )
{   
   assert(lpWindow);
   //Se rellena en una estructura las características que tendrán los buffers de 
   // vídeo:
   static   PIXELFORMATDESCRIPTOR lPfp=                    
   {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,
      PFD_DRAW_TO_WINDOW |
      PFD_SUPPORT_OPENGL |
      PFD_DOUBLEBUFFER, 
      PFD_TYPE_RGBA,
      lpWindow->GetBpp(),
	  0, 0, 0, 0, 0, 0,
      0,
      0, 0, 0, 0, 0, 0,
      32, 0, 0,
      PFD_MAIN_PLANE,
      0, 0, 0, 0
   };
   HDC &lDC = lpWindow->GetHDC();
   if (!lDC)                                
   {
      Deinit();                                
      MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
      return false;                                  
   }
   //Se intenta encontrar un formato de píxel que sea compatible con la configuración
   // anterior:
   mPixelFormat = ChoosePixelFormat(lDC,&lPfp);
   if (!mPixelFormat)   
   {
      Deinit(); 
      MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
      return false;
   }
   //Se establece el formato de píxel:
   if(!SetPixelFormat(lDC,mPixelFormat,&lPfp))       
   {
      Deinit();
      MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
      return false;
   }
   //Se crea un contexto de renderizado de OpenGL, por lo que se llama a la función wglCreateContext:
   mHRC = wglCreateContext(lDC);
   if (!mHRC)
   {
      Deinit();
      MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
      return false;
   }
   //Se establece el contexto de renderizado creado como el contexto de renderizado actual:
   if(!wglMakeCurrent(lDC,mHRC))
   {
      Deinit();
      MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
      return false;
   }

   mpWindow = lpWindow;
   return true;
}

//Función que inicializa el estado de OpenGL. 
void cGraphicManager::InitializeGLState()
{
   //Se inicializa el ShadeModel:
   glShadeModel(GL_SMOOTH);   
   //Se establecen los valores con los que se limpiará el backbuffer y el ZBuffer:
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClearDepth(1.0f);
   //Configurar el Z test:
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL); 
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   unsigned muiWidth = mpWindow->GetWidth();
   unsigned muiHeight = mpWindow->GetHeight();
   //Se configura el Viewport:
   glViewport(0, 0, muiWidth, muiHeight);
   //Se activa el uso de Texturas:
   glEnable(GL_TEXTURE_2D);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   //Se establece la matriz de Projection:
   glMatrixMode(GL_PROJECTION);                             
   glLoadIdentity();
   float lfAspectRatio = (float)muiWidth/(float)muiHeight;
   gluPerspective(45.0f, lfAspectRatio, 0.1f, 100.0f);
   //Se establece la matriz de ModelView:
   glMatrixMode(GL_MODELVIEW);                        
   glLoadIdentity();          
}

//Función que se encarga de intercambiar el backbuffer con el frontbuffer.
void cGraphicManager::SwapBuffer()
{
	if ( mpWindow )
	{
		//Se invoca a una función de OpenGL.
		SwapBuffers(mpWindow->GetHDC());
	}
}

//Función para renderizar un punto en el espacio.
//El primer parámetro es la posición donde 
// se renderizará el punto y el segundo parámetro será el color representado por RGB 
// entre 0.0 y 1.0.
void cGraphicManager::DrawPoint( const cVec3 &lvPosition, const cVec3 &lvColor )
{
   //Antes de pintar las primitivas se desactivan el uso de texturas y después se vuelve a activar, 
   //esto se hace para asegurarnos de que el uso de texturas no impide el renderizado de estas primitivas. 	
   glDisable(GL_TEXTURE_2D);
 
   // POINT
   glPointSize(10);
   glColor3f (lvColor.x, lvColor.y, lvColor.z);
   glBegin(GL_POINTS);
   glVertex3f(lvPosition.x, lvPosition.y, lvPosition.z);
   glEnd ();
   glEnable(GL_TEXTURE_2D);
}

//Función para rederizar una línea.
void cGraphicManager::DrawLine( const cVec3 &lvPosition1, 
  const cVec3 &lvPosition2, 
  const cVec3 &lvColor )
{
   //Antes de pintar las primitivas se desactivan el uso de texturas y después se vuelve a activar, 
   //esto se hace para asegurarnos de que el uso de texturas no impide el renderizado de estas primitivas. 	
   glDisable(GL_TEXTURE_2D);
 
   // LINE
   glLineWidth (1);
   glColor3f (lvColor.x, lvColor.y, lvColor.z);
   glBegin(GL_LINES);
   for (float xtmp = -10.0; xtmp <= 10.0; xtmp += 1.0)
   {
      glVertex3f(lvPosition1.x, lvPosition1.y, lvPosition1.z);
      glVertex3f(lvPosition2.x, lvPosition2.y, lvPosition2.z);
   };
   glEnd ();
 
   glEnable(GL_TEXTURE_2D);
}

//Función para renderizar una malla o una rejilla.
void cGraphicManager::DrawGrid()
{
   //Antes de pintar las primitivas se desactivan el uso de texturas y después se vuelve a activar, 
   //esto se hace para asegurarnos de que el uso de texturas no impide el renderizado de estas primitivas. 	
   glDisable(GL_TEXTURE_2D);
 
   // GRID
   glLineWidth (1);
   glColor3f (1.0f, 1.0f, 1.0f);
   glBegin(GL_LINES);
   for (float lfxtmp = -10.0; lfxtmp <= 10.0; lfxtmp += 1.0)
   {
      glVertex3f (lfxtmp, 0.0f,-10.0);
      glVertex3f (lfxtmp, 0.0f, 10.0);
      glVertex3f (-10.0, 0.0f, lfxtmp);
      glVertex3f ( 10.0, 0.0f, lfxtmp);
   };
   glEnd ();
   glEnable(GL_TEXTURE_2D);
}

//Función para renderizar unos ejes de coordenadas y así
// saber en qué dirección está cada uno (X, Y, Z).
//Esta función también se puede usar para pintar los ejes locales de un modelo.
void cGraphicManager::DrawAxis()
{
   //Antes de pintar las primitivas se desactivan el uso de texturas y después se vuelve a activar, 
   //esto se hace para asegurarnos de que el uso de texturas no impide el renderizado de estas primi
   glDisable(GL_TEXTURE_2D);
 
   // AXIS
   glLineWidth (2);
   glBegin(GL_LINES);
 
   //Se renderizan los ejes para que el orden coincida con RGB y así sea
   // fácil de recordar el color de cada eje. 
   // Por lo tanto, X será rojo, Y será verde y Z será azul:
   glColor3f(1.0f, 0.0f, 0.0f);     // X Axis
   glVertex3f(0.0f, 0.0f, 0.0f);    
   glColor3f(1.0f, 0.0f, 0.0f);
   glVertex3f(1.0f, 0.0f, 0.0f);    
 
   glColor3f(0.0f, 1.0f, 0.0f);     // Y Axis
   glVertex3f(0.0f, 0.0f, 0.0f);    
   glColor3f(0.0f, 1.0f, 0.0f);
   glVertex3f(0.0f, 1.0f, 0.0f);    
 
   glColor3f(0.0f, 0.0f, 1.0f);     // Z Axis
   glVertex3f(0.0f, 0.0f, 0.0f);    
   glColor3f(0.0f, 0.0f, 1.0f);
   glVertex3f(0.0f, 0.0f, 1.0f);    
      
   glEnd();
 
   glEnable(GL_TEXTURE_2D);
}

//Función que se encarga de la liberación de OpenGL. 
bool cGraphicManager::Deinit()
{
   //Se comprueba si se ha llegado a crear el contexto de renderizado y 
   // de ser así, se establece el contexto de renderizado por defecto y se elimina el que se creó
   // en la función CreateContext.
   if (mHRC)
   {
      if (!wglMakeCurrent(NULL,NULL))
      {
         MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
      }
      if (!wglDeleteContext(mHRC)) 
      {
         MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
      }
      mHRC = NULL;
   }
   //Se limpia la ventana poniéndola a NULL.
   mpWindow = NULL;
   return true;
}


/*
   Nota: Cada objeto tiene una matriz de mundo (World) asociada,
   que contiene la posición y la orientación del modelo en el mundo. 
   Por lo tanto, si multiplicamos cada uno de los vértices del 
   modelo por la matriz de mundo, colocaremos el modelo donde debe estar en la escena.
   Y por último se debe multiplicar por las matrices de vista y proyección para generar la imagen 
   final.
   Por lo tanto, para componer el renderizado se debe hacer las siguientes operaciones:
   Model * WorldMatrix * ViewMatrix * ProyectionMatrix. 
   La matriz de mundo depende de cada modelo (ya que cada modelo tiene una posición 
   y orientación distinta en el mundo), mientras que la ViewMatrix y la ProyectionMatrix 
   dependen de la cámara. 
   El problema es que OpenGL requiere estas matrices de una forma diferente: 
	•    GL_MODELVIEW: Es la matrix de mundo multiplicada por la matriz de vista 
			(WorldMatrix * ViewMatrix)
	•    GL_PROJECTION: Es la matrix de proyección (ProyectionMatrix)
   Por lo que el graphic manager debe calcular la matriz ModelView cada vez que se 
   cambie la matriz de mundo o la cámara.
   */

//Función que establece la matriz de mundo.
void cGraphicManager::SetWorldMatrix( const cMatrix &lMatrix )
{
    //Se copia la matriz de mundo y se llama a la función 
	// que actualiza la matriz ModelView en OpenGL.
   mWorldMatrix = lMatrix;
   RefreshWorldView();
}

//Función que establece la cámara actual.
void cGraphicManager::ActivateCamera( cCamera * lpCamera )
{
   assert(lpCamera);
   mpActiveCamera = lpCamera;
   // Select The Projection Matrix
   glMatrixMode(GL_PROJECTION);
   // Set The Projection Matrix
   glLoadMatrixf( mpActiveCamera->GetProj().AsFloatPointer() );     
   // Refresh the worl view matrix: 
   // Al igual que en la funcón  anterior, tenemos que llamar a RefreshWorldView 
   // porque al cambiar la cámara, ha cambiado la matriz de vista, y por tanto la matriz WorldView.
   RefreshWorldView();
}

//Función que actualiza la matriz ModelView en OpenGL.
// Se invoca desde SetWorldMatrix y desde ActivateCamera.
void cGraphicManager::RefreshWorldView()
{
   // Select The Modelview Matrix
   glMatrixMode(GL_MODELVIEW);
   // Calculate the ModelView Matrix
   cMatrix lWorldView = mpActiveCamera->GetView();
   lWorldView = mWorldMatrix * lWorldView;
   // Set The View Matrix
   glLoadMatrixf( lWorldView.AsFloatPointer() ); 
   // Loads World view proyection matrix
   mWVPMatrix = lWorldView * mpActiveCamera->GetProj();
}
