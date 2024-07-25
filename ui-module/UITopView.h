/******************************************************************************

 @File          UITopView.h

 @Title         UITopView Header

 @Author        Siddharth Hathi

 @Description   Header file for the UITopView object class. Defines UITopView

******************************************************************************/

#include "OGLES2Tools.h"
#include "UIElement.h"
#include "UIImage.h"
#include "UIProgressBar.h"
#include "UITextBlock.h"
#include "UIWorkoutView.h"

#ifndef _UITOPVIEW_
#define _UITOPVIEW_

// Enum identifier for fixed UITopView child elements
enum TVElement { TVLeftTB, TVRightTB, TVWorkoutView /*, progBar, warmup, workout, cooldown, progBG */};

// Data structure used to store position and sizing specifications for UITopView child elements
struct TVLayoutSpec {
	TVElement type;
	float x, y, width, height;
};

// Class constant that stores the number of fixed elements in the UITopView
const int c_tvNumElementPositions = 3;

// Class const reference array that defines the position and sizing of the fixed
// UITopView elements
const TVLayoutSpec c_TVLayoutSpecs[] = {
	{ TVLeftTB, -115, 300, 220, 70 },
	{ TVRightTB, 115, 300, 220, 70 },
	{ TVWorkoutView, 0, 235, 450, 30 }
};

// The default color of text in the UITopView
const GLuint c_TVDefaultTextColor = 0xFF0000FF;

/*!****************************************************************************
 @class UITopView
 Object class. The UITopView is the collection of UIElements that make up the
 information HUD at the top of the peloton workout's AR display. The UITB
 functions using a map of UI display states to element arrays. Depending on the
 UIState defined in the current frame, a different set of elements is displayed
 to the user.
******************************************************************************/
class UITopView : public UIElement
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
		UITopView();
		UITopView(UIState startingState, UIWorkoutStage startingWorkoutState);
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