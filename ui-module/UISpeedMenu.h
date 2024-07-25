/******************************************************************************
 @File          UISpeedMenu.h
 @Title         UISpeedMenu Header
 @Author        Siddharth Hathi
 @Description   Header file for the UISpeedMenu object class. Defines UISpeedMenu
******************************************************************************/

#include "UIElement.h"
#include "UIMessage.h"
#include "UIImage.h"
#include "UIProgressBar.h"
#include "UICompositeView.h"

#ifndef _UISPEEDMENU_H
#define _UISPEEDMENU_H

// Types of statically initialized elements in the speed menu
enum UISMType {
	Title,
	Arrows,
	Value,
	Back
};

// Specification structure that stores data for each statically initialized element
struct UISMSpec {
	char* text;
	float x, y, scale, width, height;
	UISMType type;
};

// Array of layout specifications for the menu
const UISMSpec c_UISMSpecs[] = {
	{ "Speed", 0, 120, 0.5, 0, 0, Title },
	{ NULL, 120, 20, 0, 30, 40, Arrows },
	{ "00:00", -40, 20, 0.75, 0, 0, Value },
	{ "Back", 0, -200, 0.5, 0, 0, Back }
};

// number of statically initialized elements
const int c_numUISMElems = 4;

/*!****************************************************************************
 @class UISpeedMenu
 Object class. The UISpeedMenu is the collection of UIElements that make up the
 menu section used to modify the running speed. It supports the app layer's
 flashing element functionality, and highlight toggling between the two elements
******************************************************************************/
class UISpeedMenu : public UIElement
{
	protected:
		// Instance variables

		// Is the speed toggling section currently selected
		bool m_selected;

		// Is the speed currently flashed off
		bool m_flash;

		// Is the menu hidden?
		bool m_hidden;

		// The current speed string displayed to the user
		char* m_currentSpeed;

		// The UIImage object used to display the toggling arrows (white)
		UIImage* m_arrowsActive;

		// The UIImage object used to display the toggling arrows (gray)
		UIImage* m_arrowsInactive;
	public:
		// Publicly exported functions
		UISpeedMenu();
		virtual bool LoadTextures(CPVRTString* const pErrorStr);
		virtual void BuildVertices();
		virtual bool Render(GLuint uiMVPMatrixLoc, UIPrinter* printer);
		virtual void Update(UIMessage updateMessage);
		virtual void Hide();
		virtual void Show();
		virtual void Delete();
};

#endif
