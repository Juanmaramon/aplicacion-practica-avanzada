#include "ChaserNoOrientation.h"
#include <string>

//Inicializa los atributos
void cChaserNoOrientation::Init(cCharacter * lpCharacter){
		mpCharacter = lpCharacter;
		mTarget = cVec3(0.0f, 0.0f, 0.0f);		
}

//Deinicializa los atributos
void cChaserNoOrientation::Deinit(){
	mpCharacter = NULL;
}

//Implementa la variante del comportamiento perseguidor
//correspondiente
void cChaserNoOrientation::Update(float lfTimestep){

	//Calcular el vector distancia (diferencia entre la posición del
	//perseguidor y la posición del objetivo a perseguir)
	cVec3 lDistanceVec = - (mTarget, mpCharacter-> GetPosition());
	//Calcular la distancia que se moverá el perseguidor teniendo en
	//cuenta su velocidad máxima
	float lfDisplacement = mpCharacter->GetSpeed() * lfTimestep;
	//Si la distancia que el perseguidor se va a desplazar es mayor o
	//igual a la distancia que hay a su objetivo, se mueve sólo hasta el
	//punto objetivo (para no pasarse). Si no, se mueve la distancia
	//establecida (para irse acercando //poco a poco al objetivo)
	if (lfDisplacement >= lDistanceVec.Length())
	{
		mpCharacter->SetPosition( mTarget );
	}
	else
	{
		//La nueva posición del perseguidor será igual a su posición inicial
		//más el desplazamiento que debe hacer en la dirección indicada por el
		//vector distancia (para moverse así en dirección a su objetivo)
		mpCharacter->SetPosition(mpCharacter->GetPosition() +
		( lfDisplacement * lDistanceVec.Normalize() ));
	}

}

//Setea el objetivo
void cChaserNoOrientation::SetTarget(cVec3 target){
	mTarget = target;
}