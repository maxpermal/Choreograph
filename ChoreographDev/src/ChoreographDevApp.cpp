#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Choreograph.hpp"
#include "cinder/Easing.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace choreograph;

class ChoreographDevApp : public AppNative {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
private:
  float                 _ball_y;
  ci::vec2              _ball_2 = ci::vec2( 400.0f, 400.0f );
  float                 _ball_radius = 50.0f;
  co::Timeline          _anim;
};

void ChoreographDevApp::setup()
{
  auto &sequence = _anim.move( &_ball_y )
  .startFn( [] (Motion<float> &c) { cout << "Start red" << endl; } )
  .getSequence().set( 5.0f ).hold( 0.5f ).rampTo( 500.0f, 1.0f, EaseInOutQuad() ).hold( 500.0f, 0.33f ).rampTo( 700.0f, 1.0f ).hold( 20.0f, 1.0f ).set( 400.0f );

  _anim.move( &_ball_2 )
  .startFn( [] (Motion<vec2> &c) { cout << "Start blue" << endl; } )
  .finishFn( [] (Motion<vec2> &c) { c.playbackSpeed( c.getPlaybackSpeed() * -1.0f ); } )
  .continuous( true )
  .updateFn( [&] (const vec2 &v) {
    vec2 size = app::getWindowSize();
    float shortest = std::min( v.x, size.x - v.x );
    shortest = std::min( shortest, size.y - v.y );
    shortest = std::min( shortest, v.y );
    _ball_radius = shortest;
  } )
  .getSequence().rampTo( vec2( app::getWindowSize() ) / 2.0f, 2.0f ).rampTo( vec2( app::getWindowSize() ), 2.0f ).rampTo( vec2( app::getWindowWidth() / 2.0f, 10.0f ), 3.0f ).rampTo( vec2( app::getWindowSize() ) / 2.0f, 0.5f );

}

void ChoreographDevApp::mouseDown( MouseEvent event )
{
}

void ChoreographDevApp::update()
{
    _anim.step( 1.0f / 60.0f );
}

void ChoreographDevApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );

  gl::disableDepthRead();
  gl::disableDepthWrite();

  gl::clear( Color( 0, 0, 0 ) );
  gl::color( Color( 1.0f, 0.0f, 0.0f ) );
  gl::drawSolidCircle( vec2( 200.0f, _ball_y ), 120.0f );
  gl::color( 0.0f, 0.0f, 1.0f );
  gl::drawSolidCircle( _ball_2, _ball_radius );
}

CINDER_APP_NATIVE( ChoreographDevApp, RendererGl )
