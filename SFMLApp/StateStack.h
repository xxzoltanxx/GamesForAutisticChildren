#pragma once

#include "States.h"
#include <vector>
#include <unordered_map>
#include <functional>

enum class Command
{
	None,
	PushGame,
	PushMain,
	Pop,
	PushGusjenica
};

class StateStack
{
public:
	StateStack(sf::RenderWindow* win);

	bool isEmpty() const
	{
		return mStates.empty();
	}

	void pushbackState(State::States state);
	void popState();
	void handleEvents(sf::Event& event);
	void update(float dt);
	void render(sf::RenderWindow& window);
	bool noCommand() const { return mCommand == Command::None;  }

	sf::RenderWindow* mWindow;
private:

	template <typename T> void registerState(State::States state)
	{
		mStateFactory.insert(std::make_pair(state, [](StateStack* stack) -> std::unique_ptr<State> { return std::unique_ptr<T>(new T(stack)); }));
	}

	std::vector<std::unique_ptr<State>> mStates;

	std::unordered_map<State::States, std::function<std::unique_ptr<State>(StateStack* stateStack)>> mStateFactory;
	Command mCommand = Command::None;
	
};

class TextureCache
{
public:
	enum Textures
	{
		Smiley,
		Sad,
		Seg1,
		Seg2,
		Seg3,
		Seg4,
		Seg5,
		Seg6
	};
	TextureCache();
	sf::Texture& const getTexture(TextureCache::Textures texture);

	static TextureCache* get();

private:
	std::unordered_map<Textures, std::unique_ptr<sf::Texture>> mTextures;

	static TextureCache* sInstance;
};