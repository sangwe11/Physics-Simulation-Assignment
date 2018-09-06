#ifndef _TIME_H
#define _TIME_H

class Time
{
public:
	Time()
	{

	}

	~Time()
	{

	}

	Time(Time const &) = delete;
	void operator=(Time const &) = delete;

	void start();
	void end();

	const float &delta_time() { return m_delta; }

	const float elapsed() { return m_last / 1000.0f; }

	void update();

private:
	float m_delta = 0.0f;
	unsigned int m_last = 0;
};

#endif