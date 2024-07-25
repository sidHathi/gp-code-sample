/******************************************************************************
 @File          UIProgressBar.h
 @Title         UIProgressBar Header File 
 @Author        Siddharth Hathi
 @Description   Header file for UIProgressBar.cpp
******************************************************************************/

#include "OGLES2Tools.h"
#include "UIElement.h"
#include "UIImage.h"

#ifndef _UIPROGRESSBAR_H
#define _UIPROGRESSBAR_H

// Names of pvr textures for the bar and it's background
const char c_progBGRTex[] = "progBGR.pvr";
const char c_progRTex[] = "progR.pvr";
const char c_progBGBTex[] = "progBGB.pvr";
const char c_progBTex[] = "progB.pvr";
const char c_progBGGTex[] = "progBGG.pvr";
const char c_progGTex[] = "progG.pvr";
const char c_brightRTex[] = "brightBarR.pvr";
const char c_brightRBGTex[] = "brightBGR.pvr";
const char c_brightBTex[] = "brightBarB.pvr";
const char c_brightBBGTex[] = "brightBGB.pvr";
const char c_brightGrayTex[] = "brightBarGray.pvr";
const char c_brightGrayBGTex[] = "brightBGGray.pvr";

// Default sizing specifications
const float c_defaultPBWidth = 350;
const float c_defaultPBHeight = 18;
const float c_defaultBrightWidth = 200;
const float c_defaultPBInsetX = 12;
const float c_defaultPBInsetY = 12;

// Enum identifiers for progress bar colors
enum PBType { PBR, PBG, PBB, BrightnessSelected, BrightnessUnselected };

/*!****************************************************************************
 @class UIProgressBar
 Object Class. A UIProgressBar is a C++ class that displays a Progress Bar that can
 be dynamically animated. It contains functionality for loading, building, rendering 
 and modifying the Progress Bar. It's functionality is implemented in UIProgressBar.cpp
******************************************************************************/
class UIProgressBar : public UIElement
{
    protected:
        // instance variables

        // The appearance identifier for the bar
        PBType m_type;

        // The empty bar UIImage
        UIImage m_bg;

        // The progress filler UIImage
        UIImage m_progress;

        // Completion from 0-1 and positional/layout details
        float m_completion, m_width, m_height, m_insetX, m_insetY, m_x, m_y;

        // Is the bar hidden?
        bool m_hidden;

        // Is the bar rotated?
        bool m_rotated;
    public:
        // Exported functions
        UIProgressBar();
        UIProgressBar(float x, float y, float initialCompletion);
        UIProgressBar(PBType type, float x, float y, float initialCompletion);
        UIProgressBar(float x, float y, float width, float height, float insetX, float insetY, float initialCompletion);
        UIProgressBar(char* bgTex, char* progTex, float x, float y, float width, float height, 
            float insetX, float insetY, float initialCompletion);
		virtual bool LoadTextures(CPVRTString* const pErrorStr);
        virtual void BuildVertices();
		virtual bool Render(GLuint uiMVPMatrixLoc);
        virtual bool Render(GLuint uiMVPMatrixLoc, UIPrinter* printer);
        void SetCompletion(float newCompletion);
        float GetCompletion();
		virtual void Update(UIMessage updateMessage);
        void SetSize(float newWidth, float newHeight);
        void SetPosition(float newX, float newY);
        PVRTVec2 GetSize();
		virtual void Hide();
		virtual void Show();
		void Delete();
};

#endif _UIPROGRESSBAR_H