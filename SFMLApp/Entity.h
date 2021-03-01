#pragma once

#include <unordered_map>
#include "Subcriber.h";
#include "States.h";
#include <SFML/Graphics.hpp>;

class EntityScript;

class Entity
{
public:
	template <typename T> EntityScript* getComponent()
	{
		for (auto& script : mScripts)
		{
			if (dynamic_cast<T>(script) != nullptr)
			{
				return static_cast<T>(script);
			}
		}
		return nullptr;
	}
	void addComponent(EntityScript* script)
	{
		mScripts.push_back(script);
	}
	void update(float dt);
private:
	std::vector<EntityScript*> mScripts;
};

//Unity-like abomination below

class EntityScript : Observer
{
public:
	enum Type
	{
		None,
		MouseHandler,
		Rectangle
	};
	EntityScript(Type type, Entity* entity)
	{
		mEntityScriptType = type;
		mEntity = entity;
		onCreate();
	}
	virtual ~EntityScript()
	{
		onDestroy();
	}
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
};

class RectangleRenderScript : public RenderScript
{
public:
	RectangleRenderScript(sf::Vector2f size, sf::Color color, const sf::Vector2f& position, Entity* entity, State* state);
	void update(float dt) override;
	void onCreate() override;
	void onDestroy() override;
	void notify(const ObserverMessage& msg) override;
	void draw(sf::RenderWindow& window) override;
	void changePosition(const sf::Vector2f& position) override;
private:
	sf::RectangleShape mShape;
	State* mState;
};