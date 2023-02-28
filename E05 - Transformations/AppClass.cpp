#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Nick Altier - nca6109@rit.edu";
	vector3 v3Position(0.0f, 0.0f, 10.0f);
	vector3 v3Target = ZERO_V3;
	vector3 v3Upward = AXIS_Y;
	m_pCameraMngr->SetPositionTargetAndUpward(v3Position, v3Target, v3Upward);

	//Allocate the memory for the Meshes
	for (uint i = 0; i < m_uMeshCount; i++)
	{
		m_pMesh = new MyMesh();
		m_pMeshes.push_back(m_pMesh);
		m_pMeshes[i]->GenerateCube(1.0f, C_BLACK);
	}
		
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//Calculate the model, view and projection matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	//m_pMesh->Render(m4Projection, m4View, ToMatrix4(m_qArcBall));

	matrix4 m4Position = glm::translate(vector3(-5.0f,-3.0f,0.0f))*glm::translate(vector3(m_fDisplacement, 0.0f, 0.0f));
	m_fDisplacement += 0.05;
	uint i = 0;

	//first row
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3.0f, 0.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(4.0f, 0.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(6.0f, 0.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(7.0f, 0.0f, 0.0f))); ++i;

	//second row
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(0.0f, 1.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2.0f, 1.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8.0f, 1.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(10.0f, 1.0f, 0.0f))); ++i;

	//third row
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(0.0f, 2.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2.0f, 2.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3.0f, 2.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(4.0f, 2.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(5.0f, 2.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(6.0f, 2.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(7.0f, 2.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8.0f, 2.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(10.0f, 2.0f, 0.0f))); ++i;

	//fourth row
	for (uint count = 0; count < 11; ++count)
	{
		m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 3.0f, 0.0f))); ++i;
	}

	//fifth row
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(1.0f, 4.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2.0f, 4.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(4.0f, 4.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(5.0f, 4.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(6.0f, 4.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8.0f, 4.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(9.0f, 4.0f, 0.0f))); ++i;

	//sixth row
	for (uint count = 2; count < 9; ++count)
	{
		m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 5.0f, 0.0f))); ++i;
	}

	//seventh row
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3, 6.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(7, 6.0f, 0.0f))); ++i;

	//eight row
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2, 7.0f, 0.0f))); ++i;
	m_pMeshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8, 7.0f, 0.0f))); ++i;

	// draw a skybox
	m_pModelMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pModelMngr->Render();

	//clear the render list
	m_pModelMngr->ClearRenderList();

	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Release meshes
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}