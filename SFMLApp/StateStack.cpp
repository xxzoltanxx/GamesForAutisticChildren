#include "StateStack.h"

void StateStack::pushbackState(State::States state)
{
	if (state == State::MainMenu)
	{
		mCommand = Command::PushMain;
	}
	else
	{
		mCommand = Command::PushGame;
	}
}

void StateStack::popState()
{
	mCommand = Command::Pop;
}

StateStack::StateStack()
{
	registerState<GameState>(State::GameState);
	registerState<MainMenu>(State::MainMenu);
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
	else if (mCommand == Command::Pop)
	{
		mStates.pop_back();
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

	mTextures[Textures::Smiley]->loadFromFile("smiley.png", sf::IntRect(0, 0, 410, 420));
	mTextures[Textures::Sad]->loadFromFile("smiley.png", sf::IntRect(410, 0, 410, 420));
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