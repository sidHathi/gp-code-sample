/******************************************************************************
 @File          UIPrinter.h
 @Title         UIPrinter Header File 
 @Author        Siddharth Hathi
 @Description   Header file for the UIPrinter class.
******************************************************************************/

#include "OGLES2Tools.h"

#ifndef _UIPRINTER_H
#define _UIPRINTER_H

#include "Fonts/CustomMontserratBold.h"
// #include "Fonts/MontserratMedium.h"
#include "UIMessage.h"
#include "Fonts/Pelotare200.h"
// #include "Fonts/MontserratRegular.h"
// #include "Fonts/MontserratThin.h"
// #include "Fonts/MontserratLight.h"

// Enum font identifiers
enum UIFont {
	UIFBold,
	UIFMedium,
	UIFRegular,
	UIFThin,
	UIFLight
};

// struct used to store updating text 
struct UITextSpec {
	char* text;
	GLuint color;
	float xRel, yRel, scale;
	UITextType updateKey;
	UIFont font;
};

/*!****************************************************************************
 @class UIPrinter
 Object class. A UIPrinter is a wrapper for a map of enum identifiers to CPVRTPrint3D
 objects. Each identifier represents a font and each CPVRTPrint3D object is intended
 to be preloaded with said font's textures. The UIPrinter contains functionality
 for initializing the print3D objects, measuring text, and printing via print3D
 with a desired font.
******************************************************************************/
class UIPrinter
{
	protected:
		// Instance variables

		// Is the text rotated?
	 	bool m_isRotated;

		 // Mapping from font identifiers to print3D objects
		CPVRTMap<UIFont, CPVRTPrint3D*> m_fontMap;

	public:
		// Exported functions
		UIPrinter();
		bool LoadFonts(int pvrShellWidth, int pvrShellHeight, bool isRotated);
		void Print(float x, float y, float scale, GLuint color, UIFont font, char* text);
		void Measure(float* width, float* height, float scale, UIFont font, char* text);
		bool Rotated();
		void Delete();
};

#endif