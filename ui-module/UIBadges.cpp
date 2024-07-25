/******************************************************************************

 @File          UIBadges.cpp

 @Title         UIBadges

 @Author        Siddharth Hathi

 @Description   Implements the UIBadges object defined in UIBadges.h

******************************************************************************/

#include "UIBadges.h"

/*!****************************************************************************
 @Function		Constructor
 @Input			x, y		Pixel-coordinates of the Badges object
 @Description	Initializes the instance variables and populates the m_badges
				map based on information stored in c_Badges.
******************************************************************************/
UIBadges::UIBadges(float x, float y)
{
	m_x = x;
	m_y = y;
	m_badges.Reserve(c_numBadges);
	m_activeBadges = CPVRTArray<UIImage*>(c_numBadges);
	for ( int i = 0; i < c_numBadges; i ++) {
		UIBadge badge = c_Badges[i];
		m_badges[badge.identifier] = new UIImage(badge.imageName, m_x, m_y, 2*c_badgeDim, 2*c_badgeDim);
	}
	m_hidden = true;
	Test();
}

/*!****************************************************************************
 @Function		Test
 @Description	Tests adding sample badges to the badges object
******************************************************************************/
void
UIBadges::Test()
{
	AddBadge(UIBadge1);
	AddBadge(UIBadge2);
	AddBadge(UIBadge3);
}

/*!****************************************************************************
 @Function		AddBadge
 @Input			identifier		UIBool used to identify the badge in the m_badges
								map
 @Description	Moves the badge specified in the m_badges map by the inputted
				indentifier into the array of badges being displayed.
******************************************************************************/
bool
UIBadges::AddBadge(UIBool identifier)
{
	if (!m_badges.Exists(identifier)) {
		return false;
	} else {
		UIImage* badge = m_badges[identifier];
		int numImages;
		if ((numImages = m_activeBadges.GetSize()) == 0) {
			m_activeBadges.Append(badge);
			return true;
		} else {
			for ( int i = 0; i < numImages; i ++ ) {
				m_activeBadges[i]->Move(-c_badgeDim-c_badgePadding, 0);
			}
			badge->Move(numImages*c_badgeDim+c_badgePadding, 0);
			m_activeBadges.Append(badge);
			return true;
		}
	}
	return true;
}

/*!****************************************************************************
 @Function		LoadTextures
 @Output		pErrorStr		Pointer to the string returned on error
 @Description	Loads the object's textures into graphics memory
******************************************************************************/
bool
UIBadges::LoadTextures(CPVRTString* const pErrorStr)
{
	if (m_badges.GetSize() > 0) {
		for ( int i = 0 ; i < m_badges.GetSize(); i ++ ) {
			if (m_badges.GetDataAtIndex(i) != NULL && !(*m_badges.GetDataAtIndex(i))->LoadTextures(pErrorStr)) {
				return false;
			} else {
				fprintf(stderr, "Badge texture successfully loaded\n");
			}
		}
	}
	return true;
}

/*!****************************************************************************
 @Function		BuildVertices
 @Description	Builds the object's vertex buffers
******************************************************************************/
void
UIBadges::BuildVertices()
{
	if (m_badges.GetSize() > 0) {
		for ( int i = 0 ; i < m_badges.GetSize(); i ++ ) {
			UIImage* badge = *m_badges.GetDataAtIndex(i);
			if (badge != NULL) {
				badge->BuildVertices();
			}
		}
	}
}

/*!****************************************************************************
 @Function		Render
 @Input			uiMVPMatrixLoc		Address of the shader's MVP matrix
 @Input			printer				UIPrinter object used to display text
 @Description	Renders the object using gl
******************************************************************************/
bool
UIBadges::Render(GLuint uiMVPMatrixLoc, UIPrinter* printer)
{
	if (m_activeBadges.GetSize() > 0 && !m_hidden) {
		for ( int i = 0 ; i < m_activeBadges.GetSize(); i ++ ) {
			UIImage* badge = m_activeBadges[i];
			if (badge != NULL && !badge->Render(uiMVPMatrixLoc, printer)) {
				return false;
			}
		}
	}
	return true;
}

/*!****************************************************************************
 @Function		Update
 @Input			updateMessage		UIMessage object containing frame info
 @Description	Updates the object based on information passed using UIMessage
******************************************************************************/
void
UIBadges::Update(UIMessage updateMessage)
{
	// Calls AddBadge if updateMessage indicates
	if (updateMessage.Read(UIBadge1)) {
		fprintf(stderr, "Adding badge\n");
		AddBadge(UIBadge1);
	} else if (updateMessage.Read(UIBadge2)) {
		AddBadge(UIBadge2);
	} else if (updateMessage.Read(UIBadge3)) {
		AddBadge(UIBadge3);
	} else {
		return;
	}
}

/*!****************************************************************************
 @Function		Hide
 @Description	Makes object hidden
******************************************************************************/
void
UIBadges::Hide()
{
	m_hidden = true;
}

/*!****************************************************************************
 @Function		Hide
 @Description	Makes object visibile
******************************************************************************/
void
UIBadges::Show()
{
	m_hidden = false;
}

/*!****************************************************************************
 @Function		Delete
 @Description	Frees any memory allocated within the object
******************************************************************************/
void
UIBadges::Delete()
{
	if (m_badges.GetSize() > 0) {
		for ( int i = 0; i < c_numBadges; i ++) {
			UIBool key = c_Badges[i].identifier;
			if (!m_badges.Exists(key) || m_badges[key] == NULL) {
				continue;
			}
			m_badges[key]->Delete();
			delete m_badges[key];
			m_badges[key] = NULL;
		}
	}
}