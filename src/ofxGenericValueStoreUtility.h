//
//  ofxGenericValueStoreUtility.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 6/8/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericValueStore.h"

ofPtr< ofxGenericValueStore > ofxGGetWithCaseInsensitiveKey( ofPtr< ofxGenericValueStore > from, string key );
int ofxGToInt( ofPtr< ofxGenericValueStore > from );
ofColor toofColor( ofPtr< ofxGenericValueStore > from );
ofRectangle toofRectangle( ofPtr< ofxGenericValueStore > from );
ofRectangle toofRectangleLowercase( ofPtr< ofxGenericValueStore > from );
ofPoint toofPoint( ofPtr< ofxGenericValueStore > from );
ofPoint toofPointSize( ofPtr< ofxGenericValueStore > from );
ofxGenericTextHorizontalAlignment toofxGenericTextHorizontalAlignment( ofPtr< ofxGenericValueStore > from );

std::vector< unsigned int > toVectorOfUnsignedInts( ofPtr< ofxGenericValueStore > from );

std::vector< string > toVectorOfStrings( ofPtr< ofxGenericValueStore > from );
