/******************************************************************************
 @File          UIMessage.h
 @Title         UIMessage Header
 @Author        Siddharth Hathi
 @Description   Header file for the UIMessage object class. Defines UIMessgae
******************************************************************************/

#include "OGLES2Tools.h"

#ifndef _UIMESSAGE_H
#define _UIMESSAGE_H

// Enum identifiers for messages containing float values
enum UIFloat {
	UIStageProgress,
	UIBrightnessProg,
	UIVolumeProg
};


// Enum identifiers for messages containing text values
enum UITextType {
	UISpeedMPM,
	UIRank,
	UIEnergyKJ,
	UICalories,
	UITimeS,
	UIDistanceM,
	UIBrightness,
	UIVolume,
	UIAvatarDistance,
	UIBPM,
	UIClock,
	UINone
};

// Enum identifiers for messages containing bool values
enum UIBool {
	UIHidden,
	UIButtonActive,
	UIResumeButtonActive,
	UIVolumeButtonActive,
	UIBrightButtonActive,
	UIEndButtonActive,
	UIMileMarker,
	UIBadge1,
	UIBadge2,
	UIBadge3,
	UIMultiProg,
	UIBrightnessSelected,
	UISpeedSelected,
	UIVolumeSelected,
	UIFlash
};

// Enum identifiers for the UI's display states
enum UIState {
	UIMain,
	UIInfo,
	UIBPMView,
	UIPause,
	UISummary,
	UIMenuBrightness,
	UIMenuVolume,
	UIMenuSpeed,
	UIMenuMain,
	noUI
};

// Enum identifiers for the peloton workout stages
enum UIWorkoutStage {
	UIWarmup, 
	UIWorkout,
	UICooldown
};

// Enum identifiers for action items
enum UIAction {
	UINoAction,
	UIToggleActiveButton
};

/*!****************************************************************************
 @class UIMessage
 Object class. A UIMessage object stores the information needed to update the UI
 based on the current frame data. UIMessages are intended to be passed from 
 element to element through the UIElement's Update function.
******************************************************************************/
class UIMessage
{
	protected:
		// Instance variables

		// Maps from enum identifiers to message data
		CPVRTMap<UIFloat, float> m_fValueMap;
		CPVRTMap<UITextType, char*> m_sValueMap;
		CPVRTMap<UIBool, bool> m_bValueMap;

		// The current UI state
		UIState m_UIState;

		// Any UI action items
		UIAction m_UIAction;

		// The current peloton workout stage
		UIWorkoutStage m_WorkoutStage;

		// the total number of racers in the workout
		int m_totalRacers;

	public:
		// Exported functions
		void Write(UIFloat key, float value);
		void Write(UITextType key, char* value);
		void Write(UIBool key, bool value);
		void Write(UIState state);
		void Write(UIAction action);
		void Write(UIWorkoutStage stage);
		void SetTotalRacers(int total);
		void FreeStrings();
		float Read(UIFloat key);
		char* Read(UITextType key);
		bool Read(UIBool key);
		UIState ReadState();
		UIAction ReadAction();
		UIWorkoutStage ReadWorkoutStage();
		int GetTotalRacers();
		UIMessage Delegate(UIFloat key);
		UIMessage Delegate(UITextType key);
		UIMessage Delegate(UIBool key);
};

#endif