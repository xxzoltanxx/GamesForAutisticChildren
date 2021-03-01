#include "Entity.h";
#include "States.h";

void Entity::update(float dt)
{
	for (auto& a : mScripts)
	{
		a->update(dt);
	}
}

sf::Vector2f RectangleRenderScript::getSize() const
{
	return mShape.getSize();
}

sf::Vector2f RectangleRenderScript::getPosition() const
{
	return mShape.getPosition();
}

RectangleRenderScript::RectangleRenderScript(sf::Vector2f size, sf::Color color, const sf::Vector2f& position, Entity* entity, State* state)
	:
	RenderScript(EntityScript::Rectangle, entity),
	mState(state)
{
	mShape.setFillColor(color);
	mShape.setPosition(position);
	mShape.setSize(size);
	mShape.setOrigin(size / 2.0f);
}

void RectangleRenderScript::changeRotation(float rotation)
{
	mShape.setRotation(rotation);
}

void RectangleRenderScript::changePosition(const sf::Vector2f& position)
{
	mShape.setPosition(position);
}

void RectangleRenderScript::update(float dt)
{

}

void RectangleRenderScript::onCreate()
{
	mState->addDrawable(this);
}

void RectangleRenderScript::onDestroy()
{
	mState->removeDrawable(this);
}

void RectangleRenderScript::notify(const ObserverMessage& msg)
{

}

void RectangleRenderScript::draw(sf::RenderWindow& window)
{
	window.draw(mShape);
}

BoxCollider::BoxCollider(b2World* world, RenderScript* object, float density, float friction, Entity* entity, bool isStatic)
	:EntityScript(EntityScript::BoxCollider, entity)
{
	b2BodyDef BodyDef;
	auto position = (object->getPosition()) / SCALE;
	BodyDef.position = b2Vec2(position.x, position.y);
	if (isStatic)
	{
		BodyDef.type = b2_staticBody;
	}
	else
	{
		BodyDef.type = b2_dynamicBody;
	}
	Body = world->CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox(object->getSize().x / 2.0f / SCALE, object->getSize().y / 2.0f / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.friction = friction;
	FixtureDef.density = 1.f;
	FixtureDef.shape = &Shape;


	Body->CreateFixture(&FixtureDef);
}

void BoxCollider::update(float dt)
{
	RenderScript* renderScript = (RenderScript*)mEntity->getComponent<RenderScript>();
	renderScript->changePosition(sf::Vector2f((Body->GetPosition().x) * SCALE, Body->GetPosition().y * SCALE));
	renderScript->changeRotation(Body->GetAngle() * 180.0f / b2_pi);
}

void BoxCollider::onCreate()
{

}

void BoxCollider::onDestroy()
{

}

void BoxCollider::notify(const ObserverMessage& msg)
{

}