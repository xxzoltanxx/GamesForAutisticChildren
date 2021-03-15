#pragma once

#include <unordered_map>
#include <algorithm>

enum class ObserverMessageType
{
	MouseMoved,
	MouseClicked,
	MouseReleased,
	SetInPosition,
	OutOfPosition
};

struct ObserverMessage
{
	ObserverMessageType mType;
	union
	{
		int mInt;
		bool mBool;
		struct TwoFloats
		{
			float mFloat1;
			float mFloat2;
		};

		TwoFloats twoFloats;
	};
};


class Observer
{
public:
	virtual ~Observer() {}
	virtual void notify(const ObserverMessage& message) = 0;
};



class Subscription
{
public:
	void sendMessage(ObserverMessageType type, ObserverMessage message);
	void addSubscriber(Observer* obs, ObserverMessageType type);
	void removeSubscriber(Observer* obs, ObserverMessageType type);

	static Subscription* get()
	{
		if (instance == nullptr)
		{
			instance = new Subscription();
		}
		return instance;
	}
private:

	static Subscription* instance;

	std::unordered_map<ObserverMessageType, std::vector<Observer*>> mSubscribers;
};

