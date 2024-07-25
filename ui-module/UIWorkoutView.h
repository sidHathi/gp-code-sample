/******************************************************************************

 @File          UIWorkoutView.h

 @Title         UIWorkoutView Header

 @Author        Siddharth Hathi

 @Description   Header file for the UIWorkoutView object class. Defines UIWorkoutView

******************************************************************************/


#include "OGLES2Tools.h"
#include "UIElement.h"
#include "UIMessage.h"
#include "UIImage.h"
#include "UIProgressBar.h"

#ifndef _UIWORKOUTVIEW_H
#define _UIWORKOUTVIEW_H

// Enum identifiers for each fixed element in the view
enum WVElement { wvBG, wvWarmupIconActive, wvWarmupIconInactive, 
                    wvWorkoutIconActive, wvWorkoutIconInactive,
                    wvCooldownIconActive, wvCooldownIconInactive,
                    wvProgressBarRed, wvProgressBarGreen, wvProgressBarBlue };

// Data structure used to store the positioning and size of each fixed element
struct WVLayoutSpec {
    WVElement type;
    float x, y, width, height;
};

// Class constant for number of fixed elements in the view
const int c_numWVLayoutSpecs = 10;

// Class constant index containing the layout specification of each fixed element
const WVLayoutSpec c_wvLayoutSpecs[] = {
    { wvBG, 0, 235, 450, 30 },
    { wvWarmupIconActive, -200, 235, 12, 16 },
    { wvWarmupIconInactive, -200, 235, 12, 16 },
    { wvWorkoutIconActive, -176, 235, 12, 16 },
    { wvWorkoutIconInactive, -176, 235, 12, 16 },
    { wvCooldownIconActive, -152, 235, 12, 16 }, 
    { wvCooldownIconInactive, -152, 235, 12, 16 },
    { wvProgressBarRed, 40, 235, 450, 30 },
    { wvProgressBarGreen, 40, 235, 450, 30 },
    { wvProgressBarBlue, 40, 235, 450, 30 }
};

/*!****************************************************************************
 @class UIWorkoutView
 Object class. The UIWorkoutView is a peloton specific component of the workout
 UI that houses the progress bar, and workout stage indicator icons. Depending,
 on the current workout stage specified in the UIMessage updates, it displays a
 different set of icons and modulates the color of the progress bar.
******************************************************************************/
class UIWorkoutView : public UIElement
{
    protected:
		// Instance variables

		// Is the view hidden?
        bool m_hidden;

		// The current peloton workout stage
        UIWorkoutStage m_state;

		// A map from each possible workout stage to the array of elements that
		// should be displayed in that workout stage
        CPVRTMap<UIWorkoutStage, UIElement**> m_stateMap;

    public:
		// Exported functions
        UIWorkoutView();
        UIWorkoutView(UIWorkoutStage startingState);
		virtual bool LoadTextures(CPVRTString* const pErrorString);
		virtual void BuildVertices();
        virtual bool Render(GLuint uiMVPMatrixLoc, UIPrinter* printer);
		virtual void Update(UIMessage updateMessage);
		virtual void Hide();
		virtual void Show();
		virtual void Delete();
};

#endif