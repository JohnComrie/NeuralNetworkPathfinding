#include "DynamicShape.h"

namespace AIFramework
{
	void DynamicShape::SynchronisePosition()
	{
		rectangleShape.setPosition(sf::Vector2f((body->GetTransform().p.x * SCALING_FACTOR), (body->GetTransform().p.y * SCALING_FACTOR)));
	}
}