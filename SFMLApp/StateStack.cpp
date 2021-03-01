#include "StateStack.h"

void StateStack::pushbackState(State::States state)
{
	if (state == State::MainMenu)
	{
		mCommand = Command::PushMain;
	}
	else if (state == State::GameState)
	{
		mCommand = Command::PushGame;
	}
	else if (state == State::SnakeState)
	{
		mCommand = Command::PushSnake;
	}
	else if (state == State::PhysicsGame)
	{
		mCommand = Command::PushPhysics;
	}
	else
	{
		mCommand = Command::PushGusjenica;
	}
}

void StateStack::popState()
{
	mCommand = Command::Pop;
}

StateStack::StateStack(sf::RenderWindow* window)
{
	mWindow = window;
	registerState<GameState>(State::GameState);
	registerState<MainMenu>(State::MainMenu);
	registerState<GusjenicaState>(State::Gusjenica);
	registerState<SnakeState>(State::SnakeState);
	registerState<PhysicsWorld>(State::PhysicsGame);
}

void StateStack::render(sf::RenderWindow& window)
{
	for (std::vector<std::unique_ptr<State>>::iterator iter = mStates.end(); iter != mStates.begin();)
	{
		--iter;
		(*iter)->draw(window);
		if (!(*iter)->isTranslucent())
		{
			break;
		}
	}
}

void StateStack::update(float dt)
{
	for (std::vector<std::unique_ptr<State>>::iterator iter = mStates.end(); iter != mStates.begin();)
	{
		--iter;
		(*iter)->update(dt);
		if (!(*iter)->isTranscendent())
		{
			break;
		}
	}
	if (mCommand == Command::PushGame)
	{
		mStates.push_back(std::move(mStateFactory[State::GameState](this)));
	}
	else if (mCommand == Command::PushMain)
	{
		mStates.push_back(std::move(mStateFactory[State::MainMenu](this)));
	}
	else if (mCommand == Command::PushGusjenica)
	{
		mStates.push_back(std::move(mStateFactory[State::Gusjenica](this)));
	}
	else if (mCommand == Command::PushSnake)
	{
		mStates.push_back(std::move(mStateFactory[State::SnakeState](this)));
	}
	else if (mCommand == Command::Pop)
	{
		mStates.pop_back();
	}
	else if (mCommand == Command::PushPhysics)
	{
		mStates.push_back(std::move(mStateFactory[State::PhysicsGame](this)));
	}
	mCommand = Command::None;
}

void StateStack::handleEvents(sf::Event& event)
{
	for (std::vector<std::unique_ptr<State>>::iterator iter = mStates.end(); iter != mStates.begin();)
	{
		--iter;
		(*iter)->handleEvent(event);
		if (!(*iter)->isTranscendent())
		{
			break;
		}
	}
}

sf::Texture& const TextureCache::getTexture(const TextureCache::Textures texturee)
{
	return (*mTextures[texturee]);
}

TextureCache::TextureCache()
{
	mTextures.insert(std::make_pair(Textures::Smiley, std::unique_ptr<sf::Texture>(new sf::Texture())));
	mTextures.insert(std::make_pair(Textures::Sad, std::unique_ptr<sf::Texture>(new sf::Texture())));
	mTextures.insert(std::make_pair(Textures::Seg1, std::unique_ptr<sf::Texture>(new sf::Texture())));
	mTextures.insert(std::make_pair(Textures::Seg2, std::unique_ptr<sf::Texture>(new sf::Texture())));
	mTextures.insert(std::make_pair(Textures::Seg3, std::unique_ptr<sf::Texture>(new sf::Texture())));
	mTextures.insert(std::make_pair(Textures::Seg4, std::unique_ptr<sf::Texture>(new sf::Texture())));
	mTextures.insert(std::make_pair(Textures::Seg5, std::unique_ptr<sf::Texture>(new sf::Texture())));
	mTextures.insert(std::make_pair(Textures::Seg6, std::unique_ptr<sf::Texture>(new sf::Texture())));

	mTextures[Textures::Smiley]->loadFromFile("smiley.png", sf::IntRect(0, 0, 410, 420));
	mTextures[Textures::Sad]->loadFromFile("smiley.png", sf::IntRect(410, 0, 410, 420));
	mTextures[Textures::Seg1]->loadFromFile("6.png");
	mTextures[Textures::Seg2]->loadFromFile("5.png");
	mTextures[Textures::Seg3]->loadFromFile("4.png");
	mTextures[Textures::Seg4]->loadFromFile("3.png");
	mTextures[Textures::Seg5]->loadFromFile("2.png");
	mTextures[Textures::Seg6]->loadFromFile("1.png");
}

TextureCache* TextureCache::sInstance = nullptr;

TextureCache* TextureCache::get()
{
	if (sInstance == nullptr)
	{
		sInstance = new TextureCache();
	}
	return sInstance;
}