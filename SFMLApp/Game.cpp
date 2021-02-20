#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "States.h"
#include "StateStack.h"

class Game
{
public:
	Game();
	void run();
private:
	sf::RenderWindow mWindow;
	StateStack mStateStack;
};

Game::Game()
	: mWindow(sf::VideoMode(640, 480), "farba"), mStateStack(&mWindow)
{
	mStateStack.pushbackState(State::MainMenu);
}

void Game::run()
{
	sf::Clock clock;
	sf::Time time;
	while (!mStateStack.isEmpty() || !mStateStack.noCommand())
	{
		mWindow.clear();
		mStateStack.render(mWindow);
		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			mStateStack.handleEvents(event);
		}
		while (time > sf::seconds(1.0f / 60.0f))
		{
			time -= sf::seconds(1.0f / 60.0f);
			mStateStack.update(1.0f / 60.0f);
		}
		time += clock.restart();
		mWindow.display();
	}
}

int main()
{
	srand(time(nullptr));
	Game game;
	game.run();
}