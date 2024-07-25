/******************************************************************************

 @File          UIElement.h

 @Title         UIElement

 @Author        Siddharth Hathi

 @Description   Abstract class/Interface defining a set of functions to be implemented
				by every visual element in the UI.

******************************************************************************/

#include "OGLES2Tools.h"
#include "UIMessage.h"

#ifndef _UIELEMENT_H
#define _UIELEMENT_H

#include "UIPrinter.h"

/*!****************************************************************************
 @class UIElement
 Abstract class. Defines a UIElement. Every UIElement should be able to load
 its own textures, build its vertex buffers, render itself, show/hide itself,
 and free its allocated memory. By creating a series of UIElements that all 
 implement the functionality defined in the UIElement abstract class, we can
 texture, build, and render different UI classes with different functionalities
 and purposes using the same syntax. By implementing the UIElement functions,
 in the UI we ensure uniform efficiency and clarity among UI classes.
******************************************************************************/
class UIElement
{
    public:
		// Function to load the element's textures
        virtual bool LoadTextures(CPVRTString* const pErrorStr) = 0;

		// Function to build the element's vertex buffers
        virtual void BuildVertices() = 0;

		// Function to render the element after the textures have been loaded,
		// and vertex buffers build
        virtual bool Render(GLuint uiMVPMatrixLoc, UIPrinter* printer) = 0;

		// Function to update the element with data from a protobuf
		virtual void Update(UIMessage updateMessage) = 0;

		// Function to hide the element
		virtual void Hide() = 0;

		// Function to show a hidden element
		virtual void Show() = 0;

		// Function to free allocated memory within the element
		virtual void Delete() = 0;
};

#endif _UIELEMENT_H