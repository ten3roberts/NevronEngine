#include "src/pch.h"
#include "Wave.h"

void Wave::Update()
{
	GetComponent<Transform>();
	transform->position.x = Math::SineWave(-1, 1, 1, Time::elapsedTime);
}
