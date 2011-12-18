#include "Vehicle.h"
#include "..\..\Physics\cPhysics.h"


int rightIndex = 0;
int upIndex = 1;
int forwardIndex = 2;

const int maxProxies = 32766;
const int maxOverlap = 65535;

///btRaycastVehicle is the interface for the constraint that implements the raycast vehicle
///notice that for higher-quality slow-moving vehicles, another approach might be better
///implementing explicit hinged-wheel constraints with cylinder collision, rather then raycasts
float	gEngineForce = 0.f;
float	gBreakingForce = 0.f;

float	maxEngineForce = 100000.f;//this should be engine/velocity dependent
float	maxBreakingForce = 10000.f;

float	gVehicleSteering = 0.f;
float	steeringIncrement = 0.04f;
float	steeringClamp = 0.3f;
float	wheelRadius = 0.5f;
float	wheelWidth = 0.4f;
float	wheelFriction = 1000;//BT_LARGE_FLOAT;
float	suspensionStiffness = 20.f;
float	suspensionDamping = 2.3f;
float	suspensionCompression = 4.4f;
float	rollInfluence = 0.1f;//1.0f;
btVector3 wheelDirectionCS0(0,-1,0);
btVector3 wheelAxleCS(-1,0,0);

btScalar suspensionRestLength(0.6);

#define CUBE_HALF_EXTENTS 1

Vehicle::Vehicle():
	m_carChassis(0),
	m_indexVertexArrays(0),
	m_vertices(0)
//	m_cameraHeight(4.f),
//	m_minCameraDistance(3.f),
//	m_maxCameraDistance(10.f)
	{
		m_vehicle = 0;
		m_wheelShape = 0;
//		m_cameraPosition = btVector3(30,30,30);
}

void Vehicle::initPhysics(){

	btCollisionShape* chassisShape = new btBoxShape(btVector3(1.f,0.5f,2.f));

	// stash this shape away
	cPhysics::Get().getCollisionShapes().push_back(chassisShape);

	btCompoundShape* compound = new btCompoundShape();
	cPhysics::Get().getCollisionShapes().push_back(compound);
	btTransform localTrans;
	localTrans.setIdentity();
	//localTrans effectively shifts the center of mass with respect to the chassis
	localTrans.setOrigin(btVector3(0,1,0));

	compound->addChildShape(localTrans,chassisShape);

	btTransform tr;
	tr.setIdentity();
	tr.setOrigin(btVector3(0,0.f,0));

	m_carChassis = cPhysics::Get().GetNewBody(compound, 800, cVec3(0.f, 0.f, 0.f)); //chassisShape
	//m_carChassis->setDamping(0.2,0.2);
	
	m_wheelShape = new btCylinderShapeX(btVector3(wheelWidth,wheelRadius,wheelRadius));
	
	ResetVehicleParams();

	/// create vehicle
	{
		
		m_vehicleRayCaster = new btDefaultVehicleRaycaster(cPhysics::Get().GetBulletWorld());
		m_vehicle = new btRaycastVehicle(m_tuning, m_carChassis, m_vehicleRayCaster);
		
		///never deactivate the vehicle
		m_carChassis->setActivationState(DISABLE_DEACTIVATION);

		cPhysics::Get().GetBulletWorld()->addVehicle(m_vehicle);

		float connectionHeight = 1.2f;

		bool isFrontWheel=true;

		//choose coordinate system
		m_vehicle->setCoordinateSystem(rightIndex, upIndex, forwardIndex);

		btVector3 connectionPointCS0(CUBE_HALF_EXTENTS-(0.3*wheelWidth),connectionHeight,2*CUBE_HALF_EXTENTS-wheelRadius);

		m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);

		connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(0.3*wheelWidth),connectionHeight,2*CUBE_HALF_EXTENTS-wheelRadius);

		m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);

		connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(0.3*wheelWidth),connectionHeight,-2*CUBE_HALF_EXTENTS+wheelRadius);

		isFrontWheel = false;
		m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);

		connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS-(0.3*wheelWidth),connectionHeight,-2*CUBE_HALF_EXTENTS+wheelRadius);

		m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);
		
		for (int i=0;i<m_vehicle->getNumWheels();i++){
			btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
			wheel.m_suspensionStiffness = suspensionStiffness;
			wheel.m_wheelsDampingRelaxation = suspensionDamping;
			wheel.m_wheelsDampingCompression = suspensionCompression;
			wheel.m_frictionSlip = wheelFriction;
			wheel.m_rollInfluence = rollInfluence;
		}
	}
}

void Vehicle::ResetVehicleParams(){
	gVehicleSteering = 0.f;
	m_carChassis->setCenterOfMassTransform(btTransform::getIdentity());
	m_carChassis->setLinearVelocity(btVector3(0,0,0));
	m_carChassis->setAngularVelocity(btVector3(0,0,0));
	cPhysics::Get().GetBulletWorld()->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_carChassis->getBroadphaseHandle(), cPhysics::Get().GetBulletWorld()->getDispatcher());
	if (m_vehicle)
	{
		m_vehicle->resetSuspension();
		for (int i=0;i<m_vehicle->getNumWheels();i++)
		{
			//synchronize the wheels with the (interpolated) chassis worldtransform
			m_vehicle->updateWheelTransform(i,true);
		}
	}
}

void Vehicle::renderme(){

	btScalar m[16];
	int i;

	btVector3 wheelColor(1,0,0);

	btVector3	worldBoundsMin,worldBoundsMax;
	cPhysics::Get().GetBulletWorld()->getBroadphase()->getBroadphaseAabb(worldBoundsMin,worldBoundsMax);

	for (i=0;i<m_vehicle->getNumWheels();i++)
	{
		//synchronize the wheels with the (interpolated) chassis worldtransform
		m_vehicle->updateWheelTransform(i,true);
		//draw wheels (cylinders)
		m_vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(m);
		//m_shapeDrawer->drawOpenGL(m,m_wheelShape,wheelColor,getDebugMode(),worldBoundsMin,worldBoundsMax);
	}

	//DemoApplication::renderme();
}

void Vehicle::Update(){
	
	{			
		int wheelIndex = 2;
		m_vehicle->applyEngineForce(gEngineForce,wheelIndex);
		m_vehicle->setBrake(gBreakingForce,wheelIndex);
		wheelIndex = 3;
		m_vehicle->applyEngineForce(gEngineForce,wheelIndex);
		m_vehicle->setBrake(gBreakingForce,wheelIndex);

		wheelIndex = 0;
		m_vehicle->setSteeringValue(gVehicleSteering,wheelIndex);
		wheelIndex = 1;
		m_vehicle->setSteeringValue(gVehicleSteering,wheelIndex);

	}
}

void Vehicle::MoveForward(float lfTimestep){
	gEngineForce = maxEngineForce * lfTimestep;
	gBreakingForce = 0.f;
}

void Vehicle::Break(float lfTimestep){
	gBreakingForce = maxBreakingForce * lfTimestep; 
	gEngineForce = 0.f;
}

void Vehicle::SteeringLeft(float lfTimestep){
	gVehicleSteering += steeringIncrement * lfTimestep;
	if (gVehicleSteering > steeringClamp)
		gVehicleSteering = steeringClamp;
}

void Vehicle::SteeringRight(float lfTimestep){
	gVehicleSteering -= steeringIncrement * lfTimestep;
	if (gVehicleSteering < -steeringClamp)
		gVehicleSteering = -steeringClamp;
}

cVec3 Vehicle::GetChasisPos(void){
	btTransform chassisWorldTrans;
	m_carChassis->getMotionState()->getWorldTransform(chassisWorldTrans);
	cMatrix lmChasisTrans = cPhysics::Get().Bullet2Local(chassisWorldTrans);

	return lmChasisTrans.GetPosition();
}

cVec3 Vehicle::GerChasisRot(){
	return  cVec3(   sinf(gVehicleSteering),  0.0f, cosf(gVehicleSteering) ); 
}