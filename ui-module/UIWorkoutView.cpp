/******************************************************************************

 @File          UIWorkoutView.cpp

 @Title         UIWorkoutView

 @Author        Siddharth Hathi

 @Description   Implements the UIWorkoutView object class defined in UIWorkoutView.h

******************************************************************************/

#include "UIWorkoutView.h"


/*!****************************************************************************
 @Function		Constructor
 @Input			startingState		The peloton workout's starting workout stage
 @Description	Initializes instance variables and child elements based on params
				and class constant specifications
******************************************************************************/
UIWorkoutView::UIWorkoutView(UIWorkoutStage startingState)
{
    m_state = startingState;
    m_hidden = false;

    UIElement** warmupElements = new UIElement*[c_numWVLayoutSpecs];
    UIElement** workoutElements = new UIElement*[c_numWVLayoutSpecs];
    UIElement** cooldownElements = new UIElement*[c_numWVLayoutSpecs];

    for (int i = 0 ; i < c_numWVLayoutSpecs; i ++ ) {
        WVLayoutSpec spec = c_wvLayoutSpecs[i];
        switch(spec.type) {
            case wvBG:
                warmupElements[i] = new UIImage("progContainer.pvr", spec.x, spec.y, spec.width, spec.height);
                workoutElements[i] = new UIImage("progContainer.pvr", spec.x, spec.y, spec.width, spec.height);
                cooldownElements[i] = new UIImage("progContainer.pvr", spec.x, spec.y, spec.width, spec.height);
                break;
            case wvWarmupIconActive:
                warmupElements[i] = new UIImage("warmupActive.pvr", spec.x, spec.y, spec.width, spec.height);
                workoutElements[i] = NULL;
                cooldownElements[i] = NULL;
                break;
            case wvWarmupIconInactive:
                warmupElements[i] = NULL;
                workoutElements[i] = new UIImage("warmupInactive.pvr", spec.x, spec.y, spec.width, spec.height);
                cooldownElements[i] =  new UIImage("warmupInactive.pvr", spec.x, spec.y, spec.width, spec.height);
                break;
            case wvWorkoutIconActive:
                workoutElements[i] = new UIImage("workoutActive.pvr", spec.x, spec.y, spec.width, spec.height);
                warmupElements[i] = NULL;
                cooldownElements[i] = NULL;
                break;
            case wvWorkoutIconInactive:
                workoutElements[i] = NULL;
                warmupElements[i] = new UIImage("workoutInactive.pvr", spec.x, spec.y, spec.width, spec.height);
                cooldownElements[i] =  new UIImage("workoutInactive.pvr", spec.x, spec.y, spec.width, spec.height);
                break;
            case wvCooldownIconActive:
                cooldownElements[i] = new UIImage("cooldownActive.pvr", spec.x, spec.y, spec.width, spec.height);
                workoutElements[i] = NULL;
                warmupElements[i] = NULL;
                break;
            case wvCooldownIconInactive:
                cooldownElements[i] = NULL;
                workoutElements[i] = new UIImage("cooldownInactive.pvr", spec.x, spec.y, spec.width, spec.height);
                warmupElements[i] =  new UIImage("cooldownInactive.pvr", spec.x, spec.y, spec.width, spec.height);
                break;
            case wvProgressBarGreen:
                warmupElements[i] = new UIProgressBar(PBType::PBG, spec.x, spec.y, 1.f);
                workoutElements[i] = NULL;
                cooldownElements[i] = NULL;
                break;
            case wvProgressBarRed:
                workoutElements[i] = new UIProgressBar(PBType::PBR, spec.x, spec.y, 1.f);
                warmupElements[i] = NULL;
                cooldownElements[i] = NULL;
                break;
            case wvProgressBarBlue:
                cooldownElements[i] = new UIProgressBar(PBType::PBB, spec.x, spec.y, 1.f);
                workoutElements[i] = NULL;
                warmupElements[i] = NULL;
                break;
            default:
                warmupElements[i] = NULL;
                cooldownElements [i] = NULL;
                warmupElements[i] = NULL;
                break;
        }
    }
    m_stateMap[UIWarmup] = warmupElements;
    m_stateMap[UIWorkout] = workoutElements;
    m_stateMap[UICooldown] = cooldownElements;
}

/*!****************************************************************************
 @Function		LoadTextures
 @Output		pErrorStr		Pointer to the string returned on error
 @Description	Loads the object's textures into graphics memory
******************************************************************************/
bool
UIWorkoutView::LoadTextures(CPVRTString* const pErrorString)
{
    for ( int i = UIWarmup; i <= UICooldown; i ++ ) {
        UIWorkoutStage iState = static_cast<UIWorkoutStage>(i);
        if (!m_stateMap.Exists(iState)) {
            continue;
        }
        UIElement** elementArray = m_stateMap[iState];
        if (elementArray == NULL) {
            continue;
        }
        for ( int i = 0; i < c_numWVLayoutSpecs; i ++ ) {
            if (elementArray[i] == NULL) {
                continue;
            }
            
            if (!elementArray[i]->LoadTextures(pErrorString)) {
                fprintf(stderr, "UIWorkoutView texture %d load failed\n", i);
                return false;
            } else {
                // fprintf(stderr, "UIWorkoutView texture loaded\n");
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
UIWorkoutView::BuildVertices()
{
    for ( int i = UIWarmup; i <= UICooldown; i ++ ) {
        UIWorkoutStage iState = static_cast<UIWorkoutStage>(i);
        if (!m_stateMap.Exists(iState)) {
            continue;
        }
        UIElement** elementArray = m_stateMap[iState];
        if (elementArray == NULL) {
            continue;
        }
        for ( int i = 0; i < c_numWVLayoutSpecs; i ++ ) {
            if (elementArray[i] == NULL) {
                continue;
            }
            elementArray[i]->BuildVertices();
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
UIWorkoutView::Render(GLuint uiMVPMatrixLoc, UIPrinter* printer)
{
	if (!m_stateMap.Exists(m_state)) {
        return false;
    }
    UIElement** elementArray = m_stateMap[m_state];
    if (elementArray == NULL) {
        return true;
    }
    for ( int i = 0; i < c_numWVLayoutSpecs; i ++ ) {
        if (elementArray[i] == NULL) {
			//fprintf(stderr, "Null UIWorkoutView element index %d\n", i);
            continue;
        } else if (!elementArray[i]->Render(uiMVPMatrixLoc, printer)) {
            fprintf(stderr, "UIWorkoutView element %d render failed\n", i);
			return false;
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
UIWorkoutView::Update(UIMessage updateMessage)
{
	m_state = updateMessage.ReadWorkoutStage();

    UIElement** elementArray = m_stateMap[m_state];
	UIMessage delegateMessage;
	if (elementArray != NULL) {
		for (int i = 0 ; i < c_numWVLayoutSpecs; i ++) {
			if (elementArray[i] == NULL) {
				continue;
			}
            WVElement type = c_wvLayoutSpecs[i].type;
			if (type == wvProgressBarRed || type == wvProgressBarGreen 
                    || type == wvProgressBarBlue) {
                //fprintf(stderr, "Valid message received in UIWorkoutView\n");
				delegateMessage = updateMessage.Delegate(UIStageProgress);
				elementArray[i]->Update(delegateMessage);
			}
		}
	}
}

/*!****************************************************************************
 @Function		Hide
 @Description	Makes object hidden
******************************************************************************/
void
UIWorkoutView::Hide()
{
    m_hidden = true;
}

/*!****************************************************************************
 @Function		Show
 @Description	Makes object visibile
******************************************************************************/
void
UIWorkoutView::Show()
{
    m_hidden = false;
}

/*!****************************************************************************
 @Function		SetState
 @Input			state		New UI state
 @Description	Sets the m_state instance variable and sets the corresponding
				elements to visible.
******************************************************************************/
void
UIWorkoutView::Delete()
{
	for ( int i = UIWarmup; i <= UICooldown; i ++ ) {
        UIWorkoutStage iState = static_cast<UIWorkoutStage>(i);
        if (!m_stateMap.Exists(iState)) {
            continue;
        }
        UIElement** elementArray = m_stateMap[iState];
        if (elementArray == NULL) {
            continue;
        }
        for ( int i = 0; i < c_numWVLayoutSpecs; i ++ ) {
            if (elementArray[i] == NULL) {
                continue;
            }
            elementArray[i]->Delete();
			delete elementArray[i];
			elementArray[i] = NULL;
        }
		delete elementArray;
		elementArray = NULL;
    }
}