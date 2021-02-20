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
		if (event.key.code == sf::Keyboard::Num1)
		{
			mStateStack->pushbackState(State::GameState);
		}
		else if (event.key.code == sf::Keyboard::Num2)
		{
			mStateStack->pushbackState(State::Gusjenica);
		}
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
		if (event.key.code == sf::Keyboard::Escape)
		{
			mStateStack->popState();
		}
		if (event.key.code == sf::Keyboard::Space)
		{
			guessColor();
		}
	}
}

void GusjenicaState::update(float dt)
{

}

void GusjenicaState::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			mStateStack->popState();
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			auto coords = sf::Mouse::getPosition(*mStateStack->mWindow);
			int index = (coords.x - (320 - mDifficulty / 2.0f * 50)) / 50.0f;
			bool inside = (coords.y > 430.0f) && (coords.y < 480.0f);

			if (index >= 0 && index < mDifficulty && inside)
			{
				pickColor(mPallete[index].getFillColor());
			}
		}
	}
}

void GusjenicaState::colorSegments()
{
	for (int i = 0; i < 6; ++i)
	{
		sf::Color color = sf::Color(rand() % 255, rand() % 255, rand() % 255);
		mColored[i].color(color);
	}
}

void GusjenicaState::setPallete(const sf::Color& color)
{
	mPallete.clear();
	for (int i = 0; i < mDifficulty; ++i)
	{
		mPallete.push_back(sf::RectangleShape(sf::Vector2f(50, 50)));
		mPallete.back().setPosition(320 - mDifficulty / 2.0f * 50 + 50 * i, 430);
		mPallete.back().setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
	}
	mPallete[rand() % mDifficulty].setFillColor(color);
}

GusjenicaState::GusjenicaState(StateStack* stateStack)
	: State(false, false, stateStack) {
	for (int i = 0; i < 6; ++i)
	{
		mUncolored.push_back(Segment(Segment::SegmentType(i)));
		mUncolored.back().addToPosition(sf::Vector2f(50, 200));
	}
	for (int i = 0; i < 6; ++i)
	{
		mColored.push_back(Segment(Segment::SegmentType(i)));
		mColored.back().addToPosition(sf::Vector2f(50, 0));
	}
	mSmiley.setTexture(TextureCache::get()->getTexture(TextureCache::Smiley));
	mSadFace.setTexture(TextureCache::get()->getTexture(TextureCache::Sad));
	mSmiley.setColor(sf::Color(0, 0, 0, 0));
	mSadFace.setColor(sf::Color(0, 0, 0, 0));
	mSmiley.setScale(0.2f, 0.2f);
	mSadFace.setScale(0.2f, 0.2f);
	mSmiley.setPosition(260, 300);
	mSadFace.setPosition(260, 300);
	colorSegments();
	setPallete(mColored[mCurrentSegment].getColor());
}

void GusjenicaState::pickColor(const sf::Color& color)
{
	if (mColored[mCurrentSegment].getColor() == color)
	{
		mUncolored[mCurrentSegment++].color(color);
		++mDifficulty;
		if (mCurrentSegment == 6)
		{
			mSmiley.setColor(sf::Color(255, 255, 255, 255));
			mPallete.clear();
			gameOver = true;
		}
		else
		{
			setPallete(mColored[mCurrentSegment].getColor());
			gameOver = true;
		}
	}
	else
	{
		mPallete.clear();
		mSadFace.setColor(sf::Color(255, 255, 255, 255));
	}
}

void GusjenicaState::draw(sf::RenderWindow& window)
{
	for (auto& segment : mUncolored)
	{
		segment.draw(window);
	}
	for (auto& segment : mColored)
	{
		segment.draw(window);
	}
	for (auto& pallete : mPallete)
	{
		window.draw(pallete);
	}
	window.draw(mSmiley);
	window.draw(mSadFace);
}

void Segment::draw(sf::RenderWindow& window)
{
	window.draw(mSegmentSprite);
}

Segment::Segment(SegmentType type)
{
	switch (type)
	{
	case First:
		mSegmentSprite.setTexture(TextureCache::get()->getTexture(TextureCache::Seg1));
		mSegmentSprite.setPosition(50, 50);
		break;
	case Second:
		mSegmentSprite.setTexture(TextureCache::get()->getTexture(TextureCache::Seg2));
		mSegmentSprite.setPosition(100, 50);
		break;
	case Third:
		mSegmentSprite.setTexture(TextureCache::get()->getTexture(TextureCache::Seg3));
		mSegmentSprite.setPosition(160, 50);
		break;
	case Fourth:
		mSegmentSprite.setTexture(TextureCache::get()->getTexture(TextureCache::Seg4));
		mSegmentSprite.setPosition(215, 50);
		break;
	case Fifth:
		mSegmentSprite.setTexture(TextureCache::get()->getTexture(TextureCache::Seg5));
		mSegmentSprite.setPosition(270, 36);
		break;
	case Sixth:
		mSegmentSprite.setTexture(TextureCache::get()->getTexture(TextureCache::Seg6));
		mSegmentSprite.setPosition(335, -20);
		break;

	}
}

void Segment::addToPosition(const sf::Vector2f& position)
{
	mSegmentSprite.setPosition(mSegmentSprite.getPosition() + position);
}

void Segment::color(const sf::Color& color)
{
	mSegmentSprite.setColor(color);
}