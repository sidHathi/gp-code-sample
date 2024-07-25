/******************************************************************************

 @File          UITextBlock.h

 @Title         UITextBlock Header File 

 @Author        Siddharth Hathi

 @Description   Header file for the UITextBlock class.

******************************************************************************/

#include "OGLES2Tools.h"
#include "UIElement.h"
#include "UIImage.h"

#ifndef _UITEXTBLOCK_H
#define _UITEXTBLOCK_H

// Data structure for storing information about a text with background
struct TextblockData {
	char* bg;
	float width, height, insetX, insetY, textScale;
	UIFont font;
};

// Default textblock appearance
const TextblockData c_UITBDefaults = {
	"peloBox.pvr", 200, 90, 0, -5, 0.55, UIFont::UIFBold
};

/*!****************************************************************************
 @class UITextBlock
 Object class. A UITextBlock is a UIElement that loads, builds, and renders a
 centered peice of text over a UIImage background. Additional functionality added
 in this version to support two text objects within the UITextBlock - a main peice
 of text that updates through the UIElement update functionality and a secondary
 fixed subtext that displays to the right of the main text. 
******************************************************************************/
class UITextBlock : public UIElement
{
    protected:
		// Instance variables

		// The key used to access update info for the textblock within a UIMessage
        UITextType m_updateKey;

		// The background image for the UITB
        UIImage m_bg;

		// The main text in the UITB
        char* m_text;

		// The font of the main text element
        UIFont m_fontMain;

		// The secondary subtext in the UITB
        char* m_textSecondary;

		// The font of the secondary text element
        UIFont m_fontSecondary;

		// The color of the text
        GLuint m_color;

		// The positioning and dimensions of the UITB
        float m_x, m_y, m_width, m_height, m_insetX, m_insetY, m_textScale;

		// Is it hidden?
		bool m_hidden;

    public:
		// Exported functions
        UITextBlock();
		UITextBlock(char* text, float x, float y, GLuint color, UITextType m_updateKey);
		UITextBlock(char* text, char* textSecondary, UIFont fontMain, UIFont fontSec,
             float x, float y, GLuint color, UITextType m_updateKey);
		UITextBlock(char* bg, char* text, char* textSecondary, UIFont fontMain, UIFont fontSec,
             float x, float y, float width, float height, GLuint color, UITextType updateKey);
		void AddSecondaryText(char* textSecondary, UIFont fontSecondary);
        virtual bool LoadTextures(CPVRTString* const pErrorStr);
        virtual void BuildVertices();
        virtual bool Render(GLuint uiMVPMatrixLoc, UIPrinter* printer);
		virtual void Update(UIMessage updateMessage);
		virtual void Hide();
		virtual void Show();
		virtual void Delete();
};

#endif _UITEXTBLOCK_H