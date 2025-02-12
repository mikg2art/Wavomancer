#include "CollisionManager.h"

#include "Engine/SFMLMath/SFMLMath.hpp"
#include "Engine/EntitySystem/EntitySystemDefinitions.h"

#include <array>

int CollisionManager::RegisterShape(Entity* pOwner, const CollisionShape& shape, sf::Vector2f position, bool isStatic, bool isTriggerVolume, const TCollisionCallbackSignature& callback)
{
	m_idCounter++;

	CollisionEntry& entry = m_shapes.emplace_back();
	entry.id = m_idCounter;
	entry.pEntity = pOwner;
	entry.shape = shape;
	entry.position = position;
	entry.callback = callback;
	entry.isStatic = isStatic;
	entry.isTriggerVolume = isTriggerVolume;

	return entry.id;
}

bool CollisionManager::UnregisterShape(int id)
{
	size_t outEntryIndex = 0;
	if (CollisionEntry* pEntry = FindCollisionEntryById(id, outEntryIndex))
	{
		if (m_isIteratingShapes)
		{
			pEntry->isDeleted = true;
			m_deletedShapeIndices.push_back(outEntryIndex);
		}
		else
		{
			m_shapes.erase(m_shapes.begin() + outEntryIndex);
		}

		return true;
	}

	return false;
}

void CollisionManager::UpdateShapePosition(int id, sf::Vector2f newPosition)
{
	size_t outEntryIndex = 0;
	if (CollisionEntry* pEntry = FindCollisionEntryById(id, outEntryIndex))
	{
		pEntry->position = newPosition;
	}
}

void CollisionManager::Update(float deltaSeconds)
{
	// iterate over all shapes
	// for each shape, check for each OTHER shape whether they overlap
	// if so, invoke callback

	// ... if you have real collision resolving: Resolve collision

	m_isIteratingShapes = true;

	for (int i = 0; i < m_shapes.size(); i++)
	{
		CollisionEntry& lhs = m_shapes[i];

		if (lhs.isDeleted)
			continue;

		for (int k = i + 1; k < m_shapes.size(); k++)
		{
			CollisionEntry& rhs = m_shapes[k];

			if (rhs.isDeleted || i == k)
				continue;

			switch (lhs.shape.type)
			{
			case EShapeType::Circle:
			{
				switch (rhs.shape.type)
				{
				case EShapeType::Circle:
					HandleCollision_Circle_Circle(lhs, rhs);
					break;
				case EShapeType::Box:
					HandleCollision_Circle_Box(lhs, rhs);
					break;
				}
				break;
			}
			case EShapeType::Box:
			{
				switch (rhs.shape.type)
				{
				case EShapeType::Circle:
					HandleCollision_Circle_Box(rhs, lhs);
					break;
				case EShapeType::Box:
					HandleCollision_Box_Box(lhs, rhs);
					break;
				}
				break;
			}
			}
		}
	}

	m_isIteratingShapes = false;
}

void CollisionManager::HandleCollision_Circle_Circle(CollisionEntry& lhs, CollisionEntry& rhs)
{
	CollisionShape& lhsCircle = lhs.shape;
	CollisionShape& rhsCircle = rhs.shape;

	const sf::Vector2f lhsToRhs = lhs.position - rhs.position;
	const float distance = sf::getLength(lhsToRhs);
	const float distanceBetweenCircles = distance - (lhsCircle.radius + rhsCircle.radius);
	const sf::Vector2f normalizedLhsToRhs = sf::getNormalized(lhsToRhs);

	if (distanceBetweenCircles < 0.f)
	{
		sf::Vector2f resolveVectorLhs;
		sf::Vector2f resolveVectorRhs;

		if (lhs.isTriggerVolume || rhs.isTriggerVolume)
		{
			//Handle overlap
		}
		else if (!lhs.isTriggerVolume && !rhs.isTriggerVolume)
		{
			//Handle collision resolve
			if (lhs.isStatic != rhs.isStatic)
			{
				CollisionEntry& movableEntry = lhs.isStatic ? rhs : lhs;
				sf::Vector2f& resolveVector = lhs.isStatic ? resolveVectorRhs : resolveVectorLhs;
				resolveVector = (lhs.isStatic ? normalizedLhsToRhs : -normalizedLhsToRhs * distanceBetweenCircles);

				movableEntry.pEntity->SetPosition(movableEntry.position + resolveVector);
			}
			else if (!lhs.isStatic && !rhs.isStatic)
			{
				//Both movable move away from each other
				resolveVectorLhs = normalizedLhsToRhs * distanceBetweenCircles * -0.5f;
				resolveVectorRhs = normalizedLhsToRhs * distanceBetweenCircles * 0.5f;

				lhs.pEntity->SetPosition(lhs.position + resolveVectorLhs);
				rhs.pEntity->SetPosition(rhs.position + resolveVectorRhs);
			}
			else
			{
				//Both are static, do nothing (both immovable)
			}
		}

		if (lhs.callback)
			lhs.callback(lhs, rhs, resolveVectorLhs);

		if (rhs.callback)
			rhs.callback(rhs, lhs, resolveVectorRhs);
	}
}

void CollisionManager::HandleCollision_Circle_Box(CollisionEntry& lhs, CollisionEntry& rhs)
{
	CollisionShape& lhsCircle = lhs.shape;
	CollisionShape& rhsBox = rhs.shape;

	// get center point circle first 
	// calculate AABB info (center, half-extents)
	sf::Vector2f aabb_half_extents(rhsBox.width * 0.5f, rhsBox.height * 0.5f);
	// get difference vector between both centers
	sf::Vector2f difference = lhs.position - rhs.position;

	sf::Vector2f clamped;
	clamped.x = std::clamp(difference.x, -aabb_half_extents.x, aabb_half_extents.x);
	clamped.y = std::clamp(difference.y, -aabb_half_extents.y, aabb_half_extents.y);

	// now that we know the clamped values, add this to AABB_center and we get the value of box closest to circle
	sf::Vector2f closest = rhs.position + clamped;

	// now retrieve vector between center circle and closest point AABB and check if length < radius
	difference = closest - lhs.position;

	if (difference.x == 0.f && difference.y == 0.f)
		return;

	const float diffLength = sf::getLength(difference);

	if (diffLength < lhsCircle.radius)
	{
		const float penetrationDistance = lhsCircle.radius - diffLength;

		sf::Vector2f resolveVectorLhs;
		sf::Vector2f resolveVectorRhs;

		if (lhs.isTriggerVolume || rhs.isTriggerVolume)
		{
			//Handle overlap
		}
		else if (!lhs.isTriggerVolume && !rhs.isTriggerVolume)
		{
			//Handle collision resolve
			if (lhs.isStatic != rhs.isStatic)
			{
				//Both movable move away from each other
				sf::Vector2f& resolveVector = lhs.isStatic ? resolveVectorRhs : resolveVectorLhs;
				CollisionEntry& movableEntry = lhs.isStatic ? rhs : lhs;
				resolveVector = GetCircleBoxSolveDirection(difference) * penetrationDistance;

				movableEntry.pEntity->SetPosition(movableEntry.position - resolveVector);
			}
			else if (!lhs.isStatic && !rhs.isStatic)
			{
				//Both movable move away from each other
				sf::Vector2f solveDirection = GetCircleBoxSolveDirection(difference);

				resolveVectorLhs = solveDirection * penetrationDistance * -0.5f;
				resolveVectorRhs = solveDirection * penetrationDistance * 0.5f;

				lhs.pEntity->SetPosition(lhs.position + resolveVectorLhs);
				rhs.pEntity->SetPosition(rhs.position + resolveVectorRhs);
			}
			else
			{
				//Both are static, do nothing (both immovable)
			}
		}

		if (lhs.callback)
			lhs.callback(lhs, rhs, resolveVectorLhs);

		if (rhs.callback)
			rhs.callback(rhs, lhs, resolveVectorRhs);
	}
}

void CollisionManager::HandleCollision_Box_Box(CollisionEntry& lhs, CollisionEntry& rhs)
{
	// to be implemented
}

sf::Vector2f CollisionManager::GetCircleBoxSolveDirection(sf::Vector2f difference) const
{
	static const std::array<sf::Vector2f, 4> dirs = {
		sf::Vector2f(0.0f, 1.0f),	// up
		sf::Vector2f(1.0f, 0.0f),	// right
		sf::Vector2f(0.0f, -1.0f),	// down
		sf::Vector2f(-1.0f, 0.0f)	// left
	};

	float max = 0.0f;
	unsigned int bestMatch = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = sf::dot(sf::getNormalized(difference), dirs[i]);
		if (dot_product > max)
		{
			max = dot_product;
			bestMatch = i;
		}
	}

	return dirs[bestMatch];
}

CollisionEntry* CollisionManager::FindCollisionEntryById(int id, size_t& outIndex)
{
	for (int i = 0; i < m_shapes.size(); i++)
	{
		if (m_shapes[i].id == id)
		{
			outIndex = i;
			return &m_shapes[i];
		}
	}

	return nullptr;
}
