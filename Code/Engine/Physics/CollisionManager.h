#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

struct CollisionEntry;
class Entity;

using TCollisionCallbackSignature = std::function<void(const CollisionEntry&, const CollisionEntry&, sf::Vector2f)>;

enum class EShapeType
{
	Circle,
	Box
};

struct CollisionShape 
{
	EShapeType type = EShapeType::Circle;
	float radius = 0.f;
	float width = 0.f;
	float height = 0.f;
};

struct CollisionEntry
{
	int id = 0;
	bool isDeleted = false;
	bool isStatic = false;

	sf::Vector2f position;
	CollisionShape shape;

	bool isTriggerVolume = true;
	TCollisionCallbackSignature callback;

	Entity* pEntity = nullptr;
};

class CollisionManager
{
public:

	int RegisterShape(Entity* pOwner, const CollisionShape& shape, sf::Vector2f position, bool isStatic, bool isTriggerVolume, const TCollisionCallbackSignature& callback);
	bool UnregisterShape(int id);

	void UpdateShapePosition(int id, sf::Vector2f newPosition);

	void Update(float deltaSeconds);

protected:

	void HandleCollision_Circle_Circle(CollisionEntry& lhs, CollisionEntry& rhs);
	void HandleCollision_Circle_Box(CollisionEntry& lhs, CollisionEntry& rhs);
	void HandleCollision_Box_Box(CollisionEntry& lhs, CollisionEntry& rhs);

	sf::Vector2f GetCircleBoxSolveDirection(sf::Vector2f difference) const;

	CollisionEntry* FindCollisionEntryById(int id, size_t& outIndex);

	std::vector<CollisionEntry> m_shapes;
	int m_idCounter = 0;

	std::vector<size_t> m_deletedShapeIndices;
	bool m_isIteratingShapes = false;
};