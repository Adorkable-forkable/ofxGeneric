//
//  ofxGenericValueStoreUtility.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 6/8/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericValueStore.h"

ofColor toofColor( ofPtr< ofxGenericValueStore > from );

std::vector< unsigned int > toVectorOfUnsignedInts( ofPtr< ofxGenericValueStore > from );