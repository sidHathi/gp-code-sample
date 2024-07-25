/******************************************************************************
 @File          UIVolumeMenu.h
 @Title         UIVolumeMenu Header
 @Author        Siddharth Hathi
 @Description   Header file for the UIVolumeMenu object class. Defines UIVolumeMenu
******************************************************************************/

#include "UIElement.h"
#include "UIMessage.h"
#include "UIImage.h"
#include "UIProgressBar.h"
#include "UICompositeView.h"

#ifndef _UIVOLUMEMENU_H
#define _UIVOLUMEMENU_H

/*!****************************************************************************
 @class UIVolumeMenu
 Object class. The UIVolumeMenu is the collection of UIElements that make up the
 menu section used to modify the display's brightness. It supports the app layers
 flashing element functionality, and highlight toggling between the two elements
******************************************************************************/
class UIVolumeMenu : public UIElement
{
	protected:
		// Instance variables

		// Is the menu hidden?
		bool m_hidden;

		// Is the menu currently flashed off?
		bool m_flash;

		// Is the brightness element selected?
		bool m_volumeSelected;

		// Blue brightness elements
		UICompositeView* m_selected;

		// Red brightness elements
		UICompositeView* m_deselected;
	public:
		// Publicly exported functions
		UIVolumeMenu();
		virtual bool LoadTextures(CPVRTString* const pErrorStr);
		virtual void BuildVertices();
		virtual bool Render(GLuint uiMVPMatrixLoc, UIPrinter* printer);
		virtual void Update(UIMessage updateMessage);
		virtual void Hide();
		virtual void Show();
		virtual void Delete();
};

#endif