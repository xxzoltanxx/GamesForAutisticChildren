#include "States.h"
#include "StateStack.h"
#include "Utils.h"

State::State(bool isTranslucent, bool isTranscendent, StateStack* stateStack)
	:mIsTranslucent(isTranslucent),
	mIsTranscendent(isTranscendent),
	mStateStack(stateStack)
{

}

MainMenu::MainMenu(StateStack* stateStack)
	:State(false, false, stateStack)
{
	mSmiley = sf::Sprite(TextureCache::get()->getTexture(TextureCache::Smiley));
	mSmiley.setScale(sf::Vector2f(0.5f, 0.5f));
	mSmiley.setPosition(sf::Vector2f(200, 0));
}

void MainMenu::update(float dt)
{

}

void MainMenu::draw(sf::RenderWindow& window)
{
	window.draw(mSmiley);
}

void MainMenu::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		mStateStack->popState();
	}
	if (event.type == sf::Event::KeyPressed)
	{
		mStateStack->pushbackState(State::GameState);
	}
}

void GameState::draw(sf::RenderWindow& window)
{
	window.draw(mCircleCurrent);
	window.draw(mCircleGoal);
	window.draw(mSmiley);
	window.draw(mSadFace);
}

GameState::GameState(StateStack* stateStack) :
	State(false, false, stateStack)
{
	srand(time(nullptr));
	mCircleGoal = sf::CircleShape(100.0f, 300);
	mCircleCurrent = sf::CircleShape(100.0f, 300);

	mCircleGoal.setOrigin(100.0f, 100.0f);
	mCircleCurrent.setOrigin(100.0f, 100.0f);

	mCircleCurrent.setPosition(160, 240);
	mCircleGoal.setPosition(440, 240);

	mSmiley = sf::Sprite(TextureCache::get()->getTexture(TextureCache::Textures::Smiley));
	mSadFace = sf::Sprite(TextureCache::get()->getTexture(TextureCache::Textures::Sad));

	mSmiley.setScale(sf::Vector2f(0.5, 0.5));
	mSadFace.setScale(sf::Vector2f(0.5, 0.5));

	mSmiley.setPosition(sf::Vector2f(200, 0));
	mSadFace.setPosition(sf::Vector2f(200, 0));

	mSmiley.setColor(sf::Color(0, 0, 0, 0));
	mSadFace.setColor(sf::Color(0, 0, 0, 0));
	setNextGoal();
}

void GameState::setNextGoal()
{
	for (int i = 0; i < colorsToPick; ++i)
	{
		mRed = rand() % 255;
		mGreen = rand() % 255;
		mBlue = rand() % 255;

		mGuessColors[i] = sf::Color(mRed, mGreen, mBlue);
	}

	int goal = rand() % colorsToPick;

	mRed = mGuessColors[goal].r;
	mBlue = mGuessColors[goal].b;
	mGreen = mGuessColors[goal].g;

	mCurrentColor = 0;
	mCircleGoal.setFillColor(sf::Color(mRed, mGreen, mBlue));
	mCircleCurrent.setFillColor(sf::Color(mGuessColors[0].r, mGuessColors[0].g, mGuessColors[0].b));

	mInterpolation = 0.0f;
}

void GameState::update(float dt)
{
	if (mLocked)
	{
		mInterpolationSize += dt;
		if (mGuess == Guess::Right)
		{
			mSmiley.setColor(sf::Color(255, 255, 255, mInterpolationSize * 255.0));
		}
		else
		{
			mSadFace.setColor(sf::Color(255, 255, 255, mInterpolationSize * 255.0));
		}

		mCircleCurrent.setScale(sf::Vector2f(lerp(mSizeCircle, mGoalCircle, mInterpolationSize) / 100.0f, lerp(mSizeCircle, mGoalCircle, mInterpolationSize) / 100.0f));
		if (mInterpolationSize >= 1.0f)
		{
			if (mGoalCircle == 0.0f)
			{
				mStateStack->popState();
			}
			mSmiley.setColor(sf::Color(0, 0, 0, 0));
			mSadFace.setColor(sf::Color(0, 0, 0, 0));
			mSizeCircle = mGoalCircle;
			mLocked = false;
			mGuess = Guess::None;
		}
	}
	else
	{
		if (mInterpolatingUpwards)
		{
			mInterpolation += dt / mFactor;
			mRedInterpolated = mGuessColors[mCurrentColor].r;
			mGreenInterpolated = mGuessColors[mCurrentColor].g;
			mBlueInterpolated = mGuessColors[mCurrentColor].b;
			if (mInterpolation > 1.0f)
			{
				mInterpolation = 0.0f;
				mCurrentColor = ++mCurrentColor % colorsToPick;
			}
		}
		mCircleCurrent.setFillColor(sf::Color(mRedInterpolated, mGreenInterpolated, mBlueInterpolated));
	}

}

void GameState::guessColor()
{
	if (mLocked) return;
	mLocked = true;
	if (std::abs(mRed - mRedInterpolated) < guessThreshold && (mBlue - mBlueInterpolated) < guessThreshold && (mGreen - mGreenInterpolated) < guessThreshold)
	{
		mInterpolationSize = 0.0f;
		mGuess = Guess::Right;
		mFactor = mFactor <= 1 ? 1 : mFactor - 0.1;
		mGoalCircle = mGoalCircle >= 100.0f ? 100.0f : mGoalCircle + 10.0f;
		setNextGoal();
	}
	else
	{
		mFactor += 0.1;
		mInterpolationSize = 0.0f;
		mGoalCircle -= 10.0f;
		mGuess = Guess::Wrong;
	}
}

void GameState::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Space)
		{
			guessColor();
		}
	}
}