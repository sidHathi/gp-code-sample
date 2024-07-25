/******************************************************************************
 @File          UISpeedMenu.cpp
 @Title         UISpeedMenu
 @Author        Siddharth Hathi
 @Description   Implements the UISpeedMenu object class defined in UISpeedMenu.h
******************************************************************************/

#include "UISpeedMenu.h"

/*!****************************************************************************
 @Function		Constructor
 @Description	Initializes the UISM elements
******************************************************************************/
UISpeedMenu::UISpeedMenu()
{
	m_hidden = false;
	m_flash = false;
	m_selected = false;
	m_currentSpeed = "0";
	UISMSpec imgSpec = c_UISMSpecs[Arrows];
	m_arrowsActive = new UIImage("updown.pvr", imgSpec.x, imgSpec.y, imgSpec.width, imgSpec.height);
	m_arrowsInactive = new UIImage("updownGray.pvr", imgSpec.x, imgSpec.y, imgSpec.width, imgSpec.height);
}

/*!****************************************************************************
 @Function		LoadTextures
 @Output		pErrorStr		Pointer to the string returned on error
 @Description	Loads the object's textures into graphics memory
******************************************************************************/
bool
UISpeedMenu::LoadTextures(CPVRTString* const pErrorStr)
{
	if (m_arrowsActive != NULL & m_arrowsInactive != NULL) {
		return m_arrowsActive->LoadTextures(pErrorStr) && m_arrowsInactive->LoadTextures(pErrorStr);
	}
	return true;
}

/*!****************************************************************************
 @Function		BuildVertices
 @Description	Builds the object's vertex buffers
******************************************************************************/
void
UISpeedMenu::BuildVertices()
{
	if (m_arrowsActive != NULL & m_arrowsInactive != NULL) {
		m_arrowsActive->BuildVertices();
		m_arrowsInactive->BuildVertices();
	}
}

/*!****************************************************************************
 @Function		Render
 @Input			uiMVPMatrixLoc		Address of the shader's MVP matrix
 @Input			printer				UIPrinter object used to display text
 @Description	Renders the object using gl
******************************************************************************/
bool
UISpeedMenu::Render(GLuint uiMVPMatrixLoc, UIPrinter* printer)
{
	if (m_hidden) {
		return true;
	}
	GLuint selectedColor = 0xFFFF0000;
	GLuint deselectedColor = 0xdddddddd;
	for ( int i = 0; i < c_numUISMElems; i ++ ) {
		UISMSpec spec = c_UISMSpecs[i];
		switch (spec.type) {
			case Title:
					if (m_selected) {
						printer->Print(spec.x, spec.y, spec.scale, selectedColor, UIFBold, spec.text);
					} else {
						printer->Print(spec.x, spec.y, spec.scale, deselectedColor, UIFBold, spec.text);
					}
				break;
			case Value:
				if (!m_flash) {
					if (m_selected) {
						printer->Print(spec.x, spec.y, spec.scale, selectedColor, UIFBold, m_currentSpeed);
					} else {
						printer->Print(spec.x, spec.y, spec.scale, deselectedColor, UIFBold, m_currentSpeed);
					}
				}
				break;
			case Arrows:
				if (m_arrowsActive == NULL || m_arrowsInactive == NULL) {
					break;
				}

				if (m_selected) {
					m_arrowsActive->Render(uiMVPMatrixLoc, printer);
				} else {
					m_arrowsInactive->Render(uiMVPMatrixLoc, printer);
				}
				break;
			case Back:
				if (m_selected) {
					printer->Print(spec.x, spec.y, spec.scale, deselectedColor, UIFBold, spec.text);
				} else {
					printer->Print(spec.x, spec.y, spec.scale, selectedColor, UIFBold, spec.text);
				}
				break;
			default:
				break;
		}
	}
	return true;
}

/*!****************************************************************************
 @Function		Update
 @Input			updateMessage		UIMessage object containing frame info
 @Description	Updates the object based on information passed using UIMessage
******************************************************************************/
void
UISpeedMenu::Update(UIMessage updateMessage)
{
	if (updateMessage.ReadState() != UIMenuSpeed) {
		m_hidden = true;
		return;
	}
	m_hidden = false;
	m_selected = updateMessage.Read(UISpeedSelected);
	m_flash = updateMessage.Read(UIFlash);
	if (updateMessage.Read(UISpeedMPM) != NULL) {
		char* prevSpeed = m_currentSpeed;
		m_currentSpeed = new char[strlen(updateMessage.Read(UISpeedMPM)) + 1];
		strcpy(m_currentSpeed, updateMessage.Read(UISpeedMPM));
		if (strcmp(prevSpeed, "0") != 0) {
			delete prevSpeed;
		}
	}
	return;
}

/*!****************************************************************************
 @Function		Hide
 @Description	Makes object hidden
******************************************************************************/
void
UISpeedMenu::Hide()
{
	m_hidden = true;
}

/*!****************************************************************************
 @Function		Show
 @Description	Makes object visibile
******************************************************************************/
void
UISpeedMenu::Show()
{
	m_hidden = false;
}

/*!****************************************************************************
 @Function		Delete
 @Description	Frees memory allocated by the object
******************************************************************************/
void
UISpeedMenu::Delete()
{
	if (m_arrowsInactive != NULL) {
		m_arrowsInactive->Delete();
		delete m_arrowsInactive;
		m_arrowsInactive = NULL;
	}

	if (m_arrowsActive != NULL) {
		m_arrowsActive->Delete();
		delete m_arrowsActive;
		m_arrowsActive = NULL;
	}
}
