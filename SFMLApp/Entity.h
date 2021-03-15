#pragma once

#include <unordered_map>
#include "Subcriber.h";
#include <SFML/Graphics.hpp>;
#include <box2d.h>;
#include "Utils.h";

class EntityScript;
class State;

class Entity
{
public:
	template <typename T> EntityScript* getComponent()
	{
		for (auto& script : mScripts)
		{
			if (dynamic_cast<T*>(script) != nullptr)
			{
				return static_cast<T*>(script);
			}
		}
		return nullptr;
	}

	void addComponent(EntityScript* script)
	{
		mScripts.push_back(script);
	}

	void removeComponent(EntityScript* script);
	void update(float dt);
private:
	std::vector<EntityScript*> mScripts;
};

//Unity-like abomination below

class EntityScript : public Observer
{
public:
	enum class Type
	{
		None,
		MouseHandler,
		Rectangle,
		BoxCollider
	};
	EntityScript(Type type, Entity* entity);
	virtual ~EntityScript() {}
	virtual void update(float dt) = 0;
protected:
	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;
	Type mEntityScriptType = Type::None;
	Entity* mEntity;
};

class RenderScript : public EntityScript
{
public:
	using EntityScript::EntityScript;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void changePosition(const sf::Vector2f& position) = 0;
	virtual void changeRotation(float rotation) = 0;
	virtual void changeColor(const sf::Color& col) = 0;
	virtual bool containsPoint(const sf::Vector2f& point) = 0;
	virtual sf::Color getColor() const = 0;
	virtual sf::Vector2f getSize() const = 0;
	virtual sf::Vector2f getPosition() const = 0;
	virtual float getRotation() const = 0;
};

class RectangleRenderScript : public RenderScript
{
public:
	RectangleRenderScript(sf::Vector2f size, sf::Color color, const sf::Vector2f& position, Entity* entity, State* state);
	virtual ~RectangleRenderScript()
	{
		onDestroy();
	}
	virtual void changeColor(const sf::Color& col);
	bool containsPoint(const sf::Vector2f& point) override;
	void update(float dt) override;
	void onCreate() override;
	sf::Color getColor() const override;
	void onDestroy() override;
	void notify(const ObserverMessage& msg) override;
	void draw(sf::RenderWindow& window) override;
	void changePosition(const sf::Vector2f& position) override;
	void changeRotation(float rotation) override;
	float getRotation() const override;
	sf::Vector2f getPosition() const override;
	sf::Vector2f getSize() const override;
private:
	sf::RectangleShape mShape;
	State* mState;
};

class BoxCollider : public EntityScript
{
public:
	BoxCollider(b2World* world, RenderScript* object, float density, float friction, Entity* entity, bool isStatic = false);
	virtual ~BoxCollider()
	{
		onDestroy();
	}
	void update(float dt) override;
	void onCreate() override;
	void onDestroy() override;
	void notify(const ObserverMessage& msg) override { }
	void changePosition(const sf::Vector2f& newPos);
private:
	b2Body* Body;
	b2World* mWorld;
};

class MouseMoverScript : public EntityScript
{
public:
	MouseMoverScript(Entity* entity, b2World* world);
	virtual ~MouseMoverScript();
	void update(float dt) override;
	void onCreate() override;
	void onDestroy() override;
	void notify(const ObserverMessage& msg) override;
private:
	bool selected = false;
	b2World* mWorld;
};

class Fadeable : public EntityScript
{
public:
	Fadeable(Entity* entity, int target);
	virtual ~Fadeable()
	{
		onDestroy();
	}
	void update(float dt) override;
	void onCreate() override;
	void onDestroy() override;
	void notify(const ObserverMessage& msg) override;
private:
	float interpolation = 0.0f;
	int mTarget = -1;
	bool targetReached = false;
};

class Finishable : public EntityScript
{
public:
	Finishable(Entity* entity, int position);
	virtual ~Finishable()
	{
		onDestroy();
	}
	void update(float dt) override;
	void onCreate() override {};
	void onDestroy() override {};
	void notify(const ObserverMessage& msg) override {}
private:
	int mPosition;
	bool mIsInPosition = false;
	bool mDonePositionStep = false;
	bool mDoneOutOfPositionStep = false;

};