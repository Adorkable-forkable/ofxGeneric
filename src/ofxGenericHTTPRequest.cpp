//
//  ofxGenericHTTPRequest.cpp
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericHTTPRequest.h"
#include "ofxGenericUtility.h"

ofPtr< ofxGenericHTTPRequest > ofxGenericHTTPRequest::create( string url, string method, void* data, int dataByteLength, ofPtr< ofxGenericHTTPRequestDelegate > delegate )
{
    ofPtr< ofxGenericHTTPRequest > create = ofPtr< ofxGenericHTTPRequest >( new ofxGenericHTTPRequest( url, method, data, dataByteLength, delegate ) );
    create->_this = create;
    return create;
}

ofxGenericHTTPRequest::ofxGenericHTTPRequest( string url, string method, void* data, int dataByteLength, ofPtr< ofxGenericHTTPRequestDelegate > delegate )
: _delegate( delegate )
#if TARGET_OS_IPHONE
, _connection( nil ), _forwarder( nil )
#endif
{
#if TARGET_OS_IPHONE
    // TODO: allow caching and timeout specification
    NSURLRequest* request = [ NSURLRequest requestWithURL:[ NSURL URLWithString:ofxStringToNSString( url ) ] cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:20.0f ];

    _forwarder = [ [ NSURLConnectionDelegateForwarder alloc ] initWithDelegate:_this ];
    _connection = [ [ NSURLConnection alloc ] initWithRequest:request delegate:_forwarder startImmediately:YES ];
#endif
}

ofxGenericHTTPRequest::~ofxGenericHTTPRequest()
{
    cancel();
#if TARGET_OS_IPHONE
    release( _forwarder );
#endif
}

void ofxGenericHTTPRequest::cancel()
{
#if TARGET_OS_IPHONE
    [ _connection cancel ];
    release( _connection );
#endif
}

void ofxGenericHTTPRequest::finishedWithError( ofPtr< ofxGenericHTTPResponse > response )
{
    if ( _delegate )
    {
        _delegate->httpRequest_finishedWithError( _this.lock(), response );
    }
}

void ofxGenericHTTPRequest::finishedSuccessfully( ofPtr< ofxGenericHTTPResponse > response )
{
    if ( _delegate )
    {
        _delegate->httpRequest_finishedSuccessfully( _this.lock(), response );
    }
}

#if TARGET_OS_IPHONE
@implementation NSURLConnectionDelegateForwarder

-( id )initWithDelegate:( ofPtrWeak< ofxGenericHTTPRequest > )delegate
{
    self = [ super init ];
    if ( self )
    {
        _delegate = delegate;
        _receivedData = [ [ NSMutableData alloc ] init ];
        _response = nil;
    }
    return self;
}

-( void )dealloc
{
    release( _receivedData );
    release( _response );
    [ super dealloc ];
}

-( void )connection:( NSURLConnection* )connection didFailWithError:( NSError* )error
{
    if ( _delegate )
    {
        ofPtr< ofxGenericHTTPResponse > response = ofxGenericHTTPResponse::create( error );
        _delegate.lock()->finishedWithError( response );
    }
}

-( void )connection:( NSURLConnection* )connection didReceiveResponse:( NSURLResponse* )response
{
    _response = [ response retain ];
}

-( void )connection:( NSURLConnection* )connection didReceiveData:( NSData* )data
{
    [ _receivedData appendData:data ];
}

-( void )connectionDidFinishLoading:( NSURLConnection* )connection
{
    if ( _delegate )
    {
        ofPtr< ofxGenericHTTPResponse > response = ofxGenericHTTPResponse::create( _response, _receivedData );
        _delegate.lock()->finishedSuccessfully( response );
    }
}

#ifdef SSL_PROTOCOL_VERIFICATION_OFF
-( BOOL )connection:( NSURLConnection* )connection canAuthenticateAgainstProtectionSpace:( NSURLProtectionSpace * )protectionSpace
{
    return [ protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust ];
}

-( void )connection:( NSURLConnection*)connection didReceiveAuthenticationChallenge:( NSURLAuthenticationChallenge* )challenge
{
    if ( [ challenge.protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust ] )
    {
//        if ( [ trustedHosts containsObject:challenge.protectionSpace.host])
        {
            [ challenge.sender useCredential:[ NSURLCredential credentialForTrust:challenge.protectionSpace.serverTrust ] forAuthenticationChallenge:challenge ];
        }
    }
    [ challenge.sender continueWithoutCredentialForAuthenticationChallenge:challenge ];
}
#endif

@end
#endif
