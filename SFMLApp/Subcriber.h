#pragma once

#include <unordered_map>
#include <algorithm>

struct ObserverMessage
{
	union
	{
		int mInt;
		bool mBool;
		float mFloat;
	};
};

class Observer
{
public:
	virtual void notify(const ObserverMessage& message) = 0;
};

class Subscription
{
public:
	enum Type
	{
		MouseMoved,
	};

	void sendMessage(Type type, ObserverMessage message);
	void addSubscriber(Observer* obs, Type type);
	void removeSubscriber(Observer* obs, Type type);

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

	std::unordered_map<Type, std::vector<Observer*>> mSubscribers;
};