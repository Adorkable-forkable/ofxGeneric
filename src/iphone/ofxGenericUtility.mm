//
//  ofxGenericUtility.mm
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/30/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericUtility.h"



NSString* pathToBundle( NSString* resourceFileName )
{
    NSString* bundlePath = [ [ NSBundle mainBundle ] resourcePath ];
    if ( resourceFileName != nil )
    {
        return [ NSString stringWithFormat:@"%@/%@", bundlePath, resourceFileName ];
    }
    return [ NSString stringWithFormat:@"%@/", bundlePath ];
}

CGFloat getWindowScale()
{
    if ( [ [ UIScreen mainScreen ] respondsToSelector:@selector( scale ) ] ) 
    {
        return [ [ UIScreen mainScreen ] scale ];
    }
    return 1;
}

CGRect ofRectangleToCGRect( const ofRectangle& from )
{
    return CGRectMake( from.x, from.y, from.width, from.height );
}

ofRectangle CGRectToofRectangle( const CGRect& from )
{
    return ofRectangle( from.origin.x, from.origin.y, from.size.width, from.size.height );
}

UITextAlignment ofxGenericTextHorizontalAlignmentToiOS( ofxGenericTextHorizontalAlignment from )
{
    switch ( from )
    {
        case ofxGenericTextHorizontalAlignmentLeft:
            return UITextAlignmentLeft;
        case ofxGenericTextHorizontalAlignmentCenter:
            return UITextAlignmentCenter;
        case ofxGenericTextHorizontalAlignmentRight:
            return UITextAlignmentRight;
    }
    return UITextAlignmentLeft;
}

ofxGenericTextHorizontalAlignment iOSToofxGenericTextHorizontalAlignment( UITextAlignment from )
{
    switch ( from )
    {
        case UITextAlignmentLeft:
            return ofxGenericTextHorizontalAlignmentLeft;
        case UITextAlignmentCenter:
            return ofxGenericTextHorizontalAlignmentCenter;
        case UITextAlignmentRight:
            return ofxGenericTextHorizontalAlignmentRight;
    }
    return ofxGenericTextHorizontalAlignmentLeft;
}

UILineBreakMode ofxGenericTextLinebreakModeToiOS( ofxGenericTextLinebreakMode from )
{
    switch ( from )
    {
        case ofxGenericTextLinebreakModeWordWrap:
            return UILineBreakModeWordWrap;
        case ofxGenericTextLinebreakModeCharacterWrap:
            return UILineBreakModeCharacterWrap;
        case ofxGenericTextLinebreakModeClip:
            return UILineBreakModeClip;
        case ofxGenericTextLinebreakModeHeadTruncation:
            return UILineBreakModeHeadTruncation;
        case ofxGenericTextLinebreakModeTailTruncation:
            return UILineBreakModeTailTruncation;
        case ofxGenericTextLinebreakModeMiddleTruncation:
            return UILineBreakModeMiddleTruncation;
    }
    return UILineBreakModeWordWrap;
}

ofxGenericTextLinebreakMode iOSToofxGenericTextLinebreakMode( UILineBreakMode from )
{
    switch ( from )
    {
        case UILineBreakModeWordWrap:
            return ofxGenericTextLinebreakModeWordWrap;
        case UILineBreakModeCharacterWrap:
            return ofxGenericTextLinebreakModeCharacterWrap;
        case UILineBreakModeClip:
            return ofxGenericTextLinebreakModeClip;
        case UILineBreakModeHeadTruncation:
            return ofxGenericTextLinebreakModeHeadTruncation;
        case UILineBreakModeTailTruncation:
            return ofxGenericTextLinebreakModeTailTruncation;
        case UILineBreakModeMiddleTruncation:
            return ofxGenericTextLinebreakModeMiddleTruncation;
    }
    return ofxGenericTextLinebreakModeWordWrap;
}

UITableViewCellSeparatorStyle ofxGenericTableViewSeparatorStyleToiOS( ofxGenericTableViewSeparatorStyle from )
{
    switch ( from )
    {
        case ofxGenericTableViewSeparatorStyleNone:
            return UITableViewCellSeparatorStyleNone;
        case ofxGenericTableViewSeparatorStyleSingleLine:
            return UITableViewCellSeparatorStyleSingleLine;
        case ofxGenericTableViewSeparatorStyleSingleLineEtched:
            return UITableViewCellSeparatorStyleSingleLineEtched;
    }
    return UITableViewCellSeparatorStyleSingleLine;    
}

UIReturnKeyType ofxGenericKeyboardReturnKeyToiOS( ofxGenericKeyboardReturnKey from )
{
    switch( from )
    {
        case ofxGenericKeyboardReturnKeyDefault:
            return UIReturnKeyDefault;
        case ofxGenericKeyboardReturnKeyGo:
            return UIReturnKeyGo;
            //    ofxGenericKeyboardReturnKeyGoogle, //UIReturnKeyGoogle,
            //    ofxGenericKeyboardReturnKeyJoin, UIReturnKeyJoin,
        case ofxGenericKeyboardReturnKeyNext:
            return UIReturnKeyNext;
            //    ofxGenericKeyboardReturnKeyRoute, //UIReturnKeyRoute,
        case ofxGenericKeyboardReturnKeySearch:
            return UIReturnKeySearch;
        case ofxGenericKeyboardReturnKeySend:
            return UIReturnKeySend;
            //    ofxGenericKeyboardReturnKeyYahoo, UIReturnKeyYahoo,
        case ofxGenericKeyboardReturnKeyDone:
            return UIReturnKeyDone;
            //ofxGenericKeyboardReturnKeyEmergencyCall //UIReturnKeyEmergencyCall,
    }
    return UIReturnKeyDefault;
}

UIKeyboardType ofxGenericKeyboardTypeToiOS( ofxGenericKeyboardType from )
{
    switch ( from )
    {
        case ofxGenericKeyboardTypeDefault:
            return UIKeyboardTypeDefault;                // Default type for the current input method.
        case ofxGenericKeyboardTypeASCIICapable:
            return UIKeyboardTypeASCIICapable;           // Displays a keyboard which can enter ASCII characters, non-ASCII keyboards remain active
        case ofxGenericKeyboardTypeNumbersAndPunctuation:
            return UIKeyboardTypeNumbersAndPunctuation;  // Numbers and assorted punctuation.
        case ofxGenericKeyboardTypeURL:
            return UIKeyboardTypeURL;                    // A type optimized for URL entry (shows . / .com prominently).
        case ofxGenericKeyboardTypeNumberPad:
            return UIKeyboardTypeNumberPad;              // A number pad (0-9). Suitable for PIN entry.
        case ofxGenericKeyboardTypePhonePad:
            return UIKeyboardTypePhonePad;               // A phone pad (1-9, *, 0, #, with letters under the numbers).
        case ofxGenericKeyboardTypeNamePhonePad:
            return UIKeyboardTypeNamePhonePad;           // A type optimized for entering a person's name or phone number.
        case ofxGenericKeyboardTypeEmailAddress:
            return UIKeyboardTypeEmailAddress;           // A type optimized for multiple email address entry (shows space @ . prominently).
#if __IPHONE_4_1 <= __IPHONE_OS_VERSION_MAX_ALLOWED
        case ofxGenericKeyboardTypeDecimalPad:
            return UIKeyboardTypeDecimalPad;             // A number pad with a decimal point.
#endif
#if __IPHONE_5_0 <= __IPHONE_OS_VERSION_MAX_ALLOWED
        case ofxGenericKeyboardTypeTwitter:
            return UIKeyboardTypeTwitter;                // A type optimized for twitter text entry (easy access to @ #)
#endif
    }
}

UITextAutocapitalizationType ofxGenericTextAutoCapitalizationToiOS( ofxGenericTextAutoCapitalization from )
{
    switch ( from )
    {
        case ofxGenericTextAutoCapitalizationNone:
            return UITextAutocapitalizationTypeNone;
        case ofxGenericTextAutoCapitalizationWords:
            return UITextAutocapitalizationTypeWords;
        case ofxGenericTextAutoCapitalizationSentences:
            return UITextAutocapitalizationTypeSentences;
        case ofxGenericTextAutoCapitalizationAllCharacters:
            return UITextAutocapitalizationTypeAllCharacters;
    }
    return UITextAutocapitalizationTypeNone;
}

UIDeviceOrientation ofOrientationToiOS( ofOrientation from )
{
    switch ( from ) 
    {
        case OF_ORIENTATION_UNKNOWN:
            return UIDeviceOrientationUnknown;
        case OF_ORIENTATION_DEFAULT:
            return UIDeviceOrientationPortrait;
        case OF_ORIENTATION_180:
            return UIDeviceOrientationPortraitUpsideDown;
        case OF_ORIENTATION_90_RIGHT:
            return UIDeviceOrientationLandscapeLeft;
        case OF_ORIENTATION_90_LEFT:
            return UIDeviceOrientationLandscapeRight;
            // TODO:
//        case ofxGenericOrientationFaceUp:
//            return UIDeviceOrientationFaceUp;
//        case ofxGenericOrientationFaceDown:
//            return UIDeviceOrientationFaceDown;
    }
    return UIDeviceOrientationUnknown;
}

ofOrientation iOSToofOrientation( UIDeviceOrientation from )
{
    switch ( from ) 
    {
        case UIDeviceOrientationUnknown:
            return OF_ORIENTATION_UNKNOWN;
        case UIDeviceOrientationPortrait:
            return OF_ORIENTATION_DEFAULT;
        case UIDeviceOrientationPortraitUpsideDown:
            return OF_ORIENTATION_180;
        case UIDeviceOrientationLandscapeLeft:
            return OF_ORIENTATION_90_RIGHT;
        case UIDeviceOrientationLandscapeRight:
            return OF_ORIENTATION_90_LEFT;
// TODO:
//        case UIDeviceOrientationFaceUp:
//            return ofxGenericOrientationFaceUp;
//        case UIDeviceOrientationFaceDown:
//            return ofxGenericOrientationFaceDown;
    }
    return OF_ORIENTATION_UNKNOWN;
}

UIViewAnimationCurve ofxViewAnimationCurveToiOS( ofxGenericViewAnimationCurve from )
{
    switch( from )
    {
        case ofxViewAnimationCurveEaseInOut:
            return UIViewAnimationCurveEaseInOut;
        case ofxViewAnimationCurveEaseIn:
            return UIViewAnimationCurveEaseIn;
        case ofxViewAnimationCurveEaseOut:
            return UIViewAnimationCurveEaseOut;
        case ofxViewAnimationCurveLinear:
            return UIViewAnimationCurveLinear;
    }
    return UIViewAnimationCurveLinear;
}

UIViewAnimationTransition ofxGenericViewAnimationTransitionToiOS( ofxGenericViewAnimationTransition from )
{
    switch ( from ) 
    {
        case ofxGenericViewAnimationTransitionNone:
            return UIViewAnimationTransitionNone;
        case ofxGenericViewAnimationTransitionFlipFromLeft:
            return UIViewAnimationTransitionFlipFromLeft;
        case ofxGenericViewAnimationTransitionFlipFromRight:
            return UIViewAnimationTransitionFlipFromRight;
        case ofxGenericViewAnimationTransitionCurlUp:
            return UIViewAnimationTransitionCurlUp;
        case ofxGenericViewAnimationTransitionCurlDown:
            return UIViewAnimationTransitionCurlDown;
    }
    return UIViewAnimationTransitionNone;
}

UIViewAutoresizing ofxGenericViewAutoresizingToiOS( ofxGenericViewAutoresizing from )
{
    UIViewAutoresizing to = 0;
    if ( from & ofxGenericViewAutoresizingLeftMargin )
    {
        to |= UIViewAutoresizingFlexibleLeftMargin;
    }
    if ( from & ofxGenericViewAutoresizingFlexibleWidth )
    {
        to |= UIViewAutoresizingFlexibleWidth;
    }
    if ( from & ofxGenericViewAutoresizingRightMargin )
    {
        to |= UIViewAutoresizingFlexibleRightMargin;
    }
    if ( from & ofxGenericViewAutoresizingTopMargin )
    {
        to |= UIViewAutoresizingFlexibleTopMargin;
    }
    if ( from & ofxGenericViewAutoresizingFlexibleHeight )
    {
        to |= UIViewAutoresizingFlexibleHeight;
    }
    if ( from & ofxGenericViewAutoresizingBottomMargin )
    {
        to |= UIViewAutoresizingFlexibleBottomMargin;
    }
    return to;
}

ofxGenericViewAutoresizing iOSToofxGenericViewAutoresizing( UIViewAutoresizing from )
{
    int to = 0;
    if ( from & UIViewAutoresizingFlexibleLeftMargin )
    {
        to |= ofxGenericViewAutoresizingLeftMargin;
    }
    if ( from & UIViewAutoresizingFlexibleWidth )
    {
        to |= ofxGenericViewAutoresizingFlexibleWidth;
    }
    if ( from & UIViewAutoresizingFlexibleRightMargin )
    {
        to |= ofxGenericViewAutoresizingRightMargin;
    }
    if ( from & UIViewAutoresizingFlexibleTopMargin )
    {
        to |= ofxGenericViewAutoresizingTopMargin;
    }
    if ( from & UIViewAutoresizingFlexibleHeight )
    {
        to |= ofxGenericViewAutoresizingFlexibleHeight;
    }
    if ( from & UIViewAutoresizingFlexibleBottomMargin )
    {
        to |= ofxGenericViewAutoresizingBottomMargin;
    }
    return ( ofxGenericViewAutoresizing )to;
}
