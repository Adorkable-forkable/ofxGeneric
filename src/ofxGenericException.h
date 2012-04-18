//
//  ofxGenericException.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/18/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofMain.h"

#include <exception>
#if TARGET_OS_IPHONE
#import <Foundation/Foundation.h>
#endif

class ofxGenericException : public std::exception
{
public:
#if TARGET_OS_IPHONE
    ofxGenericException( NSException* translate ) throw();
#endif
    virtual ~ofxGenericException() throw();

    virtual const char* what() const throw();    
    
protected:
    string _what;
};