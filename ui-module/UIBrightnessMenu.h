/******************************************************************************
 @File          UIBrightnessMenu.h
 @Title         UIBrightnessMenu Header
 @Author        Siddharth Hathi
 @Description   Header file for the UIBrightnessMenu object class. Defines UIBrightnessMenu
******************************************************************************/

#include "UIElement.h"
#include "UIMessage.h"
#include "UIImage.h"
#include "UIProgressBar.h"
#include "UICompositeView.h"

#ifndef _UIBRIGHTNESSMENU_H
#define _UIBRIGHTNESSMENU_H

/*!****************************************************************************
 @class UIBrightnessMenu
 Object class. The UIBrightnessMenu is the collection of UIElements that make up the
 menu section used to modify the display's brightness. It supports the app layers
 flashing element functionality, and highlight toggling between the two elements
******************************************************************************/
class UIBrightnessMenu : public UIElement
{
	protected:
		// Instance variables

		// Is the menu hidden?
		bool m_hidden;

		// Is the menu currently flashed off?
		bool m_flash;

		// Is the brightness element selected?
		bool m_brightSelected;

		// Blue brightness elements
		UICompositeView* m_selected;

		// Red brightness elements
		UICompositeView* m_deselected;
	public:
		// Publicly exported functions
		UIBrightnessMenu();
		virtual bool LoadTextures(CPVRTString* const pErrorStr);
		virtual void BuildVertices();
		virtual bool Render(GLuint uiMVPMatrixLoc, UIPrinter* printer);
		virtual void Update(UIMessage updateMessage);
		virtual void Hide();
		virtual void Show();
		virtual void Delete();
};

#endif