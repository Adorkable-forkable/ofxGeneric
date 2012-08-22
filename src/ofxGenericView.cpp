//
//  ofxGenericView.cpp
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericView.h"
#include "ofxGenericUtility.h"
#include "ofxGenericApp.h"

#if TARGET_OS_IPHONE

@interface ofxUIGenericViewControllerForwarder( Gesture )

-( void )gesturePerformedSwipe:( UIGestureRecognizer* )recognizer;
-( void )gesturePerformedTap:( UIGestureRecognizer* )recognizer;
-( void )gesturePerformedHold:( UIGestureRecognizer* )recognizer;
-( void )gesturePerformedPan:( UIGestureRecognizer* )recognizer;

@end

@interface ofxGenericViewAnimationForwarder : NSObject
{
    ofPtrWeak< ofxGenericViewDelegate > _delegate;
}
- ( id )initWithDelegate:( ofPtrWeak< ofxGenericViewDelegate > )delegate;
- (void)animationWillStart:( NSString* )animationID finished:( NSNumber* )finished context:( void* )context;
- (void)animationDidStop:( NSString* )animationID finished:( NSNumber* )finished context:( void* )context;

@end

@interface ofxGenericUIView : UIView
{
@protected
    ofPtrWeak< ofxGenericView > _forwardTo;
}
-( id )initWithForwardTo:( ofPtrWeak< ofxGenericView > )forwardTo frame:( const ofRectangle& )frame;
-( UIView* )hitTest:( CGPoint )point withEvent:( UIEvent* )event;

@end

#elif TARGET_ANDROID
#include "JNIUtility.h"
#include "JNIRect.h"

jclass ofxGenericView::_jniClass = NULL;
const char* ofxGenericView::className = "cc/openframeworks/ofxGeneric/View";
#endif

ofPtr< ofxGenericView > ofxGenericView::create( const ofRectangle& setFrame, NativeView nativeView )
{
    ofPtr< ofxGenericView > create = ofPtr< ofxGenericView >( new ofxGenericView() );
    create->init( create, setFrame, nativeView );
    return create;
}

ofxGenericView::ofxGenericView()
:
#if TARGET_OS_IPHONE
 _view( nil ), _viewController( nil ),
#elif TARGET_ANDROID
 _view( NULL ), 
#endif
_isAttachedToRoot( false ), _children()
{
}

ofxGenericView::~ofxGenericView()
{
    removeFromParent();

#if TARGET_OS_IPHONE
    releaseView( _view );
    releaseViewController( _viewController );
#elif TARGET_ANDROID
	destroyJNIReference( _view );
#endif
}

void ofxGenericView::init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setFrameValue, NativeView nativeView )
{
    _this = setThis;

#if TARGET_ANDROID
    registerJNIMethods();
#endif
    
    willLoad();

    if ( nativeView )
    {
        _view = nativeView;
#if TARGET_OS_IPHONE
        [ _view retain ];
#endif
        setFrame( setFrameValue );
    } else 
    {
        _view = createNativeView( setFrameValue );
    }
    
#if TARGET_OS_IPHONE
    _viewController = createUIViewController();
    [ _viewController setView:_view ];

#elif TARGET_ANDROID
    JNIRect jniRect = ofRectangleToJNIRect( setFrame );
    callJNIVoidMethod(
    		_jniMethods,
    		JNIMethod_Init,
    		jniRect.getJNIInstance()
    		);

#endif
    
    didLoad();
}

#if TARGET_ANDROID
jclass ofxGenericView::getJNIClassStatic()
{
	// TODO: handle exception
    if ( !_jniClass )
    	_jniClass = ( jclass )ofxGenericView::createJNIReferenceStatic( JNIFindClass( ofxGenericView::className ) );
    return _jniClass;
}

jclass ofxGenericView::getJNIClass()
{
	return ofxGenericView::getJNIClassStatic();
}


jobject ofxGenericView::getJNIInstance()
{
	return _view;
}

#endif

#if TARGET_OS_IPHONE
UIView* ofxGenericView::allocNativeView( const ofRectangle& setFrame )
{
    return [ [ ofxGenericUIView alloc ] initWithForwardTo:_this frame:setFrame ];
}
#elif TARGET_ANDROID
jobject ofxGenericView::allocNativeView( const ofRectangle& frame )
{
    return createJNIInstance( _jniMethods, JNIMethod_constructor );
}
#endif

NativeView ofxGenericView::createNativeView( const ofRectangle& setFrame )
{
#if TARGET_OS_IPHONE
	UIView* newView = allocNativeView( setFrame );

	// TODO: move to shared code override-able function after _view assigned? or just assign _view in here
    [ newView setBackgroundColor:[ UIColor whiteColor ] ];
    [ newView setOpaque:YES ];
    [ newView setHidden:NO ];
    [ newView setClipsToBounds:YES ];
    
    return newView;

#elif TARGET_ANDROID
    // TODO: exception handling
    jobject newView = allocNativeView( setFrame );
    // set background white
    // set opaque
    // set not hidden
    return createJNIReference( newView );
    
#endif
}

NativeView ofxGenericView::getNativeView()
{
	return _view;
}

ofxGenericView::operator NativeView()
{
	return _view;
}

#if TARGET_OS_IPHONE
ofxUIGenericViewControllerForwarder* ofxGenericView::createUIViewController()
{
    return [ [ ofxUIGenericViewControllerForwarder alloc ] initWithForwardTo:_this ];
}

ofxUIGenericViewControllerForwarder* ofxGenericView::getUIViewController()
{
    return _viewController;
}
#endif

ofRectangle ofxGenericView::getFrame()
{
#if TARGET_OS_IPHONE
    return CGRectToofRectangle( [ _view frame ] );
#elif TARGET_ANDROID
    jobject jniFrame = callJNIObjectMethod( _jniMethods, JNIMethod_GetFrame );
    JNIRect jniRect( jniFrame );
    return JNIRectToofRectangle( jniRect );
#endif
}

ofRectangle ofxGenericView::getFrame( const ofPoint& setTopLeft )
{
    ofRectangle frame = getFrame();
    frame.x = setTopLeft.x;
    frame.y = setTopLeft.y;
    return frame;
}

void ofxGenericView::setFrame( const ofRectangle& setFrame )
{
#if TARGET_OS_IPHONE
    [ _view setFrame: ofRectangleToCGRect( setFrame ) ];
#elif TARGET_ANDROID
    JNIRect jniRect = ofRectangleToJNIRect( setFrame ); // TODO: temporary ref passing going to killlllll
    callJNIVoidMethod( _jniMethods, JNIMethod_SetFrame, jniRect.getJNIInstance() );
#endif
}

ofColor ofxGenericView::getBackgroundColor()
{
#if TARGET_OS_IPHONE
    return UIColorToofColor( [ _view backgroundColor ] );
#elif TARGET_ANDROID
    jint androidColor = callJNIIntMethod( _jniMethods, JNIMethod_GetBackgroundColor );
    return JNIColorToofColor( androidColor );
#endif
}

void ofxGenericView::setBackgroundColor( const ofColor& setColor )
{
#if TARGET_OS_IPHONE
    [ _view setBackgroundColor:ofColorToUIColor( setColor ) ];
#elif TARGET_ANDROID
    callJNIVoidMethod(
    		_jniMethods,
    		JNIMethod_SetBackgroundColor,
    		( int )setColor.r, ( int )setColor.g, ( int )setColor.b, ( int )setColor.a
    		);
#endif
}

void ofxGenericView::addChildView( ofPtr< ofxGenericView > add )
{
    if ( add )
    {
        addChildViewPre( add );
        
        _children.push_back( add );
        if ( add->getNativeView() )
        {
#if TARGET_OS_IPHONE
          	[ getNativeView() addSubview:add->getNativeView() ];
#elif TARGET_ANDROID
          	callJNIVoidMethod(
                              _jniMethods,
                              JNIMethod_AddChildView,
                              add->getNativeView()
                              );
#endif
        } else
        {
            // TODO:
        }
        addChildViewPost( add );
    }
}

void ofxGenericView::addChildViewBefore( ofPtr< ofxGenericView > add, ofPtr< ofxGenericView > before )
{
    if ( add )
    {
        if ( before && before->getParent().lock() == _this.lock() )
        {
            addChildViewPre( add );
            
            std::list< ofPtr< ofxGenericView > >::iterator findIndex = _children.begin();
            while ( findIndex != _children.end() && ( *findIndex ) != before )
            {
                findIndex ++;
            }
            // TODO: extra error check
            _children.insert( findIndex, add );
            
            if ( getNativeView() && add->getNativeView() )
            {
#if TARGET_OS_IPHONE
                [ getNativeView() insertSubview:add->getNativeView() belowSubview:before->getNativeView() ];
#elif TARGET_ANDROID
#endif
            }
            addChildViewPost( add );
        } else
        {
            addChildView( add );
        }
    }    
}

void ofxGenericView::addChildViewAfter( ofPtr< ofxGenericView > add, ofPtr< ofxGenericView > after )
{
    if ( add )
    {
        if ( after && after->getParent().lock() == _this.lock() )
        {
            addChildViewPre( add );
            
            std::list< ofPtr< ofxGenericView > >::iterator findIndex = _children.begin();
            while ( findIndex != _children.end() && ( *findIndex ) != after )
            {
                findIndex ++;
            }
            // TODO: extra error check
            _children.insert( findIndex, add );
            
            if ( getNativeView() && add->getNativeView() )
            {
#if TARGET_OS_IPHONE
                [ getNativeView() insertSubview:add->getNativeView() aboveSubview:after->getNativeView() ];
#elif TARGET_ANDROID
#endif
            }
            addChildViewPost( add );
        } else
        {
            addChildView( add );
        }
    }
}


void ofxGenericView::addChildViewPre( ofPtr< ofxGenericView > add )
{
    if ( add )
    {
        if ( _isAttachedToRoot )
        {
            add->willAppear();
        }
    }
}

void ofxGenericView::addChildViewPost( ofPtr< ofxGenericView > add )
{
    if ( add )
    {
        add->_parent = _this;
        
        if ( _isAttachedToRoot )
        {
            add->setIsAttachedToRoot( true );
            add->didAppear();
        }
    }
}

void ofxGenericView::removeChildView( ofPtr< ofxGenericView > remove )
{
    if ( remove && remove->_parent.lock() == _this.lock() )
    {
        if ( _isAttachedToRoot )
        {
            remove->willDisappear();
        }
        
        NativeView nativeView = remove->getNativeView();
        if ( nativeView )
        {
#if TARGET_OS_IPHONE
            [ nativeView removeFromSuperview ];
#elif TARGET_ANDROID
            callJNIVoidMethod(
                              _jniMethods,
                              JNIMethod_RemoveChildView,
                              nativeView
                              );
#endif
        }
        _children.remove( remove );
        remove->_parent = ofPtrWeak< ofxGenericView >();
        
        if ( _isAttachedToRoot )
        {
            remove->setIsAttachedToRoot( false );
            remove->didDisappear();
        }
    }
}

void ofxGenericView::setIsAttachedToRoot( bool attached )
{
    _isAttachedToRoot = attached;
}

void ofxGenericView::removeFromParent()
{
    if ( _parent )
    {
        ofPtr< ofxGenericView > parentPtr = _parent.lock();
        parentPtr->removeChildView( _this.lock() );
    }
}

void ofxGenericView::removeChildViews()
{
    _children.clear();
}

ofPtrWeak< ofxGenericView > ofxGenericView::getParent()
{
    return _parent;
}

void ofxGenericView::setAlpha( float alpha )
{
#if TARGET_OS_IPHONE
    if ( _view )
    {
        [ _view setAlpha:( CGFloat )alpha ];
    }
#endif
}

float ofxGenericView::getAlpha()
{
#if TARGET_OS_IPHONE
    if ( _view )
    {
        return ( float )_view.alpha;
    }
#endif    
    return 1.0f;
}

void ofxGenericView::setVisible( bool visible )
{    
#if TARGET_OS_IPHONE
    if ( _view )
    {
        [ _view setHidden:( BOOL )!visible ];
    }
#endif
}

bool ofxGenericView::getVisible()
{
#if TARGET_OS_IPHONE
    if ( _view )
    {
        return ( bool )!_view.isHidden;
    }
#endif
    return false;
}

void ofxGenericView::setClipSubviews( bool clip )
{
#if TARGET_OS_IPHONE
    if ( _view )
    {
        [ _view setClipsToBounds:( BOOL )clip ];
    }
#endif
}

bool ofxGenericView::getClipSubviews()
{
#if TARGET_OS_IPHONE
    if ( _view )
    {
        return ( bool )_view.clipsToBounds;
    }
#endif    
    return false;
}

void ofxGenericView::setAutoresizingMask( int autoresizing )
{
#if TARGET_OS_IPHONE
    if ( _view )
    {
        [ _view setAutoresizingMask:ofxGenericViewAutoresizingToiOS( ( ofxGenericViewAutoresizing )autoresizing ) ];
    }
#endif
}

int ofxGenericView::getAutoresizingMask( )
{
#if TARGET_OS_IPHONE
    if ( _view )
    {
        return  iOSToofxGenericViewAutoresizing( _view.autoresizingMask );
    }
#endif
    return 0;
}

void ofxGenericView::setNextResponder( ofPtrWeak< ofxGenericView > view )
{
    _nextResponder = view;
}

ofPtr< ofxGenericView > ofxGenericView::getNextResponder()
{
    return _nextResponder.lock();
}

void ofxGenericView::willLoad()
{
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->willLoad( _this.lock() );
    }
};

void ofxGenericView::didLoad()
{
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->didLoad( _this.lock() );
    }
};

void ofxGenericView::willAppear()
{
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->willAppear( _this.lock() );
    }
    
    for( std::list< ofPtr< ofxGenericView > >::iterator travChildren = _children.begin(); travChildren != _children.end(); travChildren ++ )
    {
        ( *travChildren )->willAppear();
    }
};

void ofxGenericView::didAppear()
{
    setIsAttachedToRoot( true );
    
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->didAppear( _this.lock() );
    }
    
    for( std::list< ofPtr< ofxGenericView > >::iterator travChildren = _children.begin(); travChildren != _children.end(); travChildren ++ )
    {
        ( *travChildren )->didAppear();
    }
};

void ofxGenericView::willDisappear()
{
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->willDisappear( _this.lock() );
    }

    for( std::list< ofPtr< ofxGenericView > >::iterator travChildren = _children.begin(); travChildren != _children.end(); travChildren ++ )
    {
        ( *travChildren )->willDisappear();
    }
};

void ofxGenericView::didDisappear()
{
    setIsAttachedToRoot( false );
    
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->didDisappear( _this.lock() );
    }

    for( std::list< ofPtr< ofxGenericView > >::iterator travChildren = _children.begin(); travChildren != _children.end(); travChildren ++ )
    {
        ( *travChildren )->didDisappear();
    }
}

void ofxGenericView::hitInView( ofPoint location )
{
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->hitInView( location );
    }
}

void ofxGenericView::setViewDelegate( ofPtrWeak< ofxGenericViewDelegate > delegate )
{
    _viewDelegate = delegate;
}

bool ofxGenericView::containsPoint( const ofPoint& point )
{
#if TARGET_OS_IPHONE
    return [ _view pointInside:CGPointMake(point.x, point.y) withEvent:nil ];
#else
    return false;
#endif
}

bool ofxGenericView::containsPoint( const ofPoint& point, ofPtr< ofxGenericView > pointRelativeTo )
{
    ofPoint local = convertFrom( point, pointRelativeTo );
    return containsPoint( local );
}


ofPoint ofxGenericView::convertFrom( const ofPoint& point, ofPtr< ofxGenericView > view )
{
#if TARGET_OS_IPHONE
    CGPoint p = [ _view convertPoint:CGPointMake(point.x, point.y) fromView:view->getNativeView() ];
    return ofPoint( p.x, p.y );
#else
    return ofPoint( 0, 0 );
#endif
}

ofRectangle ofxGenericView::convertFrom( const ofRectangle& rectangle, ofPtr< ofxGenericView > view )
{
#if TARGET_OS_IPHONE
    CGRect converted = [ _view convertRect:ofRectangleToCGRect( rectangle ) fromView:view->getNativeView() ];
    return CGRectToofRectangle( converted );
#else
    return ofRectangle();
#endif
}

void ofxGenericView::beginAnimation( string animationId, ofPtr< ofxGenericViewDelegate > delegate )
{
#if TARGET_OS_IPHONE
    [ UIView beginAnimations:ofxStringToNSString( animationId ) context:nil ];
    if ( delegate )
    {
        ofxGenericViewAnimationForwarder* forwarder = [ [ ofxGenericViewAnimationForwarder alloc ] initWithDelegate:delegate ];
        [ UIView setAnimationDelegate:forwarder ];
        [ UIView setAnimationWillStartSelector:@selector( animationWillStart:finished:context: ) ];
        [ UIView setAnimationDidStopSelector:@selector( animationDidStop:finished:context: ) ];
        [ forwarder release ];
    }
#elif TARGET_ANDROID
#endif
}

void ofxGenericView::commitAnimation()
{
#if TARGET_OS_IPHONE
    [ UIView commitAnimations ];
#elif TARGET_ANDROID
#endif
}

void ofxGenericView::setAnimationDuration( double seconds )
{
#if TARGET_OS_IPHONE
    [ UIView setAnimationDuration:( NSTimeInterval )seconds ];
#elif TARGET_ANDROID
#endif
}

void ofxGenericView::setAnimationDelay( double seconds )
{
#if TARGET_OS_IPHONE
    [ UIView setAnimationDelay:( NSTimeInterval )seconds ];
#elif TARGET_ANDROID
#endif
}

void ofxGenericView::setAnimationCurve( ofxGenericViewAnimationCurve curve )
{
#if TARGET_OS_IPHONE
    [ UIView setAnimationCurve:ofxGenericViewAnimationCurveToiOS( curve ) ];
#elif TARGET_ANDROID
#endif    
}

void ofxGenericView::setAnimationTransition( ofxGenericViewAnimationTransition transition, ofPtr< ofxGenericView > forView )
{
#if TARGET_OS_IPHONE
    [ UIView setAnimationTransition:ofxGenericViewAnimationTransitionToiOS( transition ) forView:forView->getNativeView() cache:NO ];
#elif TARGET_ANDROID
#endif    
}

void ofxGenericView::replaceViewWithView( ofPtr< ofxGenericView > replace, ofPtr< ofxGenericView > with )
{
    if ( replace && replace->getParent() && with )
    {
        ofPtr< ofxGenericView > parent = replace->getParent().lock();
        // TODO: match autosizing?
        with->setFrame( replace->getFrame() );
        parent->addChildViewBefore( with, replace );
        parent->removeChildView( replace );
    }
}

void ofxGenericView::addGestureRecognizerSwipe( ofxGenericGestureTypeSwipe type )
{
#if TARGET_OS_IPHONE
    UISwipeGestureRecognizer *swipeRecognizer = [ [ [ UISwipeGestureRecognizer alloc ] initWithTarget:_viewController action:@selector( gesturePerformedSwipe: ) ] autorelease ];
	[ swipeRecognizer setDirection:ofxGenericGestureTypeSwipeToiOS( type ) ];
    swipeRecognizer.cancelsTouchesInView = NO;
    swipeRecognizer.delegate = _viewController;
	[ _view addGestureRecognizer:swipeRecognizer ];
#elif TARGET_ANDROID
#endif
}

void ofxGenericView::addGestureRecognizerTap( int tapCount, int fingerCount )
{
#if TARGET_OS_IPHONE
    UITapGestureRecognizer *recognizer = [ [ [ UITapGestureRecognizer alloc ] initWithTarget:_viewController action:@selector( gesturePerformedTap: ) ] autorelease ];
	recognizer.numberOfTapsRequired = tapCount;
    recognizer.numberOfTouchesRequired = fingerCount;
    recognizer.cancelsTouchesInView = NO;
    recognizer.delegate = _viewController;
	[ _view addGestureRecognizer:recognizer ];
#elif TARGET_ANDROID
#endif
}

void ofxGenericView::removeGestureRecognizers( )
{
#if TARGET_OS_IPHONE
    NSArray *recognizers = [ NSArray arrayWithArray:_view.gestureRecognizers ];
    for ( unsigned int i = 0; i < [recognizers count]; i++ )
    {
        [ _view removeGestureRecognizer:[recognizers objectAtIndex:i] ];
    }
#elif TARGET_ANDROID
#endif
}

void ofxGenericView::addGestureRecognizerHold( float minimumPressDuration, unsigned int fingerCount, float allowableMovement )
{
#if TARGET_OS_IPHONE
    UILongPressGestureRecognizer* recognizer = [ [ [ UILongPressGestureRecognizer alloc ] initWithTarget:_viewController action:@selector( gesturePerformedHold: ) ] autorelease ];
    recognizer.minimumPressDuration = minimumPressDuration;
    recognizer.numberOfTapsRequired = 0;
    recognizer.numberOfTouchesRequired = fingerCount;
    recognizer.cancelsTouchesInView = NO;
    recognizer.delegate = _viewController;
    recognizer.allowableMovement = allowableMovement;
    
	[ _view addGestureRecognizer:recognizer ];
    
#elif TARGET_ANDROID
#endif
}

void ofxGenericView::addGestureRecognizerPan( unsigned int minimumFingerCount, unsigned int maximumFingerCount )
{
#if TARGET_OS_IPHONE
    UIPanGestureRecognizer* recognizer = [ [ [ UIPanGestureRecognizer alloc ] initWithTarget:_viewController action:@selector( gesturePerformedPan: ) ] autorelease ];
    recognizer.minimumNumberOfTouches = minimumFingerCount;
    recognizer.maximumNumberOfTouches = maximumFingerCount;
    recognizer.cancelsTouchesInView = NO;
    recognizer.delegate = _viewController;
    
	[ _view addGestureRecognizer:recognizer ];
#elif TARGET_ANDROID
#endif
}

#if TARGET_OS_IPHONE
void ofxGenericView::gesturePerformedSwipe( UISwipeGestureRecognizer* recognizer )
{
    CGPoint cgp = [recognizer locationInView:getNativeView()];
    ofPoint p = ofPoint(cgp.x, cgp.y);
    ofxGenericGestureTypeSwipe type = iOSToofxGenericGestureTypeSwipe( [ recognizer direction ] );
    gesturePerformedSwipe( type, p );
}

void ofxGenericView::gesturePerformedTap( UITapGestureRecognizer* recognizer )
{
    CGPoint cgp = [recognizer locationInView:getNativeView()];
    ofPoint p = ofPoint(cgp.x, cgp.y);
    gesturePerformedTap( [ recognizer numberOfTapsRequired ], [ recognizer numberOfTouches ], p );
}

void ofxGenericView::gesturePerformedHold( UILongPressGestureRecognizer* recognizer )
{
    CGPoint cgp = [recognizer locationInView:getNativeView()];
    ofPoint p = ofPoint(cgp.x, cgp.y);
    gesturePerformedHold(
                         (float)[ recognizer minimumPressDuration ],
                         [ recognizer numberOfTouches ],
                         [ recognizer allowableMovement ],
                         p
                         );

}

void ofxGenericView::gesturePerformedPan( UIPanGestureRecognizer* recognizer )
{
//    CGPoint cgp = [recognizer locationInView:getNativeView()];
    CGPoint velocity = [ recognizer velocityInView:getNativeView() ];
    gesturePerformedPan( [ recognizer numberOfTouches ], ofPoint( velocity.x, velocity.y ) );
}

#endif

void ofxGenericView::gesturePerformedSwipe( ofxGenericGestureTypeSwipe type, ofPoint location )
{
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->gesturePerformedSwipe( _this.lock(), type, location );
    }
}

void ofxGenericView::gesturePerformedTap( int tapCount, int fingerCount, ofPoint location )
{
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->gesturePerformedTap( _this.lock(), tapCount, fingerCount, location );
    }
}

void ofxGenericView::gesturePerformedHold( float minimumPressDuration, unsigned int fingerCount, float allowableMovement, ofPoint location )
{
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->gesturePerformedHold( _this.lock(), minimumPressDuration, fingerCount, allowableMovement, location );
    }
}

void ofxGenericView::gesturePerformedPan( unsigned int fingerCount, ofPoint distance )
{
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->gesturePerformedPan( _this.lock(), fingerCount, distance );
    }
}

string ofxGenericView::dumpViewGraph( int depth )
{
#if defined(DEBUG) || defined(TEST)
    string result;
    string tab;
    for( int trav = 0 ; trav < depth; trav ++ )
        tab += " ";
    tab += " ";
    
    result += tab + toString() + "\n";

    if ( _children.size() )
    {
        result += tab + "\\\n";

        for( std::list< ofPtr< ofxGenericView > >::const_iterator travChildren = _children.begin(); travChildren != _children.end(); travChildren++ )
        {
            result += ( *travChildren )->dumpViewGraph( depth + 1 );
        }
    }
    
    return result;
#else
    return string();
#endif
}

string ofxGenericView::toString()
{
#if defined(DEBUG) || defined(TEST)
    const std::type_info& info = typeid( *this );
    
    string result( info.name() );    
#if TARGET_OS_IPHONE
    string nativeClassName = ofxNSStringToString( NSStringFromClass ( [ getNativeView() class ] ) ); 
    result += " native: " + nativeClassName;
#else
#endif
    result += " frame: " + getFrame().toString();
    result += " visible: " + ofxGToString( getVisible() );
    return result;
    
#else
    return string();
#endif
}

#if TARGET_ANDROID
void ofxGenericView::registerJNIMethods()
{
    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_constructor,
			"<init>",
			JNIEncodeMethodSignature( 0, JNIType_void )
    		);

    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_Init,
			"Init",
			JNIEncodeMethodSignature( 1, JNIType_void, JNIType_object, JNIRect::className )
    		);

    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_GetFrame,
    		"getFrame",
    		JNIEncodeMethodSignature( 0, JNIType_object, JNIRect::className )
    		);

    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_SetFrame,
    		"setFrame",
    		JNIEncodeMethodSignature( 1, JNIType_void, JNIType_object, JNIRect::className )
    		);

    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_GetBackgroundColor,
    		"getBackgroundColor",
    		JNIEncodeMethodSignature( 0, JNIType_int )
    		);

    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_SetBackgroundColor,
    		"setBackgroundColor",
    		JNIEncodeMethodSignature( 4, JNIType_void, JNIType_int, JNIType_int, JNIType_int, JNIType_int )
    		);

    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_AddChildView,
    		"addChildView",
    		JNIEncodeMethodSignature( 1, JNIType_void, JNIType_object, ofxGenericView::className )
    		);

    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_RemoveChildView,
    		"removeChildView",
    		JNIEncodeMethodSignature( 1, JNIType_void, JNIType_object, ofxGenericView::className )
    		);
}
#endif

#if TARGET_OS_IPHONE

@implementation ofxUIGenericViewControllerForwarder

-( id )initWithForwardTo:( ofPtrWeak< ofxGenericView > )forwardTo
{ 
    self = [ super init ];
    if ( self )
    {
        _forwardTo = forwardTo;
    }
    return self;
}

-( BOOL )shouldAutorotateToInterfaceOrientation:( UIInterfaceOrientation )interfaceOrientation
{
    if ( _forwardTo )
    {
    }
    return ofxGenericApp::getInstance()->shouldAutorotate( iOSToofOrientation( interfaceOrientation ) );
}

-( void )gesturePerformedSwipe:( UIGestureRecognizer* )recognizer
{
    if ( _forwardTo && [ recognizer isKindOfClass:[ UISwipeGestureRecognizer class ] ] )
    {
        _forwardTo.lock()->gesturePerformedSwipe( ( UISwipeGestureRecognizer* )recognizer );
    }
}

-( void )gesturePerformedTap:( UIGestureRecognizer* )recognizer
{
    if ( _forwardTo && [ recognizer isKindOfClass:[ UITapGestureRecognizer class ] ] )
    {
        _forwardTo.lock()->gesturePerformedTap( ( UITapGestureRecognizer* )recognizer );
    }
}

-( void )gesturePerformedHold:( UIGestureRecognizer* )recognizer
{
    if ( _forwardTo && [ recognizer isKindOfClass:[ UILongPressGestureRecognizer class ] ] )
    {
        _forwardTo.lock()->gesturePerformedHold( ( UILongPressGestureRecognizer* )recognizer );
    }
}

-( void )gesturePerformedPan:( UIGestureRecognizer* )recognizer
{
    if ( _forwardTo && [ recognizer isKindOfClass:[ UIPanGestureRecognizer class ] ] )
    {
        _forwardTo.lock()->gesturePerformedPan( ( UIPanGestureRecognizer* )recognizer );
    }
}

-( BOOL )gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch
{
    if ( _forwardTo && touch && touch.view && touch.view != _forwardTo.lock()->getNativeView() )
    {
        return NO;
    }
    return YES;
}

@end

@implementation ofxGenericViewAnimationForwarder

-( id )initWithDelegate:( ofPtrWeak< ofxGenericViewDelegate > )delegate
{
    self = [ super init ];
    if ( self )
    {
        _delegate = delegate;
    }
    return self;    
}

- (void)animationWillStart:( NSString* )animationID finished:( NSNumber* )finished context:( void* )context
{
    if ( _delegate )
    {
        _delegate.lock()->animationWillStart( ofxNSStringToString( animationID ) );
    }
}

- (void)animationDidStop:( NSString* )animationID finished:( NSNumber* )finished context:( void* )context
{
    if ( _delegate )
    {
        _delegate.lock()->animationDidStop( ofxNSStringToString( animationID ) );
    }
}

@end

@implementation ofxGenericUIView

-( id )initWithForwardTo:( ofPtrWeak< ofxGenericView > )forwardTo frame:( const ofRectangle& )frame
{
    self = [ super initWithFrame:ofRectangleToCGRect( frame ) ];
    if ( self )
    {
        _forwardTo = forwardTo;
    }
    return self;    
}

- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    if ( event.type == UIEventTypeTouches )
    {
        if ( _forwardTo )
        {
            _forwardTo.lock()->hitInView( ofPoint( point.x, point.y ) );
        }
    }
    
    return [ super hitTest:point withEvent:event ];
}

@end
#endif


