#include "States.h"
#include "StateStack.h"
#include "Subcriber.h"
#include "Utils.h"
#include "Entity.h"

constexpr int gameArea = 20;
constexpr float snakeFrameTime = 0.5f;

State::State(bool isTranslucent, bool isTranscendent, StateStack* stateStack)
	:mIsTranslucent(isTranslucent),
	mIsTranscendent(isTranscendent),
	mStateStack(stateStack)
{

}

void State::addDrawable(RenderScript* renderScript)
{
	mDrawables.push_back(renderScript);
}

void State::removeDrawable(RenderScript* renderScript)
{
	auto iter = std::find(mDrawables.begin(), mDrawables.end(), renderScript);
	mDrawables.erase(iter);
}

void State::update(float dt)
{
	for (auto entity = mEntities.begin(); entity != mEntities.end(); ++entity)
	{
		(*entity)->update(dt);
	}
}

//The basic states don't even use this
void State::draw(sf::RenderWindow& window)
{
	for (auto iter = mDrawables.begin(); iter != mDrawables.end(); ++iter)
	{
		(*iter)->draw(window);
	}
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
		else if (event.key.code == sf::Keyboard::Num3)
		{
			mStateStack->pushbackState(State::SnakeState);
		}
		else if (event.key.code == sf::Keyboard::Num4)
		{
			mStateStack->pushbackState(State::PhysicsGame);
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

void SnakeState::draw(sf::RenderWindow& window)
{
	for (SnakeSegment* segment : mSnakeSegments)
	{
		window.draw(segment->mRectangle);
	}
	window.draw(mFruit.mRectangle);
}

void SnakeState::gameOver()
{
	for (SnakeSegment* segment : mSnakeSegments)
	{
		segment->mRectangle.setFillColor(sf::Color::Blue);
	}
	mGameOver = true;
}

void SnakeState::update(float dt)
{
	if (mGameOver)
	{
		return;
	}

	mElapsedTime += sf::seconds(dt);
	if (mElapsedTime > sf::seconds(snakeFrameTime))
	{
		mElapsedTime = sf::seconds(0);
		mSnakeSegments[0]->mCurrentDirection = mNextDirection;

		SnakeSegment::Direction previousDir = mNextDirection;
		sf::Vector2i previousPosition;
		for (SnakeSegment* seg : mSnakeSegments)
		{
			previousPosition = seg->mPosition;
			seg->updatePosition();
			SnakeSegment::Direction temp = seg->mCurrentDirection;
			seg->mCurrentDirection = previousDir;
			previousDir = temp;
		}
		if (mSnakeSegments[0]->mPosition == mFruit.mPosition)
		{
			mSnakeSegments.push_back(new SnakeSegment(previousDir, previousPosition));
			mFruit = Fruit(sf::Vector2i(rand() % gameArea, rand() % gameArea));
		}
		if (mSnakeSegments[0]->mPosition.x < 0 || mSnakeSegments[0]->mPosition.x > gameArea || mSnakeSegments[0]->mPosition.y < 0 || mSnakeSegments[0]->mPosition.y > gameArea)
		{
			gameOver();
			return;
		}
		for (int i = 1; i < mSnakeSegments.size(); ++i)
		{
			if (mSnakeSegments[0]->mPosition == mSnakeSegments[i]->mPosition)
			{
				gameOver();
			}
		}
	}
}

void SnakeState::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			mStateStack->popState();
		}
		else if (event.key.code == sf::Keyboard::Up)
		{
			mNextDirection = SnakeSegment::Up;
		}
		else if (event.key.code == sf::Keyboard::Left)
		{
			mNextDirection = SnakeSegment::Left;
		}
		else if (event.key.code == sf::Keyboard::Right)
		{
			mNextDirection = SnakeSegment::Right;
		}
		else if (event.key.code == sf::Keyboard::Down)
		{
			mNextDirection = SnakeSegment::Down;
		}

	}
}

SnakeState::SnakeState(StateStack* stateStack)
	:State(false, false, stateStack),
	mFruit(sf::Vector2i(rand() % gameArea, rand() % gameArea))
{
	mSnakeSegments.push_back(new SnakeSegment(SnakeSegment::Up, sf::Vector2i(10, 10)));
	mFruit = Fruit(sf::Vector2i(rand() % gameArea, rand() % gameArea));
}

void SnakeSegment::updatePosition()
{
	switch (mCurrentDirection)
	{
	case SnakeSegment::Left:
		mPosition.x -= 1;
		break;
	case SnakeSegment::Right:
		mPosition.x += 1;
		break;
	case SnakeSegment::Up:
		mPosition.y -= 1;
		break;
	case SnakeSegment::Down:
		mPosition.y += 1;
		break;
	}
	mRectangle.setPosition(mPosition.x * 32, mPosition.y * 24);
}

SnakeSegment::SnakeSegment(Direction direction, const sf::Vector2i& position)
	:mCurrentDirection(direction),
	mPosition(position),
	mRectangle(sf::Vector2f(30, 22))
{
	mRectangle.setFillColor(sf::Color::Green);
	mRectangle.setPosition(position.x * 32, position.y * 24);
}

PhysicsWorld::PhysicsWorld(StateStack* stateStack) :
	State(false, false, stateStack),
	world(b2Vec2(0, 9.81))
{
	addGround();
	for (int i = 0; i < 9; ++i)
	{
		addSquare(i);
	}
}

void PhysicsWorld::addSquare(int number)
{
	Entity* newEntity = new Entity();
	int r = rand() % 255;
	int g = rand() % 255;
	int b = rand() % 255;
	RectangleRenderScript* rect = new RectangleRenderScript(sf::Vector2f(50, 50), sf::Color(r,g,b, 255), sf::Vector2f(rand() % 300 + 200, rand() % 300), newEntity, this);
	BoxCollider* collider = new BoxCollider(&world, rect, 1.0f, 2.0f, newEntity, false);
	MouseMoverScript* mover = new MouseMoverScript(newEntity, &world);
	Finishable* finishable = new Finishable(newEntity, number);
	newEntity->addComponent(rect);
	newEntity->addComponent(collider);
	newEntity->addComponent(mover);
	newEntity->addComponent(finishable);
	mEntities.push_back(newEntity);

	Entity* toMakeEntity = new Entity();
	RectangleRenderScript* rectToMake = new RectangleRenderScript(sf::Vector2f(50, 50), sf::Color(r,g,b, 130), sf::Vector2f(600, 430 - 50 * number), toMakeEntity, this);
	Fadeable* fadeable = new Fadeable(toMakeEntity, number);
	toMakeEntity->addComponent(rectToMake);
	toMakeEntity->addComponent(fadeable);
	mEntities.push_back(toMakeEntity);
}

void PhysicsWorld::addGround()
{
	Entity* newEntity = new Entity();
	RectangleRenderScript* rect = new RectangleRenderScript(sf::Vector2f(640, 20), sf::Color::Blue, sf::Vector2f(320, 470), newEntity, this);
	BoxCollider* collider = new BoxCollider(&world, rect,0.0f, 2.0f, newEntity, true);
	newEntity->addComponent(rect);
	newEntity->addComponent(collider);
	mEntities.push_back(newEntity);

	Entity* wall1 = new Entity();
	RectangleRenderScript* wallRect = new RectangleRenderScript(sf::Vector2f(100, 480), sf::Color::Blue, sf::Vector2f(-40, 240), wall1, this);
	BoxCollider* wallCollider = new BoxCollider(&world, wallRect, 0.0f, 2.0f, wall1, true);
	wall1->addComponent(wallRect);
	wall1->addComponent(wallCollider);

	Entity* wall2 = new Entity();
	RectangleRenderScript* wallRect2 = new RectangleRenderScript(sf::Vector2f(100, 480), sf::Color::Blue, sf::Vector2f(680, 240), wall2, this);
	BoxCollider* wallCollider2 = new BoxCollider(&world, wallRect2, 0.0f, 2.0f, wall2, true);
	wall2->addComponent(wallRect);
	wall2->addComponent(wallCollider);

	mEntities.push_back(wall1);
	mEntities.push_back(wall2);
}

void PhysicsWorld::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		mStateStack->popState();
	}
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Button::Left)
		{
			ObserverMessage message;
			message.mType = ObserverMessageType::MouseClicked;
			sf::Vector2i mousePos = sf::Mouse::getPosition(*(mStateStack->mWindow));
			message.twoFloats.mFloat1 = mousePos.x;
			message.twoFloats.mFloat2 = mousePos.y;
			Subscription::get()->sendMessage(ObserverMessageType::MouseClicked, message);
		}
	}

	if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Button::Left)
		{
			ObserverMessage message;
			message.mType = ObserverMessageType::MouseReleased;
			Subscription::get()->sendMessage(ObserverMessageType::MouseClicked, message);
		}
	}

	sf::Vector2i mousePos = sf::Mouse::getPosition(*(mStateStack->mWindow));
	ObserverMessage msg;
	msg.mType = ObserverMessageType::MouseMoved;
	msg.twoFloats.mFloat1 = mousePos.x;
	msg.twoFloats.mFloat2 = mousePos.y;
	Subscription::get()->sendMessage(ObserverMessageType::MouseMoved, msg);


}