/******************************************************************************

 @File          UIPTopView.h

 @Title         UIPTopView Header

 @Author        Siddharth Hathi

 @Description   Header file for the UIPTopView object class. Defines UIPTopView

******************************************************************************/

#include "OGLES2Tools.h"
#include "UIElement.h"
#include "UIImage.h"
#include "UIProgressBar.h"
#include "UITextBlock.h"
#include "UIPWorkoutView.h"

#ifndef _UIPTOPVIEW_H
#define _UIPTOPVIEW_H

// Enum identifier for fixed UIPTopView child elements
enum PTVElement { PTVLeftTB, PTVRightTB1, PTVRightTB2, PTVWorkoutView /*, progBar, warmup, workout, cooldown, progBG */};

// Data structure used to store position and sizing specifications for UIPTopView child elements
struct PTVLayoutSpec {
	PTVElement type;
	float x, y, width, height;
};

// Class constant that stores the number of fixed elements in the UIPTopView
const int c_tvPNumElementPositions = 4;

// Class const reference array that defines the position and sizing of the fixed
// UIPTopView elements
const PTVLayoutSpec c_PTVLayoutSpecs[] = {
	{ PTVLeftTB, -240, 430, 200, 90 },
	{ PTVRightTB1, 235, 355, 200, 90 },
	{ PTVRightTB2, 235, 245, 200, 90 },
	{ PTVWorkoutView, 205, 350, 465, 50 }
};

// The default color of text in the UIPTopView
const GLuint c_PTVDefaultTextColor = 0xFFFFFFFF;

/*!****************************************************************************
 @class UIPTopView
 Object class. The UIPTopView is the collection of UIElements that make up the
 information HUD at the top of the peloton workout's AR display. The UITB
 functions using a map of UI display states to element arrays. Depending on the
 UIState defined in the current frame, a different set of elements is displayed
 to the user.
******************************************************************************/
class UIPTopView : public UIElement
{
	protected:
		// Instance variables

		// The current UIState
		UIState m_state;

		// The current peloton workout stage
		UIWorkoutStage m_startingWorkoutMode;

		// A Map from possible UIStates to the elements that will be shown in the
		// UITB for those states
		CPVRTMap<UIState, UIElement**> m_stateMap;

		// Is the UITB hidden?
		bool m_hidden;
		
	public:
		// Exported functions
		UIPTopView();
		UIPTopView(UIState startingState, UIWorkoutStage startingWorkoutState);
		virtual bool LoadTextures(CPVRTString* const pErrorString);
		virtual void BuildVertices();
        virtual bool Render(GLuint uiMVPMatrixLoc, UIPrinter* printer);
		virtual void Update(UIMessage updateMessage);
		virtual void Hide();
		virtual void Show();
		void SetState(UIState state);
		UIState GetState();
		virtual void Delete();
};

#endif