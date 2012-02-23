#include "ofxGenericUtility.h"
#include "ofLog.h"
#include "ofxGenericConstants.h"

void ofxGLog( ofLogLevel level, const string & message )
{
	ofLog( ofxGenericModuleName, level, message );
}

void ofxGLogError( const string & message )
{
	ofLogError( ofxGenericModuleName, message );
}

void ofxGLogFatalError( const string & message )
{
	ofLogFatalError( ofxGenericModuleName, message );
}

void ofxGLogNotice( const string & message )
{
	ofLogNotice( ofxGenericModuleName, message );
}

void ofxGLogVerbose( const string & message )
{
	ofLogVerbose( ofxGenericModuleName, message );
}

void ofxGLogWarning( const string & message )
{
	ofLogWarning( ofxGenericModuleName, message );
}

#include "android/log.h"
void ofxGLog( ofLogLevel level, const char* format, ... )
{
	va_list args;
	va_start( args, format );
	string message = ofVAArgsToString( format, args );
	va_end( args );
	ofLog( ofxGenericModuleName, level, message );
}