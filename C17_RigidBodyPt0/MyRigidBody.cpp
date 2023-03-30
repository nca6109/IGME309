#include "MyRigidBody.h"
using namespace BTX;
//Accessors
bool MyRigidBody::GetVisible(void) { return m_bVisible; }
float MyRigidBody::GetRadius(void) { return m_fRadius; }
vector3 MyRigidBody::GetColor(void) { return m_v3Color; }
void MyRigidBody::SetColor(vector3 a_v3Color) { m_v3Color = a_v3Color; }
vector3 MyRigidBody::GetCenterLocal(void) { return m_v3Center; }
vector3 MyRigidBody::GetMinLocal(void) { return m_v3MinL; }
vector3 MyRigidBody::GetMaxLocal(void) { return m_v3MaxL; }
vector3 MyRigidBody::GetCenterGlobal(void){	return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
vector3 MyRigidBody::GetMinGlobal(void) { return m_v3MinG; }
vector3 MyRigidBody::GetMaxGlobal(void) { return m_v3MaxG; }
vector3 MyRigidBody::GetHalfWidth(void) { return m_v3HalfWidth; }
matrix4 MyRigidBody::GetModelMatrix(void) { return m_m4ToWorld; }

void MyRigidBody::SetModelMatrix(matrix4 a_m4ModelMatrix) 
{ 
	if (m_m4ToWorld == a_m4ModelMatrix)
		return;

	m_m4ToWorld = a_m4ModelMatrix; 
	std::vector<vector3> cornerList;

	cornerList.push_back(vector3(m_v3MinL.x, m_v3MinL.y, m_v3MinL.z));
	cornerList.push_back(vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MinL.z));
	cornerList.push_back(vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MinL.z));
	cornerList.push_back(vector3(m_v3MaxL.x, m_v3MaxL.y, m_v3MinL.z));

	cornerList.push_back(vector3(m_v3MinL.x, m_v3MinL.y, m_v3MaxL.z));
	cornerList.push_back(vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MaxL.z));
	cornerList.push_back(vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MaxL.z));
	cornerList.push_back(vector3(m_v3MaxL.x, m_v3MaxL.y, m_v3MaxL.z));

	for (uint i = 0; i < 8; i++)
	{
		cornerList[i] = GlobalizeVector(cornerList[i]);
	}

	MyRigidBody oTemp(cornerList);
	m_v3MinG = oTemp.m_v3MinL;
	m_v3MaxG = oTemp.m_v3MaxL;
}
//Allocation
void MyRigidBody::Init(void)
{
	m_pMeshMngr = ModelManager::GetInstance();
	m_bVisible = true;

	m_fRadius = 0.0f;

	m_v3Color = C_WHITE;

	m_v3Center = ZERO_V3;
	m_v3MinL = ZERO_V3;
	m_v3MaxL = ZERO_V3;

	m_v3MinG = ZERO_V3;
	m_v3MaxG = ZERO_V3;

	m_v3HalfWidth = ZERO_V3;

	m_m4ToWorld = IDENTITY_M4;
}
void MyRigidBody::Swap(MyRigidBody& other)
{
	std::swap(m_pMeshMngr , other.m_pMeshMngr);
	std::swap(m_bVisible , other.m_bVisible);

	std::swap(m_fRadius, other.m_fRadius);

	std::swap(m_v3Color , other.m_v3Color);

	std::swap(m_v3Center , other.m_v3Center);
	std::swap(m_v3MinL , other.m_v3MinL);
	std::swap(m_v3MaxL , other.m_v3MaxL);

	std::swap(m_v3MinG , other.m_v3MinG);
	std::swap(m_v3MaxG , other.m_v3MaxG);

	std::swap(m_v3HalfWidth , other.m_v3HalfWidth);

	std::swap(m_m4ToWorld , other.m_m4ToWorld);
}
void MyRigidBody::Release(void)
{
	m_pMeshMngr = nullptr;
}
//The big 3
MyRigidBody::MyRigidBody(std::vector<vector3> a_pointList)
{
	Init();
	uint uCount = a_pointList.size();

	//Wrong route: finding avg of pts
	//if (a_pointList.size() < 1)
	//	return;
	////Find the center
	//m_v3Center = a_pointList[0];
	//for (uint i = 1; i < uCount; i++)
	//{
	//	m_v3Center += a_pointList[i];
	//}
	//m_v3Center = m_v3Center / static_cast<float>(uCount);
	////Find furthest point from center
	//for (uint i = 0; i < uCount; i++)
	//{
	//	float fDistance = glm::distance(m_v3Center, a_pointList[i]);
	//	if (m_fRadius < fDistance)
	//		m_fRadius = fDistance;
	//	//fDistance = std::max(m_fRadius, fDistance); //Another way of doing the if statement above

	//}

	//Right way
	if (a_pointList.size() < 1)
		return;
	//Find the center
	m_v3Center = a_pointList[0];
	m_v3MaxL = m_v3MinL = a_pointList[0];
	for (uint i = 1; i < uCount; i++)
	{
		//Maxes
		if (m_v3MaxL.x < a_pointList[i].x)
			m_v3MaxL.x = a_pointList[i].x;
		if (m_v3MaxL.y < a_pointList[i].y)
			m_v3MaxL.y = a_pointList[i].y;
		if (m_v3MaxL.z < a_pointList[i].z)
			m_v3MaxL.z = a_pointList[i].z;
		//Mins
		if (m_v3MinL.x > a_pointList[i].x)
			m_v3MinL.x = a_pointList[i].x;
		if (m_v3MinL.y > a_pointList[i].y)
			m_v3MinL.y = a_pointList[i].y;
		if (m_v3MinL.z > a_pointList[i].z)
			m_v3MinL.z = a_pointList[i].z;
	}

	m_v3Center = (m_v3MaxL + m_v3MinL) / 2.0f;
	//Find furthest point from center
	//Closer but more taxing bounding sphere
	for (uint i = 0; i < uCount; i++)
	{
		float fDistance = glm::distance(m_v3Center, a_pointList[i]);
		if (m_fRadius < fDistance)
			m_fRadius = fDistance;
		//fDistance = std::max(m_fRadius, fDistance); //Another way of doing the if statement above
	}
	//Least expensive option
	/*m_fRadius = glm::distance(m_v3Center, m_v3MaxL);*/
	m_v3HalfWidth = (m_v3MaxL - m_v3MinL) / 2.0f;
}
MyRigidBody::MyRigidBody(MyRigidBody const& other)
{
	m_pMeshMngr = m_pMeshMngr;
	m_bVisible = m_bVisible;

	m_fRadius = m_fRadius;

	m_v3Color = m_v3Color;

	m_v3Center = m_v3Center;
	m_v3MinL = m_v3MinL;
	m_v3MaxL = m_v3MaxL;

	m_v3MinG = m_v3MinG;
	m_v3MaxG = m_v3MaxG;

	m_v3HalfWidth = m_v3HalfWidth;

	m_m4ToWorld = m_m4ToWorld;
}
MyRigidBody& MyRigidBody::operator=(MyRigidBody const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		MyRigidBody temp(other);
		Swap(temp);
	}
	return *this;
}
MyRigidBody::~MyRigidBody(){Release();};

vector3 MyRigidBody::GlobalizeVector(vector3 a_input)
{
	return m_m4ToWorld * vector4(a_input, 1.0f);
}

//--- Non Standard Singleton Methods
void MyRigidBody::AddToRenderList(void)
{
	if (!m_bVisible)
		return;

	matrix4 m4Transform = m_m4ToWorld;

	m4Transform = m4Transform*glm::translate(IDENTITY_M4, m_v3Center); //move sphere to center
	m4Transform = m4Transform * glm::scale(IDENTITY_M4, vector3(m_fRadius, m_fRadius, m_fRadius)); //scale to size of the radius
	//m_pMeshMngr->AddWireSphereToRenderList(m4Transform, m_v3Color);
	m4Transform = m_m4ToWorld;
	m4Transform = m4Transform*glm::translate(IDENTITY_M4, m_v3Center);
	m4Transform = m4Transform * glm::scale(2.0f*m_v3HalfWidth);
	m_pMeshMngr->AddWireCubeToRenderList(m4Transform, m_v3Color);

	vector3 v3ARBBSize = m_v3MaxG - m_v3MinG;
	//m4Transform = m_m4ToWorld;
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, GlobalizeVector(m_v3Center)) * glm::scale(v3ARBBSize), C_YELLOW);
}
bool MyRigidBody::IsColliding(MyRigidBody* const other)
{
	bool bColliding;
	/*Two circles will be colliding if the sum of their radii is more than the distance of their centers*/
	float fDistance = glm::distance(GlobalizeVector(this->m_v3Center), other->GlobalizeVector(other->m_v3Center));

	bColliding = (this->m_fRadius + other->m_fRadius) > fDistance;
	m_v3MinG = GlobalizeVector(m_v3MinL);
	m_v3MaxG = GlobalizeVector(m_v3MaxL);

	other->m_v3MinG = other->GlobalizeVector(other->m_v3MinL);
	other->m_v3MaxG = other->GlobalizeVector(other->m_v3MaxL);

	bColliding = true;
	if (m_v3MinG.x > other->m_v3MaxG.x)
		bColliding = false;
	if (m_v3MaxG.x < other->m_v3MinG.x)
		bColliding = false;
	if (m_v3MinG.y > other->m_v3MaxG.y)
		bColliding = false;
	if (m_v3MaxG.y < other->m_v3MinG.y)
		bColliding = false;
	if (m_v3MinG.z > other->m_v3MaxG.z)
		bColliding = false;
	if (m_v3MaxG.z < other->m_v3MinG.z)
		bColliding = false;

	if (bColliding)
	{
		m_v3Color = vector3(1.0f, 0.0f, 0.0f);
		other->m_v3Color = vector3(1.0f, 0.0f, 0.0f);
	}
	return false;
}