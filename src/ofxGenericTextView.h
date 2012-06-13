//
//  ofxGenericTextView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"
#include "ofxGenericConstants.h"

class ofxGenericFont;

class ofxGenericTextView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericTextView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ) );
    
    void setText( string newText );
    string getText();
    operator string();
        
    void setTextAlignment( ofxGenericTextHorizontalAlignment alignment );
    ofxGenericTextHorizontalAlignment getTextAlignment();
    
    void setTextColor ( const ofColor& setColor );
    ofColor getTextColor ();
    
    void setLineBreakMode ( ofxGenericTextLinebreakMode mode );
    ofxGenericTextLinebreakMode getLineBreakMode();
    
    void setNumberOfLines ( int n );
    int getNumberOfLines ();
    
    int getMinimumFontSize();
    void setMinimumFontSize( int s );
    
    void setFont( string name, float size );
    void setFont( ofPtr< ofxGenericFont > font ); 
    float getFontSize();
    string getFontName();
    
    void setAutosizeFontToFitText( bool autosize );
    bool getAutosizeFontToFitText();
#if TARGET_OS_IPHONE
    void autosizeFontForMultiline();
#endif

#if TARGET_OS_IPHONE
    operator UILabel*();
#endif
    
protected:
    ofxGenericTextView() {};
    
    virtual NativeView createNativeView( const ofRectangle& frame );
};
