/******************************************************************************
 @File          UIVolumeMenu.cpp
 @Title         UIVolumeMenu
 @Author        Siddharth Hathi
 @Description   Implements the UIVolumeMenu object class defined in UIVolumeMenu.h
******************************************************************************/

#include "UIVolumeMenu.h"

/*!****************************************************************************
 @Function		Constructor
 @Description	Initializes the UIBM elements
******************************************************************************/
UIVolumeMenu::UIVolumeMenu()
{
	m_hidden = true;
	m_volumeSelected = false;
	m_selected = new UICompositeView("brightnessBG.pvr", 0, 100, 300, 300);
	m_deselected = new UICompositeView("brightnessBG.pvr", 0, 100, 300, 300);
	m_selected->AddImage("volumeImgB.pvr", 0, 25, 150, 100);
	m_deselected->AddImage("volumeImgGray.pvr", 0, 25, 150, 100);
	UIProgressBar* progBlue = new UIProgressBar(PBType::BrightnessSelected, 0, 10, 0.75);
	m_selected->AddElement(progBlue);
	UIProgressBar* progRed = new UIProgressBar(PBType::BrightnessUnselected, 0, 10, 0.75);
	m_deselected->AddElement(progRed);
}

/*!****************************************************************************
 @Function		LoadTextures
 @Output		pErrorStr		Pointer to the string returned on error
 @Description	Loads the object's textures into graphics memory
******************************************************************************/
bool
UIVolumeMenu::LoadTextures(CPVRTString* const pErrorStr)
{
	if (m_selected != NULL && m_deselected != NULL) {
		m_selected->LoadTextures(pErrorStr);
		m_deselected->LoadTextures(pErrorStr);
		return true;
	} else {
		return false;
	}
}

/*!****************************************************************************
 @Function		BuildVertices
 @Description	Builds the object's vertex buffers
******************************************************************************/
void
UIVolumeMenu::BuildVertices()
{
	if (m_selected != NULL && m_deselected != NULL) {
		m_selected->BuildVertices();
		m_deselected->BuildVertices();
	}
}

/*!****************************************************************************
 @Function		Render
 @Input			uiMVPMatrixLoc		Address of the shader's MVP matrix
 @Input			printer				UIPrinter object used to display text
 @Description	Renders the object using gl
******************************************************************************/
bool
UIVolumeMenu::Render(GLuint uiMVPMatrixLoc, UIPrinter* printer)
{
	if (m_hidden) {
		//fprintf(stderr, "UIBM hidden\n");
		return true;
	}
	if (m_selected != NULL && m_deselected != NULL) {
		//fprintf(stderr, "UIBM rendering\n");
		if (m_volumeSelected) {
			m_selected->Render(uiMVPMatrixLoc, printer);
		} else {
			m_deselected->Render(uiMVPMatrixLoc, printer);
		}
	} else {
		fprintf(stderr, "UIBM NULL element\n");
	}
	GLuint color;
	if (!m_volumeSelected) {
		color = 0xFFFF0000;
	} else {
		color = 0xdddddddd;
	}
	printer->Print(0, -200, 0.5, color, UIFBold, "Back");
	return true;
}

/*!****************************************************************************
 @Function		Update
 @Input			updateMessage		UIMessage object containing frame info
 @Description	Updates the object based on information passed using UIMessage
******************************************************************************/
void
UIVolumeMenu::Update(UIMessage updateMessage)
{
	if (m_selected == NULL || m_deselected == NULL) {
		return;
	}

	if (updateMessage.ReadState() == UIMenuVolume) {
		//fprintf(stderr, "UIBM message received\n");
		m_hidden = false;
		m_flash = updateMessage.Read(UIFlash);
		m_volumeSelected = updateMessage.Read(UIVolumeSelected);
		m_selected->Update(updateMessage);
		m_deselected->Update(updateMessage);
	} else {
		m_hidden = true;
	}
}

/*!****************************************************************************
 @Function		Hide
 @Description	Makes object hidden
******************************************************************************/
void
UIVolumeMenu::Hide()
{
	m_hidden = true;
}

/*!****************************************************************************
 @Function		Show
 @Description	Makes object visibile
******************************************************************************/
void
UIVolumeMenu::Show()
{
	m_hidden = false;
}

/*!****************************************************************************
 @Function		SetState
 @Input			state		New UI state
 @Description	Sets the m_state instance variable and sets the corresponding
				elements to visible.
******************************************************************************/
void
UIVolumeMenu::Delete()
{
	if (m_deselected != NULL) {
		m_deselected->Delete();
		delete m_deselected;
		m_deselected = NULL;
	}
	if (m_selected != NULL) {
		m_selected->Delete();
		delete m_selected;
		m_selected = NULL;
	}
	return;
}