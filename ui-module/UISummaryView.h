/******************************************************************************

 @File          UISummaryView.h

 @Title         UISummaryView Header File 

 @Author        Siddharth Hathi

 @Description   Header file for the UISummaryView class.

******************************************************************************/

#include "OGLES2Tools.h"
#include "UIElement.h"
#include "UITextBlock.h"
#include "UICompositeView.h"
#include "UIBadges.h"

#ifndef _UISUMMARYVIEW_H
#define _UISUMMARYVIEW_H

// Enum identifiers for fixed SummaryView elements
enum SUMElement { SVComplete, SVDistance, SVEnergy, SVPace, SVCalories, SVBadges };

// Data structure for defining fixed element dimensions and positions
struct SUMLayoutSpec {
	SUMElement type;
	float x, y, width, height;
};

// The default number of fixed elements
const int c_numSUMElements = 6;

// The default dimensions and positions of the elements corresponding to the
// SUMElement identifiers
const SUMLayoutSpec c_SUMLayouSpecs[] = {
	{ SVComplete, 0, -35, 450, 70 },
	{ SVDistance, -115, -140, 220, 70 },
	{ SVEnergy, 115, -140, 220, 70 },
	{ SVPace, -115, -260, 220, 70 },
	{ SVCalories, 115, -260, 220, 70 },
	{ SVBadges, 0, 80, 0, 0 }
};

/*!****************************************************************************
 @class UISummaryView
 Object class. A UIButton is a peice of text with a background that can dynamically
 switch between active and inactive states. It stores appearances for both states
 and it's update function changes the one being displayed.
******************************************************************************/
class UISummaryView : public UIElement
{
	protected:
		// Instance variables

		// Child element array
		UIElement** m_elements;

		// Is the view hidden?
		bool m_hidden;

	public:
		// Exported functions
		UISummaryView();
        virtual bool LoadTextures(CPVRTString* const pErrorStr);
        virtual void BuildVertices();
        virtual bool Render(GLuint uiMVPMatrixLoc, UIPrinter* printer);
		virtual void Update(UIMessage updateMessage);
		virtual void Hide();
		virtual void Show();
		virtual void Delete();
};

#endif