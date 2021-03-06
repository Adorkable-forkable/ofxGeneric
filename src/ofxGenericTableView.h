//
//  ofxGenericTableView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 1/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"
#include "ofxGenericScrollView.h"
#include "ofxGenericConstants.h"

#if TARGET_OS_IPHONE
@class ofxGenericTableViewForwarder;
#endif
class ofxGenericTableViewCell;
class ofxGenericTableViewDelegate;

class ofxGenericTableView : public ofxGenericScrollView
{
public:
    static ofPtr< ofxGenericTableView > create( bool areHeadersFixedInPlace = false, const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ) );
    virtual ~ofxGenericTableView();
    
    virtual unsigned int getNumberOfCells( unsigned int section );
    virtual ofPtr< ofxGenericTableViewCell > getCell( unsigned int section, unsigned int index );
    virtual float getHeightForCell( unsigned int section, unsigned int index );
    
    virtual unsigned int getNumberOfSections();
    
    bool getAreHeadersFixedInPlace();
    virtual float getHeightForHeaderInSection( unsigned int section );
    virtual ofPtr< ofxGenericTableViewCell > getHeaderForSection( unsigned int section );

    float getContentHeight();

    void setAutoresizeToFit( bool autoResizeToFit );
    
    virtual void setSeparatorColor( const ofColor& separatorColor );
    virtual void setSeparatorStyle( ofxGenericTableViewSeparatorStyle separatorStyle );
    ofxGenericTableViewSeparatorStyle getSeparatorStyle();
    virtual void setSeparatorPaddedHeight( float height );    
    virtual void setUsePaddedSeparator( float height );

    virtual void setDelegate( ofPtrWeak< ofxGenericTableViewDelegate > delegate );
    virtual void reloadData();
    
    virtual void selectedRow( unsigned int section, unsigned int index);
    virtual void setAllowsSelection( bool allow );
    virtual bool allowsSelection();
    virtual void setCellIsSelected( unsigned int section, unsigned int index, bool selected);
    virtual bool deselectAllCells();
    virtual int getSelectedIndex();
    virtual int getSelectedSection();
    
    virtual void setCellDragging( bool enabled );
    bool getCellDraggingEnabled( unsigned int section, unsigned int index );
    virtual void moveRow( unsigned int sourceSection, unsigned int sourceIndex, unsigned int destinationSection, unsigned int destinationIndex );
    
    virtual void reloadCell( unsigned int section, unsigned int index );
    
    virtual void scrollToCell( unsigned int section, unsigned int index );
    
#if TARGET_OS_IPHONE
    operator UITableView*();
#endif

    virtual ofPtr< ofxGenericTableViewCell > nativeGetHeaderInSection( unsigned int section );
    virtual float nativeGetHeightForHeaderInSection( unsigned int section );
    virtual unsigned int nativeGetNumberOfCells( unsigned int section );
    virtual ofPtr< ofxGenericTableViewCell > nativeGetCell( unsigned int section, unsigned int index );
    virtual float nativeGetHeightForCell( unsigned int section, unsigned int index );
    virtual void nativeSelectedRow( unsigned int section, unsigned int index );
    virtual float nativeGetContentHeight();
    
    virtual void setHeaderView( ofPtr< ofxGenericView > headerView );
    virtual void setFooterView( ofPtr< ofxGenericView > footerView );

protected:
    virtual void init( ofPtrWeak< ofxGenericView > setThis, bool areHeadersFixedInPlace, const ofRectangle& setBounds = ofRectangle( 0, 0, 0, 0 ), NativeView nativeView = NativeNull );
    virtual NativeView createNativeView( const ofRectangle& frame );
#if TARGET_OS_IPHONE
    ofxGenericTableViewForwarder* _forwarder;    
#endif
    ofPtrWeak< ofxGenericTableViewDelegate > _delegate;

    ofxGenericTableViewSeparatorStyle _separatorStyle;
    float _paddedSeparatorHeight;
    ofPtr< ofxGenericTableViewCell > _paddedSeparator;
    
    bool _areHeadersFixedInPlace;
    
    virtual void resizeToFitContents();
    bool _autoResizeToFit;
    float _maximumHeight;
    
    bool _cellDraggingEnabled;
    
    ofxGenericTableView();
};

class ofxGenericTableViewDelegate
{
public:
    virtual ~ofxGenericTableViewDelegate() {};
    virtual unsigned int getNumberOfCells( ofPtr< ofxGenericTableView > tableView, unsigned int section ) { return 0; };
    virtual ofPtr< ofxGenericTableViewCell > getCell( ofPtr< ofxGenericTableView > tableView, unsigned int section, unsigned int index ) { return ofPtr< ofxGenericTableViewCell >(); };
    virtual float getHeightForCell( ofPtr< ofxGenericTableView > tableView, unsigned int section, unsigned int index ) { return 0.0f; };
    
    virtual void selectedRow( ofPtr< ofxGenericTableView > tableView, unsigned int section, unsigned int index) {};

    virtual ofPtr< ofxGenericTableViewCell > getHeaderForSection( ofPtr< ofxGenericTableView > tableView, unsigned int section ) { return ofPtr< ofxGenericTableViewCell >(); };
    virtual float getHeightForHeaderInSection( ofPtr< ofxGenericTableView > tableView, unsigned int section ) { return 0.0f; };

    virtual unsigned int getNumberOfSections( ofPtr< ofxGenericTableView > tableView ) { return 1; };
    
    virtual void moveRow( ofPtr< ofxGenericTableView > tableView, unsigned int sourceSection, unsigned int sourceIndex, unsigned int destinationSection, unsigned int destinationIndex ) {};
};