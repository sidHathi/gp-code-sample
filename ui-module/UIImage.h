/******************************************************************************
 @File          UIImage.h
 @Title         UIImage Header File 
 @Author        Siddharth Hathi
 @Description   Header file for the UIImage class.
******************************************************************************/

#include "PVRShell.h"
#include "OGLES2Tools.h"
#include "../file.h"
#include "UIElement.h"

#include <stdio.h>
#include <stddef.h>

#ifndef _UIIMAGE_H
#define _UIIMAGE_H

// Index to bind the attributes to vertex shaders
#define IVERTEX_ARRAY	0
#define INORMAL_ARRAY	1
#define ICOLOR_ARRAY		2
#define ITEXCOORD_ARRAY	3

// From OGLES2Coverflow
const float g_FOV = 0.78539819f;

// Struct used to store Vertex info
struct SVertex
{
	PVRTVec3	p;
	PVRTVec3	n;
	PVRTVec4	c;
	PVRTVec2	t;
};

// Class Constant
const unsigned int NormalOffset = (unsigned int)sizeof(PVRTVec3);
const unsigned int ColorOffset = NormalOffset+(unsigned int)sizeof(PVRTVec3);
const unsigned int TexCoordOffset = ColorOffset+(unsigned int)sizeof(PVRTVec4);

/*!****************************************************************************
 @class UIImage
 Object class. A UIImage is a prototype C++ class used to load, build, render,
 and display a 2d image UI component using OpenGL ES 2.0 tools. The class, 
 implemented in UIImage.cpp, constructs and stores OpenGL images with modular, 
 user-defined, and dynamic heights, widths, positions, and textures. The UIImage 
 respresents an evolution on the original StaticImage class in that it supports 
 pixel coordinate specifications and dynamic frame-by-frame scaling and movement.
******************************************************************************/
class UIImage : public UIElement
{
    protected:
		// Instance variables

		// Address of the image's texture in graphic memory
		GLuint  m_uiImgTex;

		// Address of the image's vertex buffer
        GLuint	m_uiVbo;

		// Address of image's opaque vertex buffer
        GLuint	m_uiOpaqueIndexVbo;

		// Array of image's vertices
        SVertex m_vertices[32];
		
		// Array of Image's opaque vertices
        unsigned short m_indicesOpaque[12];

		// Number of opaque vertices
	    int m_iNumOpaque;

		// Image's projection and view matrices
        PVRTMat4		m_mProjection, m_mView;

		// Name of the image's texture
		const char* m_texName;

		// Image's position and dimensions
		float m_x, m_y, m_width, m_height;

		// Image scaling matrix
        PVRTVec3 m_scale;

		// Is the image hidden
		bool m_hidden;

		// Local Function
        bool loadTextureFromFile(FILE* pvr, GLuint* texture, PVR_Texture_Header* header);
		bool loadTextureFromFilename(char* filename, GLuint* texture, PVR_Texture_Header* header);

    public:
		// Exported functions
		UIImage();
        UIImage(const char* textureName, float x, float y, float width, float height);
		virtual bool LoadTextures(CPVRTString* const pErrorStr);
		virtual void BuildVertices();
		PVRTMat4* GetProjection();
		PVRTMat4* GetView();
		PVRTVec2 GetPosition();
		PVRTVec2 GetSize();
        void Move(float x, float y);
		void SetPosition(float x, float y);
		void SetSize(float width, float height);
        void Scale(float scaleX, float scaleY, float scaleZ);
        virtual void Draw(GLuint uiMVPMatrixLoc, bool rotate);
		void DrawMesh();
		bool Render(GLuint uiMVPMatrixLoc);
        virtual bool Render(GLuint uiMVPMatrixLoc, UIPrinter* printer);
		void CreateAndDisplay(GLuint uiMVPMatrixLoc);
		virtual void Update(UIMessage updateMessage);
		virtual void Hide();
		virtual void Show();
		bool Text();
		virtual void Delete();
        
};

#endif _UIIMAGE_H