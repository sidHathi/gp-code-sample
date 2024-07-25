/******************************************************************************

 @File          UIPauseView.cpp

 @Title         UIPauseView

 @Author        Siddharth Hathi

 @Description   Implements the functionality of the UIPauseView class defined
				in UIPauseView.h

******************************************************************************/

#include "UIPauseView.h"

/*!****************************************************************************
 @Function		Constructor
 @Description	Initializes all the elements in the UIPV based on class constant
				layout specifications in UIPauseView.h
******************************************************************************/
UIPauseView::UIPauseView()
{
	m_hidden = false;
	m_elements = new UIElement*[c_numPVElements];
	for ( int i = 0; i < c_numPVElements; i ++ ) {
		PVLayoutSpec layoutSpec = c_PVLayoutSpecs[i];
		switch(layoutSpec.type) {
			case PVIcon:
				m_elements[i] = new UIImage("pause.pvr", layoutSpec.x, layoutSpec.y, layoutSpec.width, layoutSpec.height);
				break;
			case PVResumeButton:
				m_elements[i] = new UIButton("Resume", layoutSpec.x, layoutSpec.y, 0xFFFFFFFF, 0xFFFFFFFF, UIResumeButtonActive);
				break;
			case PVVolumeButton:
				m_elements[i] = new UIButton("Volume", layoutSpec.x, layoutSpec.y, 0xFFFFFFFF, 0xFFFFFFFF, UIVolumeButtonActive);
				break;
			case PVBrightnessButton:
				m_elements[i] = new UIButton("Brightness", layoutSpec.x, layoutSpec.y, 0xFFFFFFFF, 0xFFFFFFFF, UIBrightButtonActive);
				break;
			case PVEndButton:
				m_elements[i] = new UIButton("End", layoutSpec.x, layoutSpec.y, 0xFFFFFFFF, 0xFFFFFFFF, UIEndButtonActive);
				break;
			default:
				m_elements[i] = NULL;
				break;
		}
	}
}

/*!****************************************************************************
 @Function		LoadTextures
 @Output		pErrorStr		Pointer to the string returned on error
 @Description	Loads the object's textures into graphics memory
******************************************************************************/
bool
UIPauseView::LoadTextures(CPVRTString* const pErrorString)
{
	for ( int i = 0 ; i < c_numPVElements; i ++ ) {
		if (m_elements == NULL || !m_elements[i]->LoadTextures(pErrorString)) {
			fprintf(stderr, "UIPauseView item texture failed to load\n");
			return false;
		}
	}
	return true;
}

/*!****************************************************************************
 @Function		BuildVertices
 @Description	Builds the object's vertex buffers
******************************************************************************/
void
UIPauseView::BuildVertices()
{
	for ( int i = 0 ; i < c_numPVElements; i ++ ) {
		if (m_elements[i] != NULL) {
			m_elements[i]->BuildVertices();
		}
	}
}

/*!****************************************************************************
 @Function		Render
 @Input			uiMVPMatrixLoc		Address of the shader's MVP matrix
 @Input			printer				UIPrinter object used to display text
 @Description	Renders the object using gl
******************************************************************************/
bool
UIPauseView::Render(GLuint uiMVPMatrixLoc, UIPrinter* printer)
{
	if (m_hidden) {
		return true;
	}
	for ( int i = 0 ; i < c_numPVElements; i ++ ) {
		if (m_elements == NULL || !m_elements[i]->Render(uiMVPMatrixLoc, printer)) {
			fprintf(stderr, "UIPauseView item render failed\n");
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
UIPauseView::Update(UIMessage updateMessage)
{
	if (updateMessage.ReadState() != UIPause) {
		// fprintf(stderr, "State: UISummary\n");
		m_hidden = true;
		return;
	} else {
		m_hidden = false;
	}
	for ( int i = 0; i < c_numPVElements; i ++ ) {
		m_elements[i]->Update(updateMessage);
	}
}

/*!****************************************************************************
 @Function		Hide
 @Description	Makes object hidden
******************************************************************************/
void
UIPauseView::Hide()
{
	m_hidden = true;
}

/*!****************************************************************************
 @Function		Show
 @Description	Makes object visibile
******************************************************************************/
void
UIPauseView::Show()
{
	m_hidden = false;
}

/*!****************************************************************************
 @Function		ResumeSelected
 @Return		bool	Is the resume button selected
 @Description	Getter for m_resume
******************************************************************************/
bool
UIPauseView::ResumeSelected()
{
	return m_resume;
}

/*!****************************************************************************
 @Function		Delete
 @Description	Frees any memory allocated within the object
******************************************************************************/
void
UIPauseView::Delete()
{
	for ( int i = 0 ; i < c_numPVElements; i ++ ) {
		if (m_elements == NULL && m_elements[i] != NULL) {
			delete m_elements[i];
			m_elements[i] = NULL;
		}
	}
}