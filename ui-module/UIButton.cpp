/******************************************************************************

 @File          UIButton.cpp

 @Title         UIButton

 @Author        Siddharth Hathi

 @Description   Implements the UIButton object class defined in UIButton.h

******************************************************************************/

#include "UIButton.h"

/*!****************************************************************************
 @Function		Constructor
 @Description	Initializes some default values
******************************************************************************/
UIButton::UIButton()
{
    m_activeBG = UIImage();
	m_inactiveBG = UIImage();
    m_text = NULL;
    m_x = 0;
    m_y = 0;
    m_width = 0;
    m_height = 0;
    m_insetX = 0;
    m_insetY = 0;
}

/*!****************************************************************************
 @Function		Constructor
 @Input			text			Button's text
 @Input			x, y			Button's position
 @Input			activeColor		Text color when active
 @Input			inactiveColor	Text color when inactive
 @Input			updateKey		Enum key used to update active status
 @Description	Initializes UIButton
******************************************************************************/
UIButton::UIButton(char* text, float x, float y, GLuint activeColor, GLuint inactiveColor, UIBool updateKey)
{
	m_activeBG = UIImage(c_UIBDefaults.activeBG, x, y, c_UIBDefaults.width, c_UIBDefaults.height);
	m_inactiveBG = UIImage(c_UIBDefaults.inactiveBG, x, y, c_UIBDefaults.width, c_UIBDefaults.height);
	m_text = text;
	m_x = x;
	m_y = y;
	m_insetX = c_UIBDefaults.insetX;
	m_insetY = c_UIBDefaults.insetY;
	m_textScale = c_UIBDefaults.textScale;
	m_activeColor = activeColor;
	m_inactiveColor = inactiveColor;
	m_fontActive = c_UIBDefaults.fontActive;
	m_fontInactive = c_UIBDefaults.fontInactive;
	m_updateKey = updateKey;
}

/*!****************************************************************************
 @Function		Constructor
 @Input			activeBG		Filename of background texture when active
 @Input			inactiveBG		Filename of background texture when inactive
 @Input			text			Button's text
 @Input			x, y			Button's position
 @Input			width, height	Button's dimensions
 @Input			insetX, insetY	Difference between text position and button position
 @Input			activeColor		Text color when active
 @Input			inactiveColor	Text color when inactive
 @Input			font			Text font
 @Description	Initializes UIButton
******************************************************************************/
UIButton::UIButton(char* activeBG, char* inactiveBG, char* text, float x, float y, float width, float height, 
	float insetX, float insetY, float textScale, GLuint activeColor, GLuint inactiveColor, UIFont font)
{
    m_activeBG = UIImage(activeBG, x, y, width, height);
	m_inactiveBG = UIImage(inactiveBG, x, y, width, height);
    m_text = text;
    m_x = x;
    m_y = y;
    m_insetX = insetX;
    m_insetY = insetY;
    m_textScale = textScale;
	m_activeColor = activeColor;
	m_inactiveColor = inactiveColor;
    // m_print2D = NULL;
	m_fontActive = font;
	m_fontInactive = font;
}

/*!****************************************************************************
 @Function		ToggleActive
 @Description	Toggles button's active status
******************************************************************************/
void
UIButton::ToggleActive()
{
	m_active = !m_active;
}

/*!****************************************************************************
 @Function		LoadTextures
 @Output		pErrorStr		Pointer to the string returned on error
 @Description	Loads the object's textures into graphics memory
******************************************************************************/
bool
UIButton::LoadTextures(CPVRTString* const pErrorStr)
{
    return (m_activeBG.LoadTextures(pErrorStr) && m_inactiveBG.LoadTextures(pErrorStr));
}

/*!****************************************************************************
 @Function		BuildVertices
 @Description	Builds the object's vertex buffers
******************************************************************************/
void
UIButton::BuildVertices()
{
    m_activeBG.BuildVertices();
	m_inactiveBG.BuildVertices();
}

/*!****************************************************************************
 @Function		Render
 @Input			uiMVPMatrixLoc		Address of the shader's MVP matrix
 @Input			printer				UIPrinter object used to display text
 @Description	Renders the object using gl
******************************************************************************/
bool
UIButton::Render(GLuint uiMVPMatrixLoc, UIPrinter* printer)
{
    GLint viewport[4];
    GLint vWidth;                           // Viewport width
    GLint vHeight;                          // Viewport height
	glGetIntegerv(GL_VIEWPORT, viewport);

	vWidth = viewport[2];
	vHeight = viewport[3];

	if (m_active) {
		m_activeBG.Render(uiMVPMatrixLoc, printer);
	} else {
		m_inactiveBG.Render(uiMVPMatrixLoc, printer);
	}

	GLuint textColor;
	UIFont font;
	if (m_active) {
		textColor = m_activeColor;
		font = m_fontActive;
		// fprintf(stderr, "Active status + %d\n", m_active);
		// fprintf(stderr, "Text color : %x\n", textColor);
	} else {
		textColor = m_inactiveColor;
		font = m_fontInactive;
		// fprintf(stderr, "Text color : %x\n", textColor);
	}
    printer -> Print(m_x, m_y, m_textScale, textColor, font, m_text);
	
    return true;
}

/*!****************************************************************************
 @Function		Update
 @Input			updateMessage		UIMessage object containing frame info
 @Description	Updates the object based on information passed using UIMessage
******************************************************************************/
void
UIButton::Update(UIMessage updateMessage)
{

	m_active = updateMessage.Read(m_updateKey);
	//fprintf(stderr, "Active status + %d\n", m_active);
}

/*!****************************************************************************
 @Function		Hide
 @Description	Makes object hidden
******************************************************************************/
void
UIButton::Hide()
{
	m_hidden = true;
}

/*!****************************************************************************
 @Function		Show
 @Description	Makes object visibile
******************************************************************************/
void
UIButton::Show()
{
	m_hidden = false;
}

/*!****************************************************************************
 @Function		Delete
 @Description	Frees any memory allocated within the object
******************************************************************************/
void
UIButton::Delete()
{
	return;
}