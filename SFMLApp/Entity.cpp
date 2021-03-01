#include "Entity.h"

void Entity::update(float dt)
{
	for (auto& a : mScripts)
	{
		a->update(dt);
	}
}


RectangleRenderScript::RectangleRenderScript(sf::Vector2f size, sf::Color color, const sf::Vector2f& position, Entity* entity, State* state)
	:
	mState(state),
	RenderScript(EntityScript::Rectangle, entity)
{
	mShape.setFillColor(color);
	mShape.setPosition(position);
	mShape.setSize(size);
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

