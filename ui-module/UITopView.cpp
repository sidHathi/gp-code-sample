/******************************************************************************

 @File          UITopView.cpp

 @Title         UITopView

 @Author        Siddharth Hathi

 @Description   Implements the UITopView object class defined in UITopView.h

******************************************************************************/

#include "UITopView.h"

/*!****************************************************************************
 @Function		Constructor
 @Description	Initializes some default values
******************************************************************************/
UITopView::UITopView()
{
	m_state = UIMain;
	m_hidden = true;
}

/*!****************************************************************************
 @Function		Constructor
 @Input			startingState			The UI's starting display state
 @Input			startingWorkoutMode		The peloton workout's starting workout stage
 @Description	Initializes instance variables and child elements based on params
				and class constant specifications
******************************************************************************/
UITopView::UITopView(UIState startingState, UIWorkoutStage startingWorkoutMode)
{
	m_state = startingState;
	m_startingWorkoutMode = startingWorkoutMode;
	m_hidden = false;

	// Add UIElements
	UIElement** mainElements = new UIElement*[c_tvNumElementPositions];
	UIElement** infoElements = new UIElement*[c_tvNumElementPositions];
	UIElement** pauseElements = new UIElement*[c_tvNumElementPositions];

	for (int i = 0 ; i < c_tvNumElementPositions; i ++) {
		switch(c_TVLayoutSpecs[i].type) {
			case TVLeftTB:
				mainElements[i] = new UITextBlock("0", c_TVLayoutSpecs[i].x, c_TVLayoutSpecs[i].y, c_TVDefaultTextColor, UIRank);
				infoElements[i] = new UITextBlock("0", "/mi", UIFBold, UIFMedium, c_TVLayoutSpecs[i].x, c_TVLayoutSpecs[i].y, c_TVDefaultTextColor, UISpeedMPM);
				pauseElements[i] = new UITextBlock("0", c_TVLayoutSpecs[i].x, c_TVLayoutSpecs[i].y, c_TVDefaultTextColor, UIRank);
				break;
			case TVRightTB:
				mainElements[i] = new UITextBlock("0", "kJ", UIFBold, UIFMedium, c_TVLayoutSpecs[i].x, c_TVLayoutSpecs[i].y, c_TVDefaultTextColor, UIEnergyKJ);
				infoElements[i] = new UITextBlock("0", "mi", UIFBold, UIFMedium, c_TVLayoutSpecs[i].x, c_TVLayoutSpecs[i].y, c_TVDefaultTextColor, UIDistanceM);
				pauseElements[i] = NULL;
				break;
			case TVWorkoutView:
				mainElements[i] = new UIWorkoutView(m_startingWorkoutMode);
				infoElements[i] = new UIWorkoutView(m_startingWorkoutMode);
				pauseElements[i] = new UIWorkoutView(m_startingWorkoutMode);
				break;
			default:
				mainElements[i] = NULL, infoElements[i] = NULL, pauseElements[i] = NULL;
				break;
		}
	}

	m_stateMap[UIMain] = mainElements;
	m_stateMap[UIInfo] = infoElements;
	m_stateMap[UIPause] = pauseElements;
	m_stateMap[UISummary] = NULL;
}

/*!****************************************************************************
 @Function		LoadTextures
 @Output		pErrorStr		Pointer to the string returned on error
 @Description	Loads the object's textures into graphics memory
******************************************************************************/
bool
UITopView::LoadTextures(CPVRTString* const pErrorString)
{
	if (m_stateMap.GetSize() < 1) {
		fprintf(stderr, "Unable to load UITopView textures\n");
		return false;
	}
	// fprintf(stderr, "Map size: %d\n", m_stateMap.GetSize());
	for (int i = UIMain; i != UISummary; i ++) {
		// Loads textures for every child element in every possible state
		// fprintf(stderr, "State: %d\n", i);
		UIState iState = static_cast<UIState>(i);
		UIElement** elementArray = m_stateMap[iState];
		// fprintf(stderr, "Array pointer %p: \n", elementArray);
		// fprintf(stderr, "Size: %d\n", sizeof(elementArray));
		if (elementArray == NULL) {
			continue;
		}
		for (int i = 0 ; i < c_tvNumElementPositions; i ++) {
			// fprintf(stderr, "Element array index %d\n", i);
			if (elementArray[i] != NULL) {
				// fprintf(stderr, "Non-null element %p\n", elementArray[i]);
				//fprintf(stderr, "Textual element? %d\n", elementArray[i]->Text());
				if (!elementArray[i]->LoadTextures(pErrorString)) {
					fprintf(stderr, "Texture failed to load\n");
					return false;
				}
			}
		}
	}
	return true;
}

/*!****************************************************************************
 @Function		BuildVertices
 @Description	Builds the object's vertex buffers
******************************************************************************/
void
UITopView::BuildVertices()
{
	for (int i = 0; i < m_stateMap.GetSize(); i ++) {
		UIElement** elementArray = *m_stateMap.GetDataAtIndex(i);
		if (elementArray == NULL) {
			continue;
		}
		for (int i = 0 ; i < c_tvNumElementPositions; i ++) {
			if (elementArray[i] != NULL) {
				elementArray[i]->BuildVertices();
			}
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
UITopView::Render(GLuint uiMVPMatrixLoc, UIPrinter* printer)
{
	if (m_hidden) {
		return true;
	}
	//fprintf(stderr, "Rendering UITopView\n");

	UIElement** elementArray = m_stateMap[m_state];

	if (elementArray != NULL) {
		for (int i = 0 ; i < c_tvNumElementPositions; i ++) {
			if (elementArray[i] != NULL) {
				if (!elementArray[i]->Render(uiMVPMatrixLoc, printer)) {
					 fprintf(stderr, "UIElement %d render failed\n", i);
				} else {
					// fprintf(stderr, "UIElement %d rendered\n", i);
				}
			} else {
				//printf(stderr, "Null element index %d\n", i);
			}
		}
		return true;
	}
	return false;
}

/*!****************************************************************************
 @Function		Update
 @Input			updateMessage		UIMessage object containing frame info
 @Description	Updates the object based on information passed using UIMessage
******************************************************************************/
void
UITopView::Update(UIMessage updateMessage)
{
	if (updateMessage.ReadState() != UIMain && updateMessage.ReadState() != UIInfo && updateMessage.ReadState() != UIPause) {
		m_hidden = true;
		return;
	} else {
		m_hidden = false;
	}
	m_state = updateMessage.ReadState();
	UIElement** elementArray = m_stateMap[m_state];
	UIMessage delegateMessage;
	if (elementArray != NULL) {
		for (int i = 0 ; i < c_tvNumElementPositions; i ++) {
			if (elementArray[i] == NULL) {
				continue;
			}
			elementArray[i] -> Update(updateMessage);
		}
	}
}

/*!****************************************************************************
 @Function		Hide
 @Description	Makes object hidden
******************************************************************************/
void
UITopView::Hide()
{
	for (int i = 0; i < m_stateMap.GetSize(); i ++) {
		UIElement** elementArray = *m_stateMap.GetDataAtIndex(i);
		for (int i = 0 ; i < c_tvNumElementPositions; i ++) {
			if (elementArray[i] != NULL) {
				elementArray[i]->Hide();
			}
		}
	}
}

/*!****************************************************************************
 @Function		Show
 @Description	Makes object visibile
******************************************************************************/
void
UITopView::Show()
{
	for (int i = 0; i < m_stateMap.GetSize(); i ++) {
		UIElement** elementArray = *m_stateMap.GetDataAtIndex(i);
		for (int i = 0 ; i < c_tvNumElementPositions; i ++) {
			if (elementArray[i] != NULL) {
				elementArray[i]->Show();
			}
		}
	}
}

/*!****************************************************************************
 @Function		SetState
 @Input			state		New UI state
 @Description	Sets the m_state instance variable and sets the corresponding
				elements to visible.
******************************************************************************/
void
UITopView::SetState(UIState state)
{
	this->m_state = state;
	for (int i = UIMain; i != UISummary; i ++) {
		UIState iState = static_cast<UIState>(i);
		UIElement** elementArray = m_stateMap[iState];
		for (int i = 0 ; i < c_tvNumElementPositions; i ++) {
			if (elementArray[i] != NULL) {
				if (iState == m_state) {
					elementArray[i]->Show();
				} else {
					elementArray[i]->Hide();
				}
			}
		}
	}
}

/*!****************************************************************************
 @Function		GetState
 @Return		UIState
 @Description	Returns the current m_state variable
******************************************************************************/
UIState
UITopView::GetState()
{
	return m_state;
}

/*!****************************************************************************
 @Function		Delete
 @Description	Frees any memory allocated within the object
******************************************************************************/
void
UITopView::Delete()
{
	for ( int i = UIMain; i != UISummary; i++ ) {
		UIState iState = static_cast<UIState>(i);
		UIElement** elementArray = m_stateMap[iState];
		for (int i = 0 ; i < c_tvNumElementPositions; i ++) {
			if (elementArray[i] != NULL) {
				elementArray[i]->Delete();
				delete elementArray[i];
				elementArray[i] = NULL;
			}
		}
		delete elementArray;
		elementArray = NULL;
	}
}