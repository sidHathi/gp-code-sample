/******************************************************************************

 @File          UIMessage.cpp

 @Title         UIMessage

 @Author        Siddharth Hathi

 @Description   Implements the UIMessage object class defined in UIMessage.h

******************************************************************************/

#include "UIMessage.h"

/*!****************************************************************************
 @Function		Write
 @Input			key		Enum key for text data
 @Input			value	Text being stored
 @Description	Adds textual data to the corresponding data map
******************************************************************************/
void
UIMessage::Write(UITextType key, char* value)
{
	m_sValueMap[key] = value;
}

/*!****************************************************************************
 @Function		Write
 @Input			key		Enum key for float data
 @Input			value	Float being stored
 @Description	Adds float data to the corresponding data map
******************************************************************************/
void
UIMessage::Write(UIFloat key, float value)
{
	m_fValueMap[key] = value;
}

/*!****************************************************************************
 @Function		Write
 @Input			key		Enum key for boolean data
 @Input			value	Bool being stored
 @Description	Adds boolean data to the corresponding data map
******************************************************************************/
void
UIMessage::Write(UIBool key, bool value)
{
	m_bValueMap[key] = value;
}

/*!****************************************************************************
 @Function		Write
 @Input			state	The UI's current display state
 @Description	Stores a new UIState in the message
******************************************************************************/
void
UIMessage::Write(UIState state)
{
	m_UIState = state;
}

/*!****************************************************************************
 @Function		Write
 @Input			action	A new action enum
 @Description	Stores a new UIAction in the message
******************************************************************************/
void
UIMessage::Write(UIAction action)
{
	m_UIAction = action;
}

/*!****************************************************************************
 @Function		Write
 @Input			newStage	A new workout stage
 @Description	Stores the new workout stage in the UIMessage
******************************************************************************/
void
UIMessage::Write(UIWorkoutStage newStage)
{
	m_WorkoutStage = newStage;
}

void
UIMessage::FreeStrings()
{
	for ( int i = UISpeedMPM; i <= UINone; i ++ ) {
		UITextType type = static_cast<UITextType>(i);
		if (m_sValueMap[type] != NULL) {
			delete m_sValueMap[type];
			m_sValueMap[type] = NULL;
		}
	}
}

/*!****************************************************************************
 @Function		Read
 @Input			key		Enum key for float data
 @Description	Retreives float data corresponding to given key
******************************************************************************/
float
UIMessage::Read(UIFloat key)
{
	if (m_fValueMap.Exists(key)) {
		return m_fValueMap[key];
	} else {
		return 0;
	}
}

/*!****************************************************************************
 @Function		Read
 @Input			key		Enum key for text data
 @Description	Retreives text data corresponding to given key
******************************************************************************/
char*
UIMessage::Read(UITextType key)
{
	if (m_sValueMap.Exists(key)) {
		return m_sValueMap[key];
	} else {
		return 0;
	}
}

/*!****************************************************************************
 @Function		Read
 @Input			key		Enum key for bool data
 @Description	Retreives bool data corresponding to given key
******************************************************************************/
bool
UIMessage::Read(UIBool key)
{
	if (m_bValueMap.Exists(key)) {
		return m_bValueMap[key];
	} else {
		return false;
	}
}

/*!****************************************************************************
 @Function		ReadState
 @Return		UIState		The stored UIState
 @Description	Retreives the UIState stored in the message
******************************************************************************/
UIState
UIMessage::ReadState()
{
	return m_UIState;
}

/*!****************************************************************************
 @Function		ReadAction
 @Return		UIAction	The stored UIAction
 @Description	Retreives the UIAciton stored in the message
******************************************************************************/
UIAction
UIMessage::ReadAction()
{
	return m_UIAction;
}

/*!****************************************************************************
 @Function		ReadWorkoutStage
 @Return		UIWorkoutStage	The stored UIWorkoutStage
 @Description	Retreives the UIWorkoutStage stored in the message
******************************************************************************/
UIWorkoutStage
UIMessage::ReadWorkoutStage()
{
	return m_WorkoutStage;
}

/*!****************************************************************************
 @Function		GetTotalRacers
 @Description	Returns the total number of racers as stored in the UIMessage
******************************************************************************/
int
UIMessage::GetTotalRacers()
{
	return m_totalRacers;
}

/*!****************************************************************************
 @Function		SetTotalRacers
 @Input			total	Total number of racers
 @Description	Sets a new value for the total number of racers in the message
******************************************************************************/
void
UIMessage::SetTotalRacers(int total)
{
	m_totalRacers = total;
}

/*!****************************************************************************
 @Function		Delegate
 @Return		UIMessage	New message containing the specified data
 @Description	Makes a copy of the current UIMessage that only contains the data
				specified by the UIFloat key
******************************************************************************/
UIMessage
UIMessage::Delegate(UIFloat key)
{
	UIMessage delegate = UIMessage();
	if (m_fValueMap.Exists(key)) {
		delegate.Write(key, m_fValueMap[key]);
	}
	delegate.SetTotalRacers(m_totalRacers);
	delegate.Write(m_UIAction);
	delegate.Write(m_UIState);
	delegate.Write(m_WorkoutStage);
	return delegate;
}

/*!****************************************************************************
 @Function		Delegate
 @Return		UIMessage	New message containing the specified data
 @Description	Makes a copy of the current UIMessage that only contains the data
				specified by the UITextType key
******************************************************************************/
UIMessage
UIMessage::Delegate(UITextType key)
{
	UIMessage delegate = UIMessage();
	if (m_sValueMap.Exists(key)) {
		delegate.Write(key, m_sValueMap[key]);
	}
	delegate.SetTotalRacers(m_totalRacers);
	delegate.Write(m_UIAction);
	delegate.Write(m_UIState);
	delegate.Write(m_WorkoutStage);
	return delegate;
}

/*!****************************************************************************
 @Function		Delegate
 @Return		UIMessage	New message containing the specified data
 @Description	Makes a copy of the current UIMessage that only contains the data
				specified by the UIBool key
******************************************************************************/
UIMessage
UIMessage::Delegate(UIBool key)
{
	UIMessage delegate = UIMessage();
	if (m_bValueMap.Exists(key)) {
		delegate.Write(key, m_bValueMap[key]);
	}
	delegate.SetTotalRacers(m_totalRacers);
	delegate.Write(m_UIAction);
	delegate.Write(m_UIState);
	delegate.Write(m_WorkoutStage);
	return delegate;
}