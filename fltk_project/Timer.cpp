#include "Timer.h""
#include <Fl/Fl_Timer.H>
#include <iostream>

Timer::Timer(double _duration):
	m_duration(_duration)
{
	Fl::add_timeout(m_duration, tick, this);
}

void Timer::tick(void* _userdata)
{
	Timer* timer = (Timer*)_userdata;
	timer->on_tick();
	Fl::repeat_timeout(timer->m_duration,tick, timer);
}

Timer::~Timer()
{
	Fl::remove_timeout(tick,this);
}