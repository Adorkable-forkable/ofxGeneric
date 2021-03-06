//
//  ofxGenericActivityView.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 3/9/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

class ofxGenericActivityView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericActivityView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ) );
#if TARGET_OS_IPHONE
    operator UIActivityIndicatorView*();
#endif
    
    void startAnimating();
    void stopAnimating();
    bool isAnimating();
    
    void setHidesWhenStoppedAnimating( bool hide );
    bool hidesWhenStoppedAnimating();
    
    void setColor( const ofColor& color );
    ofColor getColor();

    void setIndicatorViewStyle( ofxGenericActivityViewStyle style );
    ofxGenericActivityViewStyle getIndicatorViewStyle();
protected:
    virtual NativeView createNativeView( const ofRectangle& frame );
};