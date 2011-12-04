#include "cLuaManager.h"
#include <cassert>
#include <windows.h>

cLuaManager::cLuaManager()
{
	mpLuaContext = NULL;
}

void cLuaManager::Init()
{
	//Creamos el contexto de Lua
	mpLuaContext = lua_open();
	//Comprobamos que el contexto se creo correctamente (no es NULL)
	assert( mpLuaContext );
	//Cargamos las librerias de Lua
	luaL_openlibs( mpLuaContext );
	//Para mostrar los errores de ejecución
	lua_atpanic(mpLuaContext, FuncPanic);
}

void cLuaManager::Deinit()
{
	if ( mpLuaContext )
	{
		//Cerramos el contexto de Lua
		lua_close( mpLuaContext );
	}
	mpLuaContext = NULL;
}

bool cLuaManager::DoString( const char *lacStatement )
{
	assert( lacStatement );
	assert( mpLuaContext );
	int liRet = luaL_dostring( mpLuaContext, lacStatement );
	//Chequear si ha ocurrido algun error
	return CheckError( liRet );
}

bool cLuaManager::CheckError( int liError )
{
	//Si liError es distinto de cero ha habido un error
	if ( liError != 0 )
	{
		assert( mpLuaContext );
		//Mostramos el mensaje de error
		char lacBuffer[1024];
		sprintf(lacBuffer,"Error:%s\n",lua_tostring( mpLuaContext, -1 ));
		OutputDebugStr(lacBuffer);
		//Sacamos el mensaje del top de la pila
		lua_pop( mpLuaContext, 1 );
		return false;
	}
	return true;
}

int cLuaManager::FuncPanic( lua_State *lpContext )
{
	//Si liError es distinto de cero ha habido un error
	assert( lpContext );
	//Mostramos el mensaje de error
	char lacBuffer[1024];
	sprintf(lacBuffer,"Error:%s\n",lua_tostring( lpContext, -1 ));
	OutputDebugStr(lacBuffer);
	//Sacamos el mensaje del top de la pila
	lua_pop( lpContext, 1 );
	return 1;
}

//Carga y ejecuta un fichero .lua
bool cLuaManager::DoFile( const char* lacFile )
{
	assert( lacFile );
	assert( mpLuaContext );
	//Cargamos el fichero de script
	int liRet = luaL_dofile( mpLuaContext, lacFile );
	//Chequeamos si ha habido algun error
	return CheckError( liRet );
}

void cLuaManager::Register( const char* lacFuncName, lua_CFunction lpFunc )
{
	//Comprobamos que los argumentos no son NULL
	assert( mpLuaContext );
	assert( lpFunc );
	//Publicamos la funcion en Lua
	lua_register( mpLuaContext, lacFuncName, lpFunc );
}

/* Funciones para meter en la pila los argumentos para pasar a funciones Lua */
void cLuaManager::PushArg( int liArg )
{
	assert( mpLuaContext );
	lua_pushinteger( mpLuaContext, liArg );
}

void cLuaManager::PushArg( long liArg )
{
	assert( mpLuaContext );
	lua_pushnumber( mpLuaContext, liArg );
}

void cLuaManager::PushArg( float lfArg )
{
	assert( mpLuaContext );
	lua_pushnumber( mpLuaContext, lfArg );
}

void cLuaManager::PushArg( const char* lacArg )
{
	assert( mpLuaContext );
	assert( lacArg );
	lua_pushstring( mpLuaContext, lacArg );
}

void cLuaManager::PushArg( std::string &lacArg )
{
	assert( mpLuaContext );
	lua_pushstring( mpLuaContext, lacArg.c_str() );
}

void cLuaManager::PushArg( bool lbArg )
{
	assert( mpLuaContext );
	lua_pushboolean( mpLuaContext, lbArg );
}

/* Funciones para recuperar el valor de retorno de funciones Lua */
void cLuaManager::GetRetValue( int* liRet )
{
	assert( mpLuaContext );
	*liRet = (int)lua_tointeger( mpLuaContext, -1 );
}

void cLuaManager::GetRetValue( float* lfRet )
{
	assert( mpLuaContext );
	*lfRet = (float)lua_tonumber( mpLuaContext, -1 );
}

void cLuaManager::GetRetValue( long* liRet )
{
	assert( mpLuaContext );
	*liRet = (long)lua_tonumber( mpLuaContext, -1 );
}

void cLuaManager::GetRetValue( std::string* lacRet )
{
	assert( mpLuaContext );
	*lacRet = (const char*)lua_tostring( mpLuaContext, -1 );
}

void cLuaManager::GetRetValue( const char* lacRet )
{
	assert(mpLuaContext);
	lacRet = lua_tostring( mpLuaContext, -1 );
}

void cLuaManager::GetRetValue( bool* lbRet )
{
	assert( mpLuaContext );
	*lbRet = ( lua_toboolean( mpLuaContext, -1 ) != 0 );
}

/* Elimina de la pila N valores */
void cLuaManager::Pop( unsigned int luiNValues )
{
	assert( mpLuaContext );
	lua_pop( mpLuaContext, (int)luiNValues );
}