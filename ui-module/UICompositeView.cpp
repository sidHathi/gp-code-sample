/******************************************************************************
 @File          UICompositeView.cpp
 @Title         UICompositeView
 @Author        Siddharth Hathi
 @Description   Implements the UICompositeView object defined in UICompositeView.h
******************************************************************************/

#include "UICompositeView.h"

/*!****************************************************************************
 @Function		Constructor
 @Input			x, y		Pixel-coordinates of the UICV object
 @Description	Initializes the instance variables to preset defaults
******************************************************************************/
UICompositeView::UICompositeView(float x, float y)
{
	m_bg = new UIImage(c_bgTexDefault, x, y, c_bgWidthDefault, c_bgHeightDefault);
	m_x = x;
	m_y = y;
	m_width = c_bgWidthDefault;
	m_height = c_bgHeightDefault;
	m_children = CPVRTArray<UIElement*>();
	m_text = CPVRTArray<UITextSpec>();
	m_hidden = false;
}

/*!****************************************************************************
 @Function		Constructor
 @Input			bgTex			Filename of the the background texture of the
								UICompositeView
 @Input			x, y			Pixel-coordinates of the UICV object
 @Input			width, height	Dimensions of the object
 @Description	Initializes the UICV with given background texture and dimensions.
******************************************************************************/
UICompositeView::UICompositeView(char* bgTex, float x, float y, float width, float height)
{
	if (bgTex == NULL) {
		m_bg = NULL;
	} else {
		m_bg = new UIImage(bgTex, x, y, width, height);
	}
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
	m_children = CPVRTArray<UIElement*>();
	m_text = CPVRTArray<UITextSpec>();
	m_hidden = false;
}

/*!****************************************************************************
 @Function		AddElement
 @Input			newElement		New, preinitialized element to be added
 @Description	Adds the UIElement passed as a parameter to the UICV
******************************************************************************/
void
UICompositeView::AddElement(UIElement* newElement)
{
	m_children.Append(newElement);
}

/*!****************************************************************************
 @Function		AddImage
 @Input			textureName		Filename of the image's texture
 @Input			xRel, yRel		Position of the image relative to the UICV's pos
 @Input			width, height	Dimensions of the image
 @Description	Adds a UIImage to the UICV with the supplied position, dimensions, 
 				and texture
******************************************************************************/
void
UICompositeView::AddImage(char* textureName, float xRel, float yRel, float width, float height)
{
	UIElement* newImage = new UIImage(textureName, m_x + xRel, m_y + yRel, width, height);
	m_children.Append(newImage);
}

/*!****************************************************************************
 @Function		AddText
 @Input			text			The actual text being added to the view
 @Input			color			The color of the text
 @Input			xRel, yRel		Position of th etext relative to the UICV's pos
 @Input			width, height	Dimensions of the object
 @Description	Initializes the UICompositeView with given background texture
				and dimensions.
******************************************************************************/
void
UICompositeView::AddText(char* text, GLuint color, float xRel, float yRel, float scale, UITextType updateKey, UIFont font)
{
	UITextSpec newText = { text, color, xRel, yRel, scale, updateKey, font };
	m_text.Append(newText);
}

/*!****************************************************************************
 @Function		LoadTextures
 @Output		pErrorStr		Pointer to the string returned on error
 @Description	Loads the object's textures into graphics memory
******************************************************************************/
bool
UICompositeView::LoadTextures(CPVRTString* const pErrorStr)
{
	if (m_bg != NULL && !m_bg->LoadTextures(pErrorStr)) {
		fprintf(stderr, "UICompositeView texture failed to load\n");
		return false;
	}
	for ( int i = 0; i < m_children.GetSize(); i ++ ) {
		if (m_children[i] == NULL) {
			continue;
		} else {
			if (!m_children[i]->LoadTextures(pErrorStr)) {
				fprintf(stderr, "UICompositeView texture failed to load\n");
				return false;
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
UICompositeView::BuildVertices()
{
	if (m_bg != NULL) {
		m_bg->BuildVertices();
	}
	for ( int i = 0; i < m_children.GetSize(); i ++ ) {
		if (m_children[i] == NULL) {
			continue;
		} else {
			m_children[i]->BuildVertices();
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
UICompositeView::Render(GLuint uiMVPMatrixLoc, UIPrinter* printer)
{
	if (m_hidden) {
		fprintf(stderr, "UICV hidden\n");
		return true;
	}

	// Render the background
	if (m_bg != NULL) {
		m_bg->Render(uiMVPMatrixLoc, printer);
	}

	// Render the enclosed UIElements
	if (m_children.GetSize() > 0) {
		for ( int i = 0; i < m_children.GetSize(); i ++ ) {
			if (m_children[i] == NULL) {
				continue;
			} else {
				//m_children[i]->Show();
				if (!m_children[i]->Render(uiMVPMatrixLoc, printer)) {
					fprintf(stderr, "UICompositeView render failed\n");
				}
			}
		}
	}

	// Renders the text
	if (m_text.GetSize() > 0){
		float textWidth, textHeight;
		for ( int i = 0; i < m_text.GetSize(); i ++ ) {
			UITextSpec iText = m_text[i];
			printer->Print(m_x + iText.xRel, m_y + iText.yRel, iText.scale, iText.color, iText.font, iText.text);
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
UICompositeView::Update(UIMessage updateMessage)
{
	if (m_children.GetSize() > 0) {
		for (int i = 0; i < m_children.GetSize(); i ++ ) {
			m_children[i]->Update(updateMessage);
		}
	}

	if (m_text.GetSize() > 0) {
		for ( int i = 0; i < m_text.GetSize(); i ++ ){
			UITextType updateKey = m_text[i].updateKey;
			if (updateMessage.Read(updateKey) == NULL) {
				continue;
			} else {
				m_text[i].text = updateMessage.Read(updateKey);
			}
		}
	}
}

/*!****************************************************************************
 @Function		Hide
 @Description	Makes object hidden
******************************************************************************/
void
UICompositeView::Hide()
{
	m_hidden = true;
}

/*!****************************************************************************
 @Function		Hide
 @Description	Makes object visibile
******************************************************************************/
void
UICompositeView::Show()
{
	m_hidden = false;
}

/*!****************************************************************************
 @Function		Delete
 @Description	Frees any memory allocated within the object
******************************************************************************/
void
UICompositeView::Delete()
{
	if (m_bg != NULL) {
		m_bg->Delete();
		delete m_bg;
		m_bg = NULL;
	}
	if (m_children.GetSize() > 0) {
		for ( int i = 0; i < m_children.GetSize(); i++ ) {
			m_children[i]->Delete();
			delete m_children[i];
			m_children[i] = NULL;
		}
	}
}