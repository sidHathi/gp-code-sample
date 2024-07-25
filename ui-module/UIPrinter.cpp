/******************************************************************************
 @File          UIPrinter.cpp
 @Title         UIPrinter
 @Author        Siddharth Hathi
 @Description   Implements the UIPrinter object class defined in UIPrinter.h
******************************************************************************/

#include "UIPrinter.h"

/*!****************************************************************************
 @Function		Constructor
 @Description	Initializes some default values
******************************************************************************/
UIPrinter::UIPrinter()
{
	for ( int i = UIFBold; i <= UIFMedium; i ++ ) {
		UIFont iFont = static_cast<UIFont>(i);
		m_fontMap[iFont] = new CPVRTPrint3D();
	}
}

/*!****************************************************************************
 @Function		LoadFonts
 @Input			pvrShellWidth		Width of the PVR Shell
 @Input			pvrShellHeight		Height of the PVR Shell
 @Description	Loads the appropriate fonts into each of the print3D object
******************************************************************************/
bool
UIPrinter::LoadFonts(int pvrShellWidth, int pvrShellHeight, bool isRotated)
{
	fprintf(stderr, "Loading fonts \n");
	m_isRotated = isRotated;
	for ( int i = UIFBold; i <= UIFMedium; i ++ ) {
		UIFont iFont = static_cast<UIFont>(i);
		CPVRTPrint3D* print3D = m_fontMap[iFont];
		switch(iFont) {
			case UIFBold:
				if (print3D->SetTextures(0, (void *) _PelotareSans_200wt_pvr, pvrShellWidth, pvrShellHeight, isRotated) != PVR_SUCCESS) {
					fprintf(stderr, "Error loading print3D\n");
					return false;
				}
				break;
			case UIFMedium:
				if (print3D->SetTextures(0, (void *) _PelotareSans_200wt_pvr, pvrShellWidth, pvrShellHeight, isRotated) != PVR_SUCCESS) {
					fprintf(stderr, "Error loading print3D\n");
					return false;
				}
			// 	break;
			// case UIFRegular:
			// 	if (print3D->SetTextures(0, (void *) _MontserratRegular_pvr, pvrShellWidth, pvrShellHeight, isRotated) != PVR_SUCCESS) {
			// 		fprintf(stderr, "Error loading print3D\n");
			// 		return false;
			// 	}
			// 	break;
			// case UIFThin:
			// 	if (print3D->SetTextures(0, (void *) _MontserratThin_pvr, pvrShellWidth, pvrShellHeight, isRotated) != PVR_SUCCESS) {
			// 		fprintf(stderr, "Error loading print3D\n");
			// 		return false;
			// 	}
			// 	break;
			// case UIFLight:
			// 	if (print3D->SetTextures(0, (void *) _MontserratLight_pvr, pvrShellWidth, pvrShellHeight, isRotated) != PVR_SUCCESS) {
			// 		fprintf(stderr, "Error loading print3D\n");
			// 		return false;
			// 	}
			// 	break;
		}
	}
	return true;
}

/*!****************************************************************************
 @Function		Print
 @Input			x, y		Position of the text
 @Input			scale		Text's scale from 0 to 1
 @Input			color		Text color
 @Input			text		The text to print
 @Description	Prints text of the specified font, scale, and color at the
				appropriate position.
******************************************************************************/
void
UIPrinter::Print(float x, float y, float scale, GLuint color, UIFont font, char* text)
{
	if (!m_fontMap.Exists(font)) {
		fprintf(stderr, "Invalid call to UIPrinter::Print\n");
		return;
	}

	// if (m_isRotated) {
	// 	if (y > 0) {
	// 		y += 200;
	// 	} else if (y < 0) {
	// 		y -= 200;
	// 	}
	// }

	// fprintf(stderr, "UIPrinter::Print called \n");
    GLint viewport[4];
    GLint vWidth;                           // Viewport width
    GLint vHeight;                          // Viewport height
	glGetIntegerv(GL_VIEWPORT, viewport);

	vWidth = viewport[2];
	vHeight = viewport[3];

	CPVRTPrint3D* print3D = m_fontMap[font];

	float textWidth;
	float textHeight;

	print3D->MeasureText(&textWidth, &textHeight, scale, text);

	if (m_isRotated) {
		if (print3D->Print3D((100*(x - (textWidth)/2)/vHeight)+50, -(100*(y + (textHeight)/2)/vWidth)+50, scale, color, text) != PVR_SUCCESS) {
			fprintf(stderr, "UIPrint failed \n");
		}	
	} else {
		if (print3D->Print3D((100*(x - (textWidth)/2)/vWidth)+50, -(100*(y + (textHeight)/2)/vHeight)+50, scale, color, text) != PVR_SUCCESS) {
			fprintf(stderr, "UIPrint failed \n");
		}	
	}
	print3D->Flush();
}

/*!****************************************************************************
 @Function		Measure
 @Output		width		Pointer to the width of the text
 @Output		height		Poiter to the height of the text
 @Input			scale		Text scale
 @Input			font		The text's font
 @Input			text		The actual text being measured
 @Description	Measures text
******************************************************************************/
void
UIPrinter::Measure(float* width, float* height, float scale, UIFont font, char* text)
{
	if (!m_fontMap.Exists(font)) {
		fprintf(stderr, "Invalid call to UIPrinter::Measure\n");
		return;
	}

	CPVRTPrint3D* print3D = m_fontMap[font];
	print3D->MeasureText(width, height, scale, text);
}

/*!****************************************************************************
 @Function		Rotated
 @Return		bool	Is the text rotated
 @Description	Frees allocated memory within the printer
******************************************************************************/
bool
UIPrinter::Rotated()
{
	return m_isRotated;
}

/*!****************************************************************************
 @Function		Delete
 @Description	Frees allocated memory within the printer
******************************************************************************/
void
UIPrinter::Delete()
{
	for ( int i = UIFBold; i <= UIFMedium; i ++ ) {
		UIFont iFont = static_cast<UIFont>(i);
		if (m_fontMap.Exists(iFont) && m_fontMap[iFont] != NULL) {
			m_fontMap[iFont]->ReleaseTextures();
			delete m_fontMap[iFont];
			m_fontMap[iFont] = NULL;
		}
	}
}