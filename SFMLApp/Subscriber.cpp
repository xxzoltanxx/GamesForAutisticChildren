#include "Subcriber.h";

Subscription* Subscription::instance = nullptr;

void Subscription::sendMessage(ObserverMessageType type, ObserverMessage message)
{
	for (auto& a : mSubscribers[type])
	{
		a->notify(message);
	}
}

void Subscription::addSubscriber(Observer* obs, ObserverMessageType type)
{
	if (mSubscribers.find(type) == mSubscribers.end())
	{
		mSubscribers.insert(std::make_pair(type, std::vector<Observer*>()));
	}
	mSubscribers[type].push_back(obs);
}

void Subscription::removeSubscriber(Observer* obs, ObserverMessageType type)
{
	auto iter = std::find(mSubscribers[type].begin(), mSubscribers[type].end(), obs);
	mSubscribers[type].erase(iter);
}