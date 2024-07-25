#ifndef __BANNER_VIEW_H_
#define __BANNER_VIEW_H_

#include <vector>
#include "Print2D.h"

using std::vector;

class BannerView {
public:
    // Indicates whether BannerView should be centered within screen or pinned to top
    enum Position { TOP, CENTER };

    // BannerView Constructor
    BannerView(vector<char*> bannerLinesText, Position position, int spacing, float scale, unsigned int color, CPVRTPrint3D *print3D, bool isScreenRotated);

    // BannerView Destructor
    ~BannerView();

    // Renders banner view on screen. 
    // If provided scale is too large, text will scale to fit screen width.
    void render();
private:
    struct Coordinate {
        float x, y;
    };

    const float TOP_PADDING_PIXELS = 60;
    const float MAX_HORIZONTAL_PADDING_PIXELS = 15;

    vector<char*> bannerLinesText;
    Position position;
    int spacing;
    float scale;
    unsigned int color;
    CPVRTPrint3D *print3D;
    Print2D *print2D;

    void renderAtTopPosition();
    void renderAtCenterPosition();
    Coordinate topPositionAbsoluteOriginCoordinate();
    Coordinate centerPositionRelativeCenterCoordinate();
    void scaleToFitIfNeeded();
    float maxTextWidth();
};

#endif
