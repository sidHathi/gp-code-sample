/******************************************************************************

 @File          UIPWorkoutView.h

 @Title         UIPWorkoutView Header

 @Author        Siddharth Hathi

 @Description   Header file for the UIPWorkoutView object class. Defines UIPWorkoutView

******************************************************************************/


#include "OGLES2Tools.h"
#include "UIElement.h"
#include "UIMessage.h"
#include "UIImage.h"
#include "UIProgressBar.h"

#ifndef _UIPWORKOUTVIEW_H
#define _UIPWORKOUTVIEW_H

const float c_uipwvWidth = 465;
const float c_uipwvHeight = 50;
const float c_uipwvPaddingHorizontal = 10;
const float c_uipwvPaddingVertical = 10;
const float c_uipwvActiveBarSize = 0.5;
const int c_uipwvNumElements = 5;
const PVRTVec2 c_uipwvPosition = {105, 450};

enum UIPWVElem {
    BG, SINGLE, PB1, PB2, PB3
};

/*!****************************************************************************
 @class UIPWorkoutView
 Object class. The UIPWorkoutView is a peloton specific component of the workout
 UI that houses the progress bar, and workout stage indicator icons. Depending,
 on the current workout stage specified in the UIMessage updates, it displays a
 different set of icons and modulates the color of the progress bar.
******************************************************************************/
class UIPWorkoutView : public UIElement
{
    protected:
		// Instance variables

		// Is the view hidden?
        bool m_hidden;

        bool m_multiBars;;

        float m_x, m_y, m_width, m_height;

        // float m_pb1Width;
        // float m_pb2Width;
        // float m_pb3Width;

        // float totalWidth;
        // float verticalPadding;
        // float horizontalPaddingInternal;

        int m_activeBar;

		// The current peloton workout stage
        UIWorkoutStage m_state;

        UIElement** m_elements;

		// A map from each possible workout stage to the array of elements that
		// should be displayed in that workout stage
        // CPVRTMap<UIWorkoutStage, UIElement**> m_stateMap;

    public:
		// Exported functions
        UIPWorkoutView();
        UIPWorkoutView(UIWorkoutStage startingState);
		virtual bool LoadTextures(CPVRTString* const pErrorString);
		virtual void BuildVertices();
        virtual bool Render(GLuint uiMVPMatrixLoc, UIPrinter* printer);
		virtual void Update(UIMessage updateMessage);
		virtual void Hide();
		virtual void Show();
		virtual void Delete();
};

#endif