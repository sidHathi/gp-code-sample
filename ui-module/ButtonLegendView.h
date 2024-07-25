#ifndef __BUTTON_LEGEND_VIEW_H_
#define __BUTTON_LEGEND_VIEW_H_

#include "Print2D.h"

class ButtonLegendView {
public:
    // ButtonLegendView Constructor
    ButtonLegendView(unsigned enabledNavigationButtonColor, unsigned enabledOkButtonColor, unsigned disabledButtonColor, int spacing, CPVRTPrint3D *print3D, bool isScreenRotated);

    // ButtonLegendView Destructor
    ~ButtonLegendView();

    // Renders button legend on screen
    void render();

    // Sets the button at the given index with either its
    // selected or deselected color
    void setButtonIsEnabled(int buttonIndex, bool enabled);
private:
    const char *UP_BUTTON = "~";
    const char *DOWN_BUTTON = "`";
    const char *OK_BUTTON = "\\";
    const float SCALE = 0.7f;
    const float X_RELATIVE_CENTER_COORDINATE = 90.0f;
    const float Y_RELATIVE_CENTER_COORDINATE = 82.0f;

    vector<Print2D::AttributedText> legendItemsAttributedText;
    unsigned enabledNavigationButtonColor;
    unsigned enabledOkButtonColor;
    unsigned disabledButtonColor;
    int spacing;
    Print2D *print2D;

    char* stringRepresentationOfButton(int buttonIndex);
    unsigned int selectedColorOfButton(int buttonIndex);
};

#endif
