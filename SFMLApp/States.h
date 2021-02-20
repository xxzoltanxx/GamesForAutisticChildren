#pragma once
#include <SFML/Graphics.hpp>

class StateStack;


constexpr float guessThreshold = 30.0f;
constexpr int colorsToPick = 5;

enum Guess
{
	None,
	Right,
	Wrong
};

class State
{
public:
	State(bool isTranslucent, bool isTranscendent, StateStack* stateStack);
	enum States
	{
		MainMenu,
		GameState,
		Gusjenica
	};

	bool isTranslucent() const
	{
		return mIsTranslucent;
	}

	bool isTranscendent() const
	{
		return mIsTranscendent;
	}

	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void handleEvent(sf::Event& event) = 0;
protected:
	bool mIsTranslucent;
	bool mIsTranscendent;
	StateStack* mStateStack;
};

class Segment
{
public:
	enum SegmentType
	{
		First = 0,
		Second,
		Third,
		Fourth,
		Fifth,
		Sixth,
	};
	Segment(SegmentType type);

	void draw(sf::RenderWindow& window);
	void addToPosition(const sf::Vector2f& position);
	void color(const sf::Color& color);

	const sf::Color& getColor() const { return mSegmentSprite.getColor(); }
private:
	sf::Sprite mSegmentSprite;
};

class GusjenicaState : public State
{
public:
	GusjenicaState(StateStack* stateStack);
	void handleEvent(sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
private:

	void colorSegments();
	void setPallete(const sf::Color& color);

	void pickColor(const sf::Color& color);

	std::vector<Segment> mUncolored;
	std::vector<Segment> mColored;

	std::vector<sf::RectangleShape> mPallete;

	int mCurrentSegment = 0;
	int mDifficulty = 2;

	sf::Sprite mSmiley;
	sf::Sprite mSadFace;

	bool gameOver = false;
};

class MainMenu : public State
{
public:
	MainMenu(StateStack* stateStack);
	void handleEvent(sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
private:
	sf::Sprite mSmiley;
};

class GameState : public State
{
public:
	GameState(StateStack* stateStack);
	void setNextGoal();

	void handleEvent(sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
private:

	void guessColor();

	sf::CircleShape mCircleGoal;
	sf::CircleShape mCircleCurrent;
	sf::Sprite mSmiley;
	sf::Sprite mSadFace;

	float mRed;
	float mGreen;
	float mBlue;

	float mRedInterpolated;
	float mGreenInterpolated;
	float mBlueInterpolated;

	float mInterpolation = 0;
	bool mInterpolatingUpwards = true;

	sf::Color mGuessColors[colorsToPick];
	int mCurrentColor = 0;

	float mSizeCircle = 100.0f;
	float mGoalCircle = 100.0f;

	float mInterpolationSize = 0.0f;

	bool mLocked = false;

	float mFactor = 1.0;
	Guess mGuess = Guess::None;
};
