#include "src/pch.h"
#include "Wave.h"

void Wave::Update()
{
	GetComponent<Transform>()->position = Math::SineWave(-1, 1, 1, Time::elapsedTime);
}
