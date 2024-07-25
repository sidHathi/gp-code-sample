/******************************************************************************

 @File          UIBadges.h

 @Title         UIBadges Header

 @Author        Siddharth Hathi

 @Description   Header file for the UIBadge object class. Defines UIBadge

******************************************************************************/

#include "OGLES2Tools.h"
#include "UIElement.h"
#include "UIImage.h"

#ifndef _UIBADGES_H
#define _UIBADGES_H


// Struct used to initialize a badge image
struct UIBadge {
	char* imageName;
	float width, height;
	UIBool identifier;
};

// Number of badges stored in the project
const int c_numBadges = 3;

/*
 * Global index that specifies the filenames of
 * each acheivement badge stored in the project
 */
const UIBadge c_Badges[] = {
	{ "century.pvr", UIBadge1 },
	{ "20weeks.pvr", UIBadge2 },
	{ "badge3.pvr", UIBadge3 }
};

// The radial size of a badge
const float c_badgeDim = 40;
// The padding between each badge
const float c_badgePadding = 8;

/*!****************************************************************************
 @class UIBadges
 Object class. A UIBadges object dynamically displays acheivement badges in the
 UISummaryView. It supports loading badges from .pvr texture files, building and
 rendering the badges as UIImage elements, and dynmaically displaying multiple
 inline badges with even spacing and centering.
******************************************************************************/
class UIBadges : public UIElement
{
	protected:
		// instance variables:

		// Position:
		float m_x, m_y;

		// Map from badge identifiers to images - should contain all badges in project
		CPVRTMap<UIBool, UIImage*> m_badges;
		
		// Array of all currently displaying badges
		CPVRTArray<UIImage*> m_activeBadges;

		// Is the badge hidden
		bool m_hidden;

	public:
	 	// Exported functions
		UIBadges(float x, float y);
		bool AddBadge(UIBool identifier);
		void Test();
		virtual bool LoadTextures(CPVRTString* const pErrorStr);
        virtual void BuildVertices();
        virtual bool Render(GLuint uiMVPMatrixLoc, UIPrinter* printer);
		virtual void Update(UIMessage updateMessage);
		virtual void Hide();
		virtual void Show();
		virtual void Delete();
};

#endif