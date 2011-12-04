//Clase que encapsula todos lo métodos necesarios para trabajar con la librería LUA

#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <string>//Para usar std::string
#include "..\Utility\Singleton.h"

// extern C le dice al compilador que la libreria debe ser compilada en C y no en C++ 
extern "C"
{
   #include "../Lua/src/lua.h"
   #include "../Lua/src/lauxlib.h"
   #include "../Lua/src/lualib.h"
}

//Esta clase usará el patrón Singleton definido en "..Utility/Singleton.h"
class cLuaManager : public cSingleton<cLuaManager>
{
	public:
		void Init();
		bool DoString( const char *lacStatement );	
		bool DoFile( const char* lacFile );
		void Register( const char* lacFuncName, lua_CFunction lpFunc );
		template <class Z> bool CallLua( const char* lacFuncName, Z* lpRet );
		template <class T, class Z> bool CallLua( const char* lacFuncName, T lArg, Z* lpRet);
		template <class T, class U, class Z> bool CallLua( const char* lacFuncName, T lArg1, U lArg2, Z* lpRet );
		template <class T, class U, class V, class Z> bool CallLua( const char* lacFuncName, T lArg1, U lArg2, V lArg3, Z* lpRet);
		void Deinit();
		inline lua_State* GetContext() { return mpLuaContext; }
		friend class cSingleton<cLuaManager>;
	
	protected:
		cLuaManager(); // Protected constructor
	
	private:
		/*Una variable de tipo "lua_state" mantiene una traza de qué funciones han sido llamadas, 
		qué variables han sido definidas, sus valores y otro tipo de información que se necesita 
		durante la ejecución. A este estado también se le suele llamar contexto de ejecución.*/
		lua_State* mpLuaContext;

		bool CheckError( int liError );
		static int FuncPanic(lua_State *lpContext);

		void PushArg( int liArg );
		void PushArg( long liArg );
		void PushArg( float lfArg );
		void PushArg( const char* lacArg );
		void PushArg( std::string &lacArg );
		void PushArg( bool lbArg );

		void GetRetValue( int* liRet );
		void GetRetValue( long* liRet );
		void GetRetValue( float* lfRet );
		void GetRetValue( const char* lacRet );
		void GetRetValue( std::string* lacRet );
		void GetRetValue( bool* lbRet );

		void Pop( unsigned int luiNValues );
};

/*Los métodos de tipo template se definen aquí en el .h por fuera de la clase*/

//Método  que se encargará de realizar todos los pasos necesarios para llamar a
// una función definida en Lua que no tenga argumentos.
//Parámetros: "lacFuncName", nombre de la función definida en Lua; 
// "lpRet", puntero donde se almacena el valor de retorno si lo hay. 
// Si "lpRet" es NULL significará que la función que queremos llamar no devuelve ningún valor. En 
// caso contrario, la función devuelve una valor que se almacenará en lpRet.
// "CallLua" se define como un template porque los argumentos pueden ser de cualquier tipo.
template <class Z> bool cLuaManager::CallLua( const char* lacFuncName, Z* lpRet )
{
	assert( mpLuaContext );

	//1.Introducir en la pila el nombre de la funcion definida en Lua que
	//  queremos llamar.
	lua_getglobal( mpLuaContext, lacFuncName );

    //Antes de poder usar una función de Lua es necesario que se cargue el fichero
	// (script .lua) que contiene su definición. Esto se hace normalmente en el "Main.cpp->main()".

	//Chequeamos que la función esta definida en Lua
	int lbExists = lua_isfunction( mpLuaContext, -1 );
	//Si no está definida en Lua
	if ( !lbExists )
	{
		//Sacamos el nombre de la función de la pila de Lua
		Pop( 1 );
		printf( "Error: la funcion %s no existe en Lua\n", lacFuncName );
		return false;
	}

	//Si la funcion está definida en Lua...
	
	//2.Obtener el valor de retorno (si lo hay)
	//Si la funcion devuelve un valor, el puntero "lpRet" no será nulo.
	if ( lpRet )
	{
		//Llamamos a la función Lua que se insertó en el punto 1. para el contexto dado,
		//  indicando que tiene 0 parámetros y 1 valor de retorno.
		lua_call( mpLuaContext, 0, 1 );

		//Obtenemos el resultado (valor de retorno) del tope de la pila (la función "GetRetValue" está sobrecargada: dependiendo del 
		//  tipo del argumento, se llamará automáticamente a la función correspondiente).
		GetRetValue( lpRet );
		
		//3.Eliminar el valor de retorno de la pila.
		Pop( 1 );
	}
	//La función no devuelve ningún valor
	else
	{
		//Llamamos a la función Lua que se insertó en el punto 1. para el contexto dado,
		//  indicando que tiene 0 parámetros y 0 valores de retorno.
		lua_call( mpLuaContext, 0, 0 );
	}
	return true;
}

//Método  que se encargará de realizar todos los pasos necesarios para llamar a
// una función definida en Lua que tenga 1 argumento.
//Parámetros: "lacFuncName", nombre de la función definida en Lua; "lArg", único argumento de la función,
// "lpRet", puntero donde se almacena el valor de retorno si lo hay. 
// Si "lpRet" es NULL significará que la función que queremos llamar no devuelve ningún valor. En 
// caso contrario, la función devuelve una valor que se almacenará en lpRet.
// "CallLua" se define como un template porque los argumentos pueden ser de cualquier tipo.
template <class T, class Z> bool cLuaManager::CallLua( const char* lacFuncName, T lArg, Z* lpRet)
{
	assert( mpLuaContext );

	//1.Introducir en la pila el nombre de la funcion definida en Lua que
	//  queremos llamar.
	lua_getglobal( mpLuaContext, lacFuncName );

    //Antes de poder usar una función de Lua es necesario que se cargue el fichero
	// (script .lua) que contiene su definición. Esto se hace normalmente en el "Main.cpp->main()".

	//Chequeamos que la función esta definida en Lua
	int lbExists = lua_isfunction( mpLuaContext, -1 );
	//Si no está definida en Lua
	if ( !lbExists )
	{
		//Sacamos el nombre de la función de la pila de Lua
		Pop( 1 );
		printf( "Error: la funcion %s no existe en Lua\n", lacFuncName );
		return false;
	}

	//Si la funcion está definida en Lua...

	//2.Introducir el argumento en la pila (la función "PushArg" está sobrecargada: dependiendo del 
	//  tipo del argumento, se llamará automáticamente a la función correspondiente).	
	PushArg(lArg);
	
	//3.Obtener el valor de retorno (si lo hay)
	//Si la funcion devuelve un valor, el puntero "lpRet" no será nulo.
	if ( lpRet )
	{
		//Llamamos a la función Lua que se insertó en el punto 1. para el contexto dado,
		//  indicando que tiene 1 parámetro y 1 valor de retorno.
		lua_call( mpLuaContext, 1, 1 );

		//Obtenemos el resultado (valor de retorno) del tope de la pila (la función "GetRetValue" está sobrecargada: dependiendo del 
		//  tipo del argumento, se llamará automáticamente a la función correspondiente).
		GetRetValue( lpRet );
		
		//4.Eliminar el valor de retorno de la pila.
		Pop( 1 );
	}
	//La función no devuelve ningún valor
	else
	{
		//Llamamos a la función Lua que se insertó en el punto 1. para el contexto dado,
		//  indicando que tiene 1 parámetro y 0 valores de retorno.
		lua_call( mpLuaContext, 1, 0 );
	}
	return true;
}

//Método  que se encargará de realizar todos los pasos necesarios para llamar a
// una función definida en Lua que tenga 3 argumentos.
//Parámetros: "lacFuncName", nombre de la función definida en Lua; "lArg1", primer argumento de la función;
// "lArg2", segundo argumento de la función; "lpRet", puntero donde se almacena el valor de retorno si lo hay. 
// Si "lpRet" es NULL significará que la función que queremos llamar no devuelve ningún valor. En 
// caso contrario, la función devuelve una valor que se almacenará en lpRet.
// "CallLua" se define como un template porque los argumentos pueden ser de cualquier tipo.
template <class T, class U, class Z> bool cLuaManager::CallLua( const char* lacFuncName, T lArg1, U lArg2, Z* lpRet)
{
	assert( mpLuaContext );

	//1.Introducir en la pila el nombre de la funcion definida en Lua que
	//  queremos llamar.
	lua_getglobal( mpLuaContext, lacFuncName );

    //Antes de poder usar una función de Lua es necesario que se cargue el fichero
	// (script .lua) que contiene su definición. Esto se hace normalmente en el "Main.cpp->main()".

	//Chequeamos que la función esta definida en Lua
	int lbExists = lua_isfunction( mpLuaContext, -1 );
	//Si no está definida en Lua
	if ( !lbExists )
	{
		//Sacamos el nombre de la función de la pila de Lua
		Pop( 1 );
		printf( "Error: la funcion %s no existe en Lua\n", lacFuncName );
		return false;
	}

	//Si la funcion está definida en Lua...

	//2.Introducir los argumentos en la pila (la función "PushArg" está sobrecargada: dependiendo del 
	//  tipo del argumento, se llamará automáticamente a la función correspondiente).
	//Introducimos en la pila el primer argumento
	PushArg(lArg1);
	//Introducimos en la pila el segundo argumento
	PushArg(lArg2);

	//3.Obtener el valor de retorno (si lo hay)
	//Si la funcion devuelve un valor, el puntero "lpRet" no será nulo.
	if ( lpRet )
	{
		//Llamamos a la función Lua que se insertó en el punto 1. para el contexto dado,
		//  indicando que tiene 2 parámetros y 1 valor de retorno.
		lua_call( mpLuaContext, 2, 1 );

		//Obtenemos el resultado (valor de retorno) del tope de la pila (la función "GetRetValue" está sobrecargada: dependiendo del 
		//  tipo del argumento, se llamará automáticamente a la función correspondiente).
		GetRetValue( lpRet );
		
		//4.Eliminar el valor de retorno de la pila.
		Pop( 1 );
	}
	//La función no devuelve ningún valor
	else
	{
		//Llamamos a la función Lua que se insertó en el punto 1. para el contexto dado,
		//  indicando que tiene 2 parámetros y 0 valores de retorno.
		lua_call( mpLuaContext, 2, 0 );
	}
	return true;
}

//Método  que se encargará de realizar todos los pasos necesarios para llamar a
// una función definida en Lua que tenga 3 argumentos.
//Parámetros: "lacFuncName", nombre de la función definida en Lua; "lArg1", primer argumento de la función;
// "lArg2", segundo argumento de la función; "lArg3", tercer argumento;
// "lpRet", puntero donde se almacena el valor de retorno si lo hay. 
// Si "lpRet" es NULL significará que la función que queremos llamar no devuelve ningún valor. En 
// caso contrario, la función devuelve una valor que se almacenará en lpRet.
// "CallLua" se define como un template porque los argumentos pueden ser de cualquier tipo.
template <class T, class U, class V, class Z> bool cLuaManager::CallLua( const char* lacFuncName, T lArg1, U lArg2, V lArg3, Z* lpRet)
{
	assert( mpLuaContext );

	//1.Introducir en la pila el nombre de la funcion definida en Lua que
	//  queremos llamar.
	lua_getglobal( mpLuaContext, lacFuncName );

    //Antes de poder usar una función de Lua es necesario que se cargue el fichero
	// (script .lua) que contiene su definición. Esto se hace normalmente en el "Main.cpp->main()".

	//Chequeamos que la función esta definida en Lua
	int lbExists = lua_isfunction( mpLuaContext, -1 );
	//Si no está definida en Lua
	if ( !lbExists )
	{
		//Sacamos el nombre de la función de la pila de Lua
		Pop( 1 );
		printf( "Error: la funcion %s no existe en Lua\n", lacFuncName );
		return false;
	}

	//Si la funcion está definida en Lua...

	//2.Introducir los argumentos en la pila (la función "PushArg" está sobrecargada: dependiendo del 
	//  tipo del argumento, se llamará automáticamente a la función correspondiente).
	//Introducimos en la pila el primer argumento
	PushArg(lArg1);
	//Introducimos en la pila el segundo argumento
	PushArg(lArg2);
	//Introducimos en la pila el tercer argumento
	PushArg(lArg3);

	//3.Obtener el valor de retorno (si lo hay)
	//Si la funcion devuelve un valor, el puntero "lpRet" no será nulo.
	if ( lpRet )
	{
		//Llamamos a la función Lua que se insertó en el punto 1. para el contexto dado,
		//  indicando que tiene 3 parámetros y 1 valor de retorno.
		lua_call( mpLuaContext, 3, 1 );

		//Obtenemos el resultado (valor de retorno) del tope de la pila (la función "GetRetValue" está sobrecargada: dependiendo del 
		//  tipo del argumento, se llamará automáticamente a la función correspondiente).
		GetRetValue( lpRet );
		
		//4.Eliminar el valor de retorno de la pila.
		Pop( 1 );
	}
	//La función no devuelve ningún valor
	else
	{
		//Llamamos a la función Lua que se insertó en el punto 1. para el contexto dado,
		//  indicando que tiene 3 parámetros y 0 valores de retorno.
		lua_call( mpLuaContext, 3, 0 );
	}
	return true;
}

#endif