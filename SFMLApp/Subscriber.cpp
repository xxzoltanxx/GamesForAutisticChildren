#include "Subcriber.h";

Subscription* Subscription::instance = nullptr;

void Subscription::sendMessage(Type type, ObserverMessage message)
{
	for (auto& a : mSubscribers[type])
	{
		a->notify(message);
	}
}

void Subscription::addSubscriber(Observer* obs, Type type)
{
	if (mSubscribers.find(type) == mSubscribers.end())
	{
		mSubscribers.insert(std::make_pair(type, std::vector<Observer*>()));
	}
	mSubscribers[type].push_back(obs);
}

void Subscription::removeSubscriber(Observer* obs, Type type)
{
	auto iter = std::find(mSubscribers[type].begin(), mSubscribers[type].end(), obs);
	mSubscribers[type].erase(iter);
}