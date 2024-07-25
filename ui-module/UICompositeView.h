/******************************************************************************
 @File          UICompositeView.h
 @Title         UICompositeView Header
 @Author        Siddharth Hathi
 @Description   Header file for the UICompositeView object class. Defines UICompositeView
******************************************************************************/

#include "OGLES2Tools.h"
#include "UIElement.h"
#include "UIImage.h"

#ifndef _UICOMPVIEW_H
#define _UICOMPVIEW_H

// Default background texture
const char c_bgTexDefault[] = "basicBox.pvr";
// Default dimensions
const float c_bgWidthDefault = 220;
const float c_bgHeightDefault = 70;

/*!****************************************************************************
 @class UICompoositeView
 Object class. A UICompositeView is a an object that stores, loads, builds and
 renders a collection of UIElements and text over a background UIImage. It supports 
 adding any number of images and text objects to the array of elements being displayed.
 It implements the UIElement interface 
******************************************************************************/
class UICompositeView : public UIElement
{
	protected:
		// Instance variables:

		// Is it hidden
		bool m_hidden;

		// Background UIImage
		UIImage* m_bg;

		// Position/Dimensions of the element
		float m_x, m_y, m_width, m_height;

		// Array of UIElements within the UICompositeView
		CPVRTArray<UIElement*> m_children;

		// Array of text specs contained within the UICompositeView
		CPVRTArray<UITextSpec> m_text;

	public:
		// Exported functions:
		UICompositeView(float x, float y);
		UICompositeView(char* bgTex, float x, float y, float width, float height);
		void AddElement(UIElement* newElement);
		void AddImage(char* textureName, float xRel, float yRel, float width, float height);
		void AddText(char* text, GLuint color, float xRel, float yRel, float scale, UITextType updateKey, UIFont font);
        virtual bool LoadTextures(CPVRTString* const pErrorStr);
        virtual void BuildVertices();
        virtual bool Render(GLuint uiMVPMatrixLoc, UIPrinter* printer);
		virtual void Update(UIMessage updateMessage);
		virtual void Hide();
		virtual void Show();
		virtual void Delete();
};

#endif