#pragma once
struct Timer
{
	Timer(double _duration);
	virtual ~Timer();

	virtual void on_tick() = 0;
private:
	void* m_userdata;
	double m_duration;

	static void tick(void* _userdata);
};