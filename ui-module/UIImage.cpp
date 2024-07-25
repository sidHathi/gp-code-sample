/******************************************************************************
 @File          UIImage.cpp
 @Title         UIImage
 @Author        Siddharth Hathi
 @Description   Creates the functionality of the UIImage object class.
******************************************************************************/

#include "UIImage.h"

/*!****************************************************************************
 @Function		Constructor
 @Description	Defines default values when a UIImage is constructed without
				arguments
******************************************************************************/
UIImage::UIImage()
{
    m_texName = NULL;
    m_x = 0;
    m_y = 0;
    m_width = 0;
    m_height = 0;
    m_scale = {1, 1, 1};
	m_hidden = false;

    m_uiImgTex = 0;
    m_uiVbo = 0;
    m_uiOpaqueIndexVbo = 0;
    m_iNumOpaque = 0;
}

/*!****************************************************************************
 @Function		Constructor
 @Input			textureName		The filename of the PVR file for the image texture
 @Input			x				The x Position of the Image
 @Input			y				The y position of the Image
 @Input			width			The width of the image
 @Input			height			The height of the image
 @Description	Constructs a static image object of a given position, width, and
				height with a texture file of the given name
******************************************************************************/
UIImage::UIImage(const char* textureName, float x, float y, float width, float height)
{
    m_texName = textureName;
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    m_scale = {1, 1, 1};
	m_hidden = false;

    m_uiImgTex = 0;
    m_uiVbo = 0;
    m_uiOpaqueIndexVbo = 0;
    m_iNumOpaque = 0;
}

/*!****************************************************************************
 @Function		LoadTextures
 @Output		pErrorStr	Error message on failure
 @Description	Loads the textures specified in the UIImage's constructor
				arguments into graphics memory
******************************************************************************/
bool
UIImage::LoadTextures(CPVRTString* const pErrorStr)
{
    if (m_texName == NULL) {
		fprintf(stderr, "NULL texture\n");
        return false;
    }

    char* filename = (char*)malloc(strlen(m_texName) + strlen("../../../assets/") + 1);
    sprintf(filename, "../../../assets/%s", m_texName);
	if (loadTextureFromFilename(filename, &m_uiImgTex, NULL) != true) {
		//fprintf(stderr, "ERROR: Failed to load texture\n");
		*pErrorStr = "ERROR: Failed to load texture";
    	free(filename);
		// fclose(fp);
		return false;
	}
    free(filename);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;

    return true;
}

/*!****************************************************************************
 @Function		BuildVertices
 @Description	Creates and defines the vertices and vbos used that define the
				shape of the image.
******************************************************************************/
void
UIImage::BuildVertices()
{
	int i, row, col;
	float dim = 0.5f; //initialise vertices to normalised size, can then also be used as uv coorda and scales up after
	PVRTVec3 normal = PVRTVec3(0.0f, 1.0f, 0.0f); //all the normals are the same

    /* The images are made up of 16 vertices, 9 quads, 18 triangles. The four colours of the center vertices are fully opaque while all the 
	outside vertices are fully transparent. This produces a thin fade out at the edges which avoids antialiasing.
	0--1------2--3
	|  |      |  |
	4--5------6--7
	|  |      |  |
	|  |      |  |
	|  |      |  |
	8--9-----10--11
	|  |      |  |
	12-13----14--15
    */

    m_vertices[0].p	=	PVRTVec3(-dim, dim, 0);
	m_vertices[1].p	=	PVRTVec3(-dim, dim, 0); 
	m_vertices[2].p	=	PVRTVec3(dim, dim, 0);
	m_vertices[3].p	=	PVRTVec3(dim, dim, 0);
	m_vertices[4].p	=	PVRTVec3(-dim, dim, 0);
	m_vertices[5].p	=	PVRTVec3(-dim, dim, 0); 
	m_vertices[6].p	=	PVRTVec3(dim, dim, 0);
	m_vertices[7].p	=	PVRTVec3(dim, -dim, 0);
	m_vertices[8].p	=	PVRTVec3(-dim, -dim, 0);
	m_vertices[9].p	=	PVRTVec3(-dim, -dim, 0); 
	m_vertices[10].p	=	PVRTVec3(dim, -dim, 0);
	m_vertices[11].p	=	PVRTVec3(dim, -dim, 0);
	m_vertices[12].p	=	PVRTVec3(-dim, -dim, 0);
	m_vertices[13].p	=	PVRTVec3(-dim, -dim, 0); 
	m_vertices[14].p	=	PVRTVec3(dim, -dim, 0);
	m_vertices[15].p	=	PVRTVec3(dim, -dim, 0);

    for(i = 0; i < 16; ++i)
	{
		m_vertices[i].n = normal;
		m_vertices[i].c = PVRTVec4(1.0f, 1.0f, 1.0f, 0.0f);

		//the uvs are matched to the positions (+0.5 for range 0-1)
		m_vertices[i].t.x = m_vertices[i].p.x + 0.5f;
		m_vertices[i].t.y = m_vertices[i].p.y + 0.5f;

		//scale up to desired size
		// m_vertices[i].p.x *= m_width;
		// m_vertices[i].p.y *= m_height;

	//	m_vertices[i].p.z = 1;
	}
	
	//only the center 4 vertices are fully opaque all the rest around the edge are tranparent
	m_vertices[5].c.w = 1.0f;
	m_vertices[6].c.w = 1.0f;
	m_vertices[9].c.w = 1.0f;
	m_vertices[10].c.w = 1.0f;


	//create indices for the 2 triangles for every square
	m_iNumOpaque = 0;
	//m_iNumBlend = 0;

	for(row = 0; row < 3; ++row)
	{
		for(col = 0; col < 3; ++col)
		{
			int start = (row*4)+col;
			//the centre indices are kept in a separate buffer to the border ones as they are going
			//to be drawn in 2 separate passes. 
			if(row==1 && col == 1)
			{
				m_indicesOpaque[m_iNumOpaque++] = (unsigned short) start+1;
				m_indicesOpaque[m_iNumOpaque++] = (unsigned short) start;
				m_indicesOpaque[m_iNumOpaque++] = (unsigned short) start+4;
				m_indicesOpaque[m_iNumOpaque++] = (unsigned short) start+1;
				m_indicesOpaque[m_iNumOpaque++] = (unsigned short) start+4;
				m_indicesOpaque[m_iNumOpaque++] = (unsigned short) start+5;
			}
		}
	}
	// Enable culling
	glEnable(GL_CULL_FACE);

	m_iNumOpaque *= 2;

	//create the vertex points for the mirrored cover.
	for(row = 0; row < 4; ++row)
	{
		//the colours on the flip vertices are going to fade out the further away from the mirror they are
		// using the y coordinate of the coverpoints 
		float dark = 1 - ((m_vertices[(row*4)].p.y/m_height)+ 0.5f);
		dark -= 0.5f; //made a little darker in the reflection to mimic properties of dull mirror

		for(col = 0; col < 4; ++col)
		{
			//copy the vertices with y values reversed for the mirrored equivalents
			m_vertices[((row*4)+col) + 16] = m_vertices[((row*4)+col)];
			m_vertices[((row*4)+col) + 16].p.y *= -1.f;
			// m_coverPoints[((row*4)+col) + 16].p.y -= (height + heightFromMirror);

			m_vertices[((row*4)+col) + 16].c.x = dark;
			m_vertices[((row*4)+col) + 16].c.y = dark;
			m_vertices[((row*4)+col) + 16].c.z = dark;
		}
	}

	glGenBuffers(1, &m_uiVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVbo);
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(SVertex), m_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_uiOpaqueIndexVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiOpaqueIndexVbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2*m_iNumOpaque, m_indicesOpaque, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

/*!****************************************************************************
 @Function		Draw
 @Input			uiMVPMatrixLoc	GLuint reference to the shader Matrix
 @Input			rotate			Is the display side projected?
 @Description	Draws the image with the appropriate vertices and textures,
                with the correct positions and scales. Switches coordinate
				axes if rotate parameter is true
******************************************************************************/
void
UIImage::Draw(GLuint uiMVPMatrixLoc, bool rotate)
{
	GLint viewport[4];
    GLint vWidth;                           // Viewport width
    GLint vHeight;                          // Viewport height
	glGetIntegerv(GL_VIEWPORT, viewport);

    if (viewport != NULL) {
        vWidth = viewport[2];
        vHeight = viewport[3];
    } else {
        // Default values
        vWidth = 1280;
        vHeight = 720;
    }

	// Unless the image is hidden
	if (m_hidden) {
		return;
	}

	// float yOffset;
	// if (rotate) {
	// 	if (m_y > 0) {
	// 		yOffset = 200;
	// 	} else if (m_y < 0) {
	// 		yOffset = -200;
	// 	} else {
	// 		yOffset = 0;
	// 	}
	// }

    PVRTVec3 pos;
    // Scales pixel coordinates to device normalized coordinates,
	// rotates coordinate axes if asked to
	if (!rotate) {
		pos.x = m_x/(vWidth/2);
		pos.y = (m_y)/(vHeight/2);
	} else {
		//fprintf(stderr, "Image rotated\n");
		pos.x = -(m_y)/(vWidth/2);
		pos.y = m_x/(vHeight/2);
	}
	pos.z = 0;

    // Builds scaling, sizing, and translation matrices
	PVRTMat4 mTrans, mScale, mSize, mRotation;
	PVRTMatrixTranslation(mTrans, pos.x, pos.y, pos.z);
    mScale = PVRTMat4::Scale(m_scale);
	if (rotate) {
		mScale = PVRTMat4::Scale({m_scale.y, m_scale.x, m_scale.z});
	}

    mSize = PVRTMat4::Scale({m_width/(vWidth/2), m_height/(vHeight/2), 1});
	if (rotate) {
    	mSize = PVRTMat4::Scale({m_height/(vWidth/2), m_width/(vHeight/2), 1});
	}
	mRotation = PVRTMat4::RotationZ(0);
	if (rotate) {
		mRotation = PVRTMat4::RotationZ(-M_PI/2);
	}

    // Applies scaling and translations
	PVRTMat4 mModelView, mMVP;
	mModelView =  mTrans * mScale * mSize * mRotation;
	mMVP = mModelView;


	glUniformMatrix4fv(uiMVPMatrixLoc, 1, GL_FALSE, mMVP.f);
		
	glBindTexture(GL_TEXTURE_2D, m_uiImgTex);

    DrawMesh();
}

/*!****************************************************************************
 @Function		DrawMesh
 @Description	Draws the vertex mesh given the pregenerated vertex buffers
******************************************************************************/
void
UIImage::DrawMesh()
{
	int stride = sizeof(SVertex);

	// bind the VBO for the mesh
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVbo);

	// Set the vertex attribute offsest
	glVertexAttribPointer(IVERTEX_ARRAY, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glVertexAttribPointer(INORMAL_ARRAY, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)NormalOffset);
	glVertexAttribPointer(ICOLOR_ARRAY, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)ColorOffset);
	glVertexAttribPointer(ITEXCOORD_ARRAY, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)TexCoordOffset);
	
	//firstly draw the opaque quad in the center
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiOpaqueIndexVbo);
	glDrawElements(GL_TRIANGLES, m_iNumOpaque, GL_UNSIGNED_SHORT, 0);
}

/*!****************************************************************************
 @Function		Render
 @Input			uiMVPMatrixLoc	GLuint reference to the shader Matrix
 @Description	Sets up vertex arrays and draws the image
******************************************************************************/
bool
UIImage::Render(GLuint uiMVPMatrixLoc)
{
    glEnableVertexAttribArray(IVERTEX_ARRAY);
	glEnableVertexAttribArray(INORMAL_ARRAY);
	glEnableVertexAttribArray(ICOLOR_ARRAY);
	glEnableVertexAttribArray(ITEXCOORD_ARRAY);

    Draw(uiMVPMatrixLoc, false);

    // unbind the vertex buffers as we don't need them bound anymore
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(IVERTEX_ARRAY);
	glDisableVertexAttribArray(INORMAL_ARRAY);
	glDisableVertexAttribArray(ICOLOR_ARRAY);
	glDisableVertexAttribArray(ITEXCOORD_ARRAY);

	return true;
}


/*!****************************************************************************
 @Function		Render
 @Description	UITextBlock implements a different version of UIElement::Render
******************************************************************************/
bool
UIImage::Render(GLuint uiMVPMatrixLoc, UIPrinter* printer)
{
    glEnableVertexAttribArray(IVERTEX_ARRAY);
	glEnableVertexAttribArray(INORMAL_ARRAY);
	glEnableVertexAttribArray(ICOLOR_ARRAY);
	glEnableVertexAttribArray(ITEXCOORD_ARRAY);

    Draw(uiMVPMatrixLoc, printer->Rotated());

    // unbind the vertex buffers as we don't need them bound anymore
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(IVERTEX_ARRAY);
	glDisableVertexAttribArray(INORMAL_ARRAY);
	glDisableVertexAttribArray(ICOLOR_ARRAY);
	glDisableVertexAttribArray(ITEXCOORD_ARRAY);

	return true;
}

/*!****************************************************************************
 @Function		CreateAndDisplay
 @Input			uiMVPMatrixLoc	GLuint reference to the shader Matrix
 @Description	Simplistic function for building the entire image in one pass.
				Loads textures, builds vertices, and draws, and renders. Assumes
				that the mView and mProjection instance variables have been
				prepared by the shell using PVRTMatrixPerspectiveFovRH and
				PVRTMatrixLookAtRH.
******************************************************************************/
void
UIImage::CreateAndDisplay(GLuint uiMVPMatrixLoc)
{
	CPVRTString ErrorStr;

	if (!LoadTextures(&ErrorStr)) {
		fprintf(stderr, "%s\n", ErrorStr);
		return;
    }

	BuildVertices();

	Render(uiMVPMatrixLoc);
}

/*!****************************************************************************
 @Function		Updates
 @Description	UIImage does not currently update from UIMessages
******************************************************************************/
void
UIImage::Update(UIMessage updateMessage)
{
	return;
}

/*!****************************************************************************
 @Function		GetPosition
 @Description	Returns image position as 2d vector
******************************************************************************/
PVRTVec2
UIImage::GetPosition()
{
    return {m_x, m_y};
}


PVRTVec2
UIImage::GetSize()
{
	return {m_width, m_height};
}

/*!****************************************************************************
 @Function		Move
 @Input         newX      new x-coordinate of the image
 @Input         newY      new y-coordinate of the image
 @Description	Changes the image's scaling property by the given inputs
******************************************************************************/
void
UIImage::Move(float x, float y)
{
    m_x += x;
    m_y += y;
}

/*!****************************************************************************
 @Function		SetPosition
 @Input         newX      new x-component of image
 @Input         newY      new y-component of image
 @Description	Translates the image in the x and y directions
******************************************************************************/
void
UIImage::SetPosition(float x, float y)
{
    m_x = x;
    m_y = y;
}

void
UIImage::SetSize(float width, float height)
{
	m_width = width;
	m_height = height;
}

/*!****************************************************************************
 @Function		Scale
 @Input         scaleX      x-component of scaling vector
 @Input         scaleY      y-component of scaling vector
 @Input         scaleZ      z-component of scaling vector
 @Description	Changes the image's scaling property by the given inputs
******************************************************************************/
void
UIImage::Scale(float scaleX, float scaleY, float scaleZ)
{
    m_scale = {scaleX, scaleY, scaleZ};
}

/*!****************************************************************************
 @Function		GetProjection
 @Return		The image's projection matrix
 @Description	Returns the projection matrix
******************************************************************************/
PVRTMat4*
UIImage::GetProjection()
{
	return &m_mProjection;
}

/*!****************************************************************************
 @Function		GetView
 @Return		The image's view matrix
 @Description	Returns the view matrix
******************************************************************************/
PVRTMat4*
UIImage::GetView()
{
	return &m_mView;
}

/*!****************************************************************************
 @Function		Hide
 @Description	Sets the image's hidden property to true
******************************************************************************/
void
UIImage::Hide()
{
	m_hidden = true;
}

/*!****************************************************************************
 @Function		Show
 @Description	Sets the image's hidden property to false
******************************************************************************/
void
UIImage::Show()
{
	m_hidden = false;
}

/*!****************************************************************************
 @Function		Text
 @Return 		false	UIImage has no text
 @Description	Used by UILayer to determine which objects have textual elements
******************************************************************************/
bool
UIImage::Text()
{
	return false;
}

/*!****************************************************************************
 @Function		loadTextureFromFile
 @Input			pvr			Pointer to the pvr file	
 @Output		texture		The GLuint* pointer to the generated gl texture
 @Output		header		The header file generated by PVR
 @Description	HELPER - loads a pvr texture from a .pvr file into a GL texture
******************************************************************************/
bool
UIImage::loadTextureFromFile(FILE* pvr, GLuint* texture, PVR_Texture_Header* header)
{
    if (pvr != NULL) {
		char* fileContentPointer = file_readFile(pvr);
		PVRTTextureLoadFromPointer(fileContentPointer, texture, header, true, 0, NULL, NULL);
		free(fileContentPointer);
		return true;
	} else {
		return false;
	}
}

/*!****************************************************************************
 @Function		loadTextureFromFilename
 @Input			filename	Name of the file
 @Output		texture		The GLuint* pointer to the generated gl texture
 @Output		header		The header file generated by PVR
 @Description	HELPER - loads a pvr texture from a .pvr file into a GL texture
******************************************************************************/
bool
UIImage::loadTextureFromFilename(char* filename, GLuint* texture, PVR_Texture_Header* header)
{
	void* buffer = file_readBinary(filename);
	if (buffer != NULL) {
		PVRTTextureLoadFromPointer(buffer, texture, header, true, 0, NULL, NULL);
		free(buffer);
		return true;
	} else {
		return false;
	}
}

/*!****************************************************************************
 @Function		Delete
 @Description	UIImage has no allocated memory - therefore Delete does nothing
******************************************************************************/
void
UIImage::Delete()
{
	return;
}