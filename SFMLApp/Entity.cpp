#include "Entity.h";
#include "States.h";

EntityScript::EntityScript(Type type, Entity* entity)
{
	mEntityScriptType = type;
	mEntity = entity;
}

void Entity::removeComponent(EntityScript* script)
{
	delete script;
	auto iter = std::find(mScripts.begin(), mScripts.end(), script);
	mScripts.erase(iter);
}

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
	RenderScript(EntityScript::Type::Rectangle, entity),
	mState(state)
{
	mShape.setFillColor(color);
	mShape.setPosition(position);
	mShape.setSize(size);
	mShape.setOrigin(size / 2.0f);
	onCreate();
}

void RectangleRenderScript::changeRotation(float rotation)
{
	mShape.setRotation(rotation);
}

void RectangleRenderScript::changePosition(const sf::Vector2f& position)
{
	mShape.setPosition(position);
}

float RectangleRenderScript::getRotation() const
{
	return mShape.getRotation();
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

bool RectangleRenderScript::containsPoint(const sf::Vector2f& point)
{
	return mShape.getGlobalBounds().contains(point);
}

BoxCollider::BoxCollider(b2World* world, RenderScript* object, float density, float friction, Entity* entity, bool isStatic)
	:EntityScript(EntityScript::Type::BoxCollider, entity)
{
	mWorld = world;
	b2BodyDef BodyDef;
	auto position = (object->getPosition()) / SCALE;
	BodyDef.position = b2Vec2(position.x, position.y);
	BodyDef.angle = object->getRotation() * b2_pi / 180.0f;
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
	onCreate();
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
	mWorld->DestroyBody(Body);
}

void BoxCollider::changePosition(const sf::Vector2f& newPos)
{
	sf::Vector2f position = (newPos) / SCALE;
	b2Vec2 posit = b2Vec2(position.x, position.y);
	Body->SetTransform(posit, Body->GetAngle());
}

MouseMoverScript::MouseMoverScript(Entity* entity, b2World* world):
	EntityScript(EntityScript::Type::MouseHandler, entity)
{
	mWorld = world;
	onCreate();
}

MouseMoverScript::~MouseMoverScript()
{
	onDestroy();
}

void MouseMoverScript::update(float dt)
{

}

void MouseMoverScript::onCreate()
{
	Subscription::get()->addSubscriber(this, ObserverMessageType::MouseMoved);
	Subscription::get()->addSubscriber(this, ObserverMessageType::MouseClicked);
	Subscription::get()->addSubscriber(this, ObserverMessageType::MouseReleased);
}

void MouseMoverScript::onDestroy()
{
	Subscription::get()->removeSubscriber(this, ObserverMessageType::MouseMoved);
	Subscription::get()->removeSubscriber(this, ObserverMessageType::MouseClicked);
	Subscription::get()->removeSubscriber(this, ObserverMessageType::MouseReleased);
}

void MouseMoverScript::notify(const ObserverMessage& msg)
{
	switch (msg.mType)
	{
	case ObserverMessageType::MouseMoved:
		if (selected)
		{
			((RenderScript*)mEntity->getComponent<RenderScript>())->changePosition(sf::Vector2f(msg.twoFloats.mFloat1, msg.twoFloats.mFloat2));
		}
		break;
	case ObserverMessageType::MouseClicked:
		if (((RenderScript*)mEntity->getComponent<RenderScript>())->containsPoint(sf::Vector2f(msg.twoFloats.mFloat1, msg.twoFloats.mFloat2)))
		{
			selected = true;
			mEntity->removeComponent(mEntity->getComponent<BoxCollider>());
		}
		else
		{
			selected = false;
		}
		break;
	case ObserverMessageType::MouseReleased:
		if (selected)
		{
			selected = false;
			EntityScript* collider = new BoxCollider(mWorld,(RenderScript*) mEntity->getComponent<RenderScript>(), 2.0f, 2.0f, mEntity, false);
			mEntity->addComponent(collider);
		}
	}
}