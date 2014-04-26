/*
 Copyright (c) 2012, The Cinder Project, All rights reserved.

 This code is intended for use with the Cinder C++ library: http://libcinder.org

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#include "cinder/app/AppImplLinux.h"
#include "cinder/app/App.h"
#include "cinder/Utilities.h"
#include "cinder/Display.h"

using std::string;
using std::wstring;
using std::vector;
using std::pair;

namespace cinder { namespace app {


AppImplLinux::AppImplLinux( App *aApp )
	: mApp( aApp ), mSetupHasBeenCalled( false )
{
}

AppImplLinux::~AppImplLinux()
{
}

void AppImplLinux::hideCursor()
{
}

void AppImplLinux::showCursor()
{
}

Buffer AppImplLinux::loadResource( int id, const std::string &type )
{
}

fs::path AppImplLinux::getAppPath()
{

	return fs::path( std::string( "test/App/path" ) );
}

fs::path AppImplLinux::getOpenFilePath( const fs::path &initialPath, vector<string> extensions )
{
		return string();
}


fs::path AppImplLinux::getFolderPath( const fs::path &initialPath )
{
	string result;


	return result;
}

fs::path AppImplLinux::getSaveFilePath( const fs::path &initialPath, vector<string> extensions )
{
	string result;

	return result;
}

///////////////////////////////////////////////////////////////////////////////
// WindowImplLinux
WindowImplLinux::WindowImplLinux( const Window::Format &format, RendererRef sharedRenderer, AppImplLinux *appImpl )
	: mWindowOffset( 0, 0 ), mAppImpl( appImpl ), mHidden( false ), mIsDragging(false)
{	
	mFullScreen = format.isFullScreen();
	mDisplay = format.getDisplay();
	mRenderer = format.getRenderer();
	std::cout << " RENDERER " << mRenderer << std::endl;
	mResizable = format.isResizable();
	mAlwaysOnTop = format.isAlwaysOnTop();
	mBorderless = format.isBorderless();

	mWindowedSize = format.getSize();
	mWindowWidth = mWindowedSize.x;
	mWindowHeight = mWindowedSize.y;
	if( format.isPosSpecified() )
		mWindowOffset = mWindowedPos = format.getPos();
	else {
		Vec2i displaySize = mDisplay->getSize();
		mWindowOffset = mWindowedPos = ( displaySize - mWindowedSize ) / 2;
	}

	createWindow( Vec2i( mWindowWidth, mWindowHeight ), format.getTitle(), mDisplay, sharedRenderer );
	// set WindowRef and its impl pointer to this
	mWindowRef = Window::privateCreate__( this, mAppImpl->getApp() );
	mAppImpl->setWindow( mWindowRef );
	
}


void WindowImplLinux::createWindow( const Vec2i &windowSize, const std::string &title, const DisplayRef display, RendererRef sharedRenderer )
{
	char windowTitle[1024];
	std::strncpy( windowTitle, title.c_str(), sizeof(windowTitle) );
	windowTitle[sizeof(windowTitle) - 1] = 0;
    GLFWwindow* _GLFWwindow = glfwCreateWindow( windowSize.x, windowSize.y, windowTitle, NULL, NULL );
    glfwSetWindowUserPointer( _GLFWwindow, this );
    glfwSetWindowCloseCallback( _GLFWwindow, window_close_callback );
    mRenderer->setup( mAppImpl->getApp(), _GLFWwindow, sharedRenderer );
}



void WindowImplLinux::setFullScreen( bool fullScreen, const app::FullScreenOptions &options )
{
	if( mFullScreen != fullScreen )
		toggleFullScreen( options );
}

void WindowImplLinux::toggleFullScreen( const app::FullScreenOptions &options )
{
}

void WindowImplLinux::getScreenSize( int clientWidth, int clientHeight, int *resultWidth, int *resultHeight )
{
}

void WindowImplLinux::setPos( const Vec2i &windowPos )
{
}

void WindowImplLinux::hide()
{
        mHidden = true;
}

void WindowImplLinux::show()
{
        mHidden = false;
}

bool WindowImplLinux::isHidden() const
{
	return mHidden;
}

std::string	WindowImplLinux::getTitle() const
{
	return "";
}

void WindowImplLinux::setTitle( const std::string &title )
{
	std::wstring titleWide = toUtf16( title );
}

void WindowImplLinux::setSize( const Vec2i &windowSize )
{
	int screenWidth, screenHeight;
	getScreenSize( windowSize.x, windowSize.y, &screenWidth, &screenHeight );
}

void WindowImplLinux::close()
{
	getAppImpl()->closeWindow( this );
	// at this point 'this' is invalid so do nothing after this line
}

void WindowImplLinux::setBorderless( bool borderless )
{
}
 
void WindowImplLinux::setAlwaysOnTop( bool alwaysOnTop )
{
}

void WindowImplLinux::draw()
{
	mAppImpl->setWindow( mWindowRef );
	mRenderer->startDraw();
	mWindowRef->emitDraw();
	mRenderer->finishDraw();
}

void WindowImplLinux::resize()
{
	mAppImpl->setWindow( mWindowRef );
	mWindowRef->emitResize();
}

void WindowImplLinux::redraw()
{
}

void WindowImplLinux::privateClose()
{
	mRenderer->kill();
}

void WindowImplLinux::keyDown( const KeyEvent &event )
{
	KeyEvent localEvent( event );
	mAppImpl->setWindow( mWindowRef );
	mWindowRef->emitKeyDown( &localEvent );
}

} } // namespace cinder::app
