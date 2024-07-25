#include "BannerView.h"

/**
* DOES: Creates a new instance of BannerView class
* PARAMS: bannerLinesText - Lines of text to display. First element
*         is displayed at the top, last element at the bottom.
*         position - Whether banner should be centered or pinned to top
*         spacing - Spacing between menu items
*         scale - Size of menu item text
*         color - Color of text
*         print3D - Pointer to an already set up CPVRTPrint3D object
* RETURNS: BannerView instance
* THROWS: None
*/
BannerView::BannerView(vector<char*> bannerLinesText, Position position, int spacing, float scale, unsigned int color, CPVRTPrint3D *print3D, bool isScreenRotated) {
    this->bannerLinesText = bannerLinesText;
    this->position = position;
    this->spacing = spacing;
    this->scale = scale;
    this->color = color;
    this->print3D = print3D;
    this->print2D = new Print2D(print3D, isScreenRotated);
}

/**
* DOES: Frees memory and destroys BannerView instance
* PARAMS: None
* RETURNS: None
* THROWS: None
*/
BannerView::~BannerView() {
    delete print2D;
}

/**
* DOES: Renders banner on screen.
*       If scale is too large,text will scale to fit screen width
* PARAMS: None
* RETURNS: None
* THROWS: None
*/
void BannerView::render() {
    scaleToFitIfNeeded();

    switch (position) {
    case TOP:
        renderAtTopPosition();
        break;
    case CENTER:
        renderAtCenterPosition();
        break;
    }
}

// ***** HELPER FUNCTIONS *****

/**
* DOES: Renders banner pinned to top of screen
* PARAMS: None
* RETURNS: None
* THROWS: None
*/
void BannerView::renderAtTopPosition() {
    Coordinate absoluteOriginCoordinate = topPositionAbsoluteOriginCoordinate();
    print2D->renderVerticalMenuAbsoluteOriginAt(absoluteOriginCoordinate.x, absoluteOriginCoordinate.y, scale, color, spacing, bannerLinesText);
}

/**
* DOES: Calculates absolute origin coordinate of banner for its top position
* PARAMS: None
* RETURNS: Absolute origin coordinate of top positioned banner
* THROWS: None
*/
BannerView::Coordinate BannerView::topPositionAbsoluteOriginCoordinate() {
    float screenWidth = print2D->getScreenWidth();
    float bannerWidth = maxTextWidth();

    Coordinate absoluteOriginCoordinate;
    absoluteOriginCoordinate.x = (screenWidth - bannerWidth) / 2;
    absoluteOriginCoordinate.y = TOP_PADDING_PIXELS;

    return absoluteOriginCoordinate;
}

/**
* DOES: Renders banner centered within screen
* PARAMS: None
* RETURNS: None
* THROWS: None
*/
void BannerView::renderAtCenterPosition() {
    Coordinate relativeCenterCoordinate = centerPositionRelativeCenterCoordinate();
    print2D->renderVerticalMenuCenteredAt(relativeCenterCoordinate.x, relativeCenterCoordinate.y, scale, color, spacing, bannerLinesText);
}

/**
* DOES: Calculates relative center coordinate of banner for its center position
* PARAMS: None
* RETURNS: Relative center coordinate of center positioned banner
* THROWS: None
*/
BannerView::Coordinate BannerView::centerPositionRelativeCenterCoordinate() {
    Coordinate relativeCenterCoordinate;
    relativeCenterCoordinate.x = 50.0f;
    relativeCenterCoordinate.y = 50.0f;
    return relativeCenterCoordinate;
}

/**
* DOES: If the scale factor provided when initializing BannerView instance is
*       too large, recalculate the scale so the banner fits within the screen width
* PARAMS: None
* RETURNS: None
* THROWS: None
*/
void BannerView::scaleToFitIfNeeded() {
    float screenWidth = print2D->getScreenWidth();
    float bannerWidth = maxTextWidth();
    float safeAreaWidth = screenWidth - 2 * MAX_HORIZONTAL_PADDING_PIXELS;
    if (bannerWidth > safeAreaWidth) {
        float oversizeFactor = safeAreaWidth / bannerWidth;
        this->scale = this->scale * oversizeFactor;
    }
}

/**
* DOES: Reports the width of the longest banner line
* PARAMS: None
* RETURNS: The width of the longest banner line found
* THROWS: None
*/
float BannerView::maxTextWidth() {
    float maxWidth = 0;
    int size = bannerLinesText.size();
    for (int ii = 0; ii < size; ii++) {
        float textWidth;
        float textHeight;
        print3D->MeasureText(&textWidth, &textHeight, scale, bannerLinesText[ii]);

        if (textWidth > maxWidth) { maxWidth = textWidth; }
    }

    return maxWidth;
}