/******************************************************************************

 @File          UITextBlock.cpp

 @Title         UITextBlock

 @Author        Siddharth Hathi

 @Description   Implements the UITextBlock object class defined in UITextBlock.h

******************************************************************************/

#include "UITextBlock.h"

/*!****************************************************************************
 @Function		Constructor
 @Description	Initializes some default values
******************************************************************************/
UITextBlock::UITextBlock()
{
    m_bg = UIImage();
    m_text = NULL;
    m_x = 0;
    m_y = 0;
    m_width = 0;
    m_height = 0;
    m_insetX = 0;
    m_insetY = 0;
	m_hidden = false;
}


/*!****************************************************************************
 @Function		Constructor
 @Input			text		The main text of the UITB
 @Input			x, y		The position of the UITB
 @Input			color		The color of the text
 @Input			updateKey	The UIMessage key used to update the text
 @Description	Initializes a UITB with only one textual element and a default
				size and background image
******************************************************************************/
UITextBlock::UITextBlock(char* text, float x, float y, GLuint color, UITextType updateKey)
{
	m_bg = UIImage(c_UITBDefaults.bg, x, y, c_UITBDefaults.width, c_UITBDefaults.height);
	m_text = text;
	m_x = x;
	m_y = y;
	m_insetX = c_UITBDefaults.insetX;
	m_insetY = c_UITBDefaults.insetY;
	m_textScale = c_UITBDefaults.textScale;
	m_color = color;
	m_updateKey = updateKey;
	m_textSecondary = NULL;
	m_fontMain = c_UITBDefaults.font;
	m_hidden = false;
}


/*!****************************************************************************
 @Function		Constructor
 @Input			text			The main text of the UITB
 @Input			textSecondary	The secondary text of the UITB
 @Input			fontMain		The font of the main text
 @Input			fontSec			The font of the secondary text
 @Input			x, y			The position of the UITB
 @Input			color			The color of the text
 @Input			updateKey		The UIMessage key used to update the text
 @Description	Initializes a UITB with two textual elements and a default
				size and background image
******************************************************************************/
UITextBlock::UITextBlock(char* text, char* textSecondary, UIFont fontMain, UIFont fontSec,
             float x, float y, GLuint color, UITextType updateKey)
{
	m_bg = UIImage(c_UITBDefaults.bg, x, y, c_UITBDefaults.width, c_UITBDefaults.height);
	m_text = text;
	m_textSecondary = textSecondary;
	m_fontMain = fontMain;
	m_fontSecondary = fontSec;
	m_x = x;
	m_y = y;
	m_insetX = c_UITBDefaults.insetX;
	m_insetY = c_UITBDefaults.insetY;
	m_textScale = c_UITBDefaults.textScale;
	m_color = color;
	m_updateKey = updateKey;
	m_hidden = false;
}

/*!****************************************************************************
 @Function		Constructor
 @Input			bg				The name of the UITB's background texture
 @Input			text			The main text of the UITB
 @Input			text			The secondary text of the UITB
 @Input			fontMain		The font of the main text
 @Input			fontSec			The font of the secondary text
 @Input			x, y			The position of the UITB
 @Input			wdth, height	The dimensions of the UITB
 @Input			color			The color of the text
 @Input			updateKey		The UIMessage key used to update the text
 @Description	Initializes a fully custom UITB
******************************************************************************/
UITextBlock::UITextBlock(char* bg, char* text, char* textSecondary, UIFont fontMain, UIFont fontSec,
             float x, float y, float width, float height, GLuint color, UITextType updateKey)
{
	m_bg = UIImage(c_UITBDefaults.bg, x, y, c_UITBDefaults.width, c_UITBDefaults.height);
	m_text = text;
	m_textSecondary = textSecondary;
	m_fontMain = fontMain;
	m_fontSecondary = fontSec;
	m_x = x;
	m_y = y;
	m_insetX = c_UITBDefaults.insetX;
	m_insetY = c_UITBDefaults.insetY;
	m_textScale = c_UITBDefaults.textScale;
	m_color = color;
	m_updateKey = updateKey;
	m_hidden = false;
}

/*!****************************************************************************
 @Function		AddSecondaryText
 @Input			text				The text to be added
 @Input			fontSecondary		The font of the subtext
 @Description	Adds a secondary subtext to a UITB initialized with only one
				textual component
******************************************************************************/
void
UITextBlock::AddSecondaryText(char* text, UIFont fontSecondary)
{
	m_textSecondary = text;
	m_fontSecondary = fontSecondary;
}

/*!****************************************************************************
 @Function		LoadTextures
 @Output		pErrorStr		Pointer to the string returned on error
 @Description	Loads the object's textures into graphics memory
******************************************************************************/
bool
UITextBlock::LoadTextures(CPVRTString* const pErrorStr)
{
    return m_bg.LoadTextures(pErrorStr);
}

/*!****************************************************************************
 @Function		BuildVertices
 @Description	Builds the object's vertex buffers
******************************************************************************/
void
UITextBlock::BuildVertices()
{
    m_bg.BuildVertices();
}

/*!****************************************************************************
 @Function		Render
 @Input			uiMVPMatrixLoc		Address of the shader's MVP matrix
 @Input			printer				UIPrinter object used to display text
 @Description	Renders the object using gl
******************************************************************************/
bool
UITextBlock::Render(GLuint uiMVPMatrixLoc, UIPrinter* printer)
{
	if (m_hidden) {
		return true;
	}

	if (printer == NULL) {
		fprintf(stderr, "Invalid call to UITextBlock::Render\n");
		return false;
	}

	m_bg.Render(uiMVPMatrixLoc, printer);
	if (m_textSecondary == NULL) {
		printer->Print(m_x, m_y, m_textScale, m_color, UIFBold, m_text);
	} else {
		float mainTextWidth, mainTextHeight, secTextWidth, secTextHeight;
		printer->Measure(&mainTextWidth, &mainTextHeight, m_textScale, m_fontMain, m_text);
		printer->Measure(&secTextWidth, &secTextHeight, m_textScale/2, m_fontSecondary, m_textSecondary);
		float mainOffset = (-secTextWidth/2);
		float secondaryOffset = (mainTextWidth/2) + 10;
		printer->Print(m_x+mainOffset, m_y, m_textScale, m_color, m_fontMain, m_text);
		printer->Print(m_x+secondaryOffset, m_y, m_textScale/2, m_color, m_fontSecondary, m_textSecondary);
	}

    return true;
}

/*!****************************************************************************
 @Function		Update
 @Input			updateMessage		UIMessage object containing frame info
 @Description	Updates the object based on information passed using UIMessage
******************************************************************************/
void
UITextBlock::Update(UIMessage updateMessage)
{
	//fprintf(stderr, "Message\n");
	char* oldText = m_text;
	if ((updateMessage.Read(m_updateKey)) != NULL) {
		m_text = new char[strlen(updateMessage.Read(m_updateKey)) + 1];
		strcpy(m_text, updateMessage.Read(m_updateKey));
		/*
		 * NOTE: By convention, it's assumed that the UITB's main text
		 * is initialized to a const string "0" that isn't stored in allocated
		 * memory. After this initialization, all updates to the main
		 * text should replace with an allocated string in memory.
		 * Any string not equal to "0" will be treated as allocated memory
		 * and freed to maximize performance
		 */
		if (strcmp("0", oldText) != 0) {
			delete oldText;
		}
	}
}

/*!****************************************************************************
 @Function		Hide
 @Description	Makes object hidden
******************************************************************************/
void
UITextBlock::Hide()
{
	m_bg.Hide();
	m_hidden = true;
}

/*!****************************************************************************
 @Function		Show
 @Description	Makes object visibile
******************************************************************************/
void
UITextBlock::Show()
{
	m_bg.Show();
	m_hidden = false;
}

/*!****************************************************************************
 @Function		Delete
 @Description	Frees any memory allocated within the object
******************************************************************************/
void
UITextBlock::Delete()
{
	/*
	 * NOTE: By convention, it's assumed that the UITB's main text
	 * is initialized to a const string "0" that isn't stored in allocated
	 * memory. After this initialization, all updates to the main
	 * text should replace with an allocated string in memory.
	 * Any string not equal to "0" will be treated as allocated memory
	 * and freed to maximize performance
	 */
	if (strcmp(m_text, "0") != 0) {
		delete m_text;
		m_text = NULL;
	}
}