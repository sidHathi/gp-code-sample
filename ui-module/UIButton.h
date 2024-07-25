/******************************************************************************

 @File          UIButton.h

 @Title         UIButton Header

 @Author        Siddharth Hathi

 @Description   Header file for the UIButton object class. Defines UIButton

******************************************************************************/

#include "OGLES2Tools.h"
#include "UIElement.h"
#include "UIImage.h"

#ifndef _UIBUTTON_H
#define _UIBUTTON_H

// Struct used to store initialization info for a UIButton
struct ButtonData {
	char* activeBG;
	char* inactiveBG;
	float width, height, insetX, insetY, textScale;
	UIFont fontActive;
	UIFont fontInactive;
};

// Global constant storing default button appearance
const ButtonData c_UIBDefaults = {
	"pButtonFill.pvr", "pButtonDim.pvr", 400, 55, 0, 0, 0.24, UIFBold, UIFMedium
};

/*!****************************************************************************
 @class UIButton
 Object class. A UIButton is a peice of text with a background that can dynamically
 switch between active and inactive states. It stores appearances for both states
 and it's update function changes the one being displayed.
******************************************************************************/
class UIButton : public UIElement
{
	protected:
		// Instance variables

		// Active background image
        UIImage m_activeBG;

		// Inactive background image
		UIImage m_inactiveBG;
		
		// Button text
        char* m_text;

		// Active font
		UIFont m_fontActive;

		// Inactive font
		UIFont m_fontInactive;

		// Active text color
        GLuint m_activeColor;

		// Inactive text color
        GLuint m_inactiveColor;

		// Is the button active
		bool m_active;
		
		// Position and sizing
        float m_x, m_y, m_width, m_height, m_insetX, m_insetY, m_textScale;
		
		// Is the button hidden
		bool m_hidden;

		// Enum key used to update the button
		UIBool m_updateKey;

	public:
		// Exported functions
        UIButton();
		UIButton(char* text, float x, float y, GLuint activeColor, GLuint inactiveColor, UIBool updateKey);
        UIButton(char* activeBG, char* inactiveBG, char* text, float x, float y, float width, float height, 
			float insetX, float insetY, float textScale, GLuint activeColor, GLuint inactiveColor, UIFont font);
		void ToggleActive();
        virtual bool LoadTextures(CPVRTString* const pErrorStr);
        virtual void BuildVertices();
        virtual bool Render(GLuint uiMVPMatrixLoc, UIPrinter* printer);
		virtual void Update(UIMessage updateMessage);
		virtual void Hide();
		virtual void Show();
		virtual void Delete();
};

#endif