#include "Entity.h";
#include "States.h";

const std::string vertexShaderCircle = R"(
varying vec4 pos;
uniform mat4 modelMatrix;

void main(){
	// transform the vertex position
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	pos = gl_Vertex;

	// transform the texture coordinates
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

	// forward the vertex color
	gl_FrontColor = gl_Color;
};)";

const std::string fragmentShaderCircle =
R"(
uniform vec2 center;
uniform vec4 defaultColor;
uniform vec4 colOne;
uniform vec4 colTwo;
uniform vec4 colThree;
uniform vec4 colFour;
uniform vec4 colFive;
uniform float radius;
uniform float interpolation;
uniform int currentShown;
uniform mat4 modelMatrix;

attribute vec4 pos;

void main()
{
	vec4 gl_Position = modelMatrix * pos;
	// multiply it by the color
if (sqrt((gl_Position.xy - center).x * (gl_Position.xy - center).x +  (gl_Position.xy - center).y * (gl_Position.xy - center).y) < radius / 5.0)
{
	if (currentShown <= 1)
{
		gl_FragColor = defaultColor;
}
	else
{
	gl_FragColor = colOne;
}
}
		
if (sqrt((gl_Position.xy - center).x * (gl_Position.xy - center).x  +  (gl_Position.xy - center).y * (gl_Position.xy - center).y) > radius / 5.0 && sqrt((gl_Position.xy - center).y * (gl_Position.xy - center).y + (gl_Position.xy - center).x * (gl_Position.xy - center).x)  < radius / 5.0 * 2)
{
		if (currentShown <= 2)
{
		gl_FragColor = defaultColor;
}
	else
{
	gl_FragColor = colTwo;
}
}
else if (sqrt((gl_Position.xy - center).x * (gl_Position.xy - center).x  +  (gl_Position.xy - center).y * (gl_Position.xy - center).y) > radius / 5.0 * 2 && sqrt((gl_Position.xy - center).y * (gl_Position.xy - center).y + (gl_Position.xy - center).x * (gl_Position.xy - center).x)  < radius / 5.0 * 3)
{
		if (currentShown <= 3)
{
		gl_FragColor = defaultColor;
}
	else
{
	gl_FragColor = colThree;
}
}
else if (sqrt((gl_Position.xy - center).x * (gl_Position.xy - center).x  +  (gl_Position.xy - center).y * (gl_Position.xy - center).y) > radius / 5.0 * 3 && sqrt((gl_Position.xy - center).y * (gl_Position.xy - center).y + (gl_Position.xy - center).x * (gl_Position.xy - center).x)  < radius / 5.0 * 4)
{
		if (currentShown <= 4)
{
		gl_FragColor = defaultColor;
}
	else
{
	gl_FragColor = colFour;
}
}
else if (sqrt((gl_Position.xy - center).x * (gl_Position.xy - center).x  +  (gl_Position.xy - center).y * (gl_Position.xy - center).y) > radius / 5.0 * 4 && sqrt((gl_Position.xy - center).y * (gl_Position.xy - center).y + (gl_Position.xy - center).x * (gl_Position.xy - center).x)  < radius / 5.0 * 5)
{
		if (currentShown <= 5)
{
		gl_FragColor = defaultColor;
}
	else
{
	gl_FragColor = colFive;
}
}
		
	if (currentShown == 1 && sqrt((gl_Position.xy - center).x * (gl_Position.xy - center).x +  (gl_Position.xy - center).y * (gl_Position.xy - center).y) < radius / 5.0)					
{	vec2 positionOnCircle = gl_Position.xy - center; 
   vec2 up = vec2(0, 1.0);
   float arkus = dot(normalize(positionOnCircle), up);
   if (abs(arkus) <= interpolation)
   gl_FragColor = colOne;
   else
   gl_FragColor = defaultColor;}
	else if (currentShown == 2 && sqrt((gl_Position.xy - center).x * (gl_Position.xy - center).x  +  (gl_Position.xy - center).y * (gl_Position.xy - center).y) > radius / 5.0 && sqrt((gl_Position.xy - center).y * (gl_Position.xy - center).y + (gl_Position.xy - center).x * (gl_Position.xy - center).x)  < radius / 5.0 * 2)				
	{vec2 positionOnCircle = gl_Position.xy - center; 
   vec2 up = vec2(0, 1.0);
   float arkus = dot(normalize(positionOnCircle), up);
   if (abs(arkus) <= interpolation)
   gl_FragColor = colTwo;
   else
   gl_FragColor = defaultColor;}
	else if (currentShown == 3 && sqrt((gl_Position.xy - center).x * (gl_Position.xy - center).x +  (gl_Position.xy - center).y * (gl_Position.xy - center).y) > radius / 5.0 * 2 && sqrt((gl_Position.xy - center).y * (gl_Position.xy - center).y + (gl_Position.xy - center).x * (gl_Position.xy - center).x) < radius / 5.0 * 3)				
	{vec2 positionOnCircle = gl_Position.xy - center; 
   vec2 up = vec2(0, 1.0);
   float arkus = dot(normalize(positionOnCircle), up);
   if (abs(arkus) <= interpolation)
   gl_FragColor = colThree;
   else
   gl_FragColor = defaultColor;}
	else if (currentShown == 4 && sqrt((gl_Position.xy - center).x * (gl_Position.xy - center).x +  (gl_Position.xy - center).y * (gl_Position.xy - center).y) > radius / 5.0 * 3 && sqrt((gl_Position.xy - center).y * (gl_Position.xy - center).y + (gl_Position.xy - center).x * (gl_Position.xy - center).x) < radius / 5.0 * 4)				
	{vec2 positionOnCircle = gl_Position.xy - center; 
   vec2 up = vec2(0, 1.0);
   float arkus = dot(normalize(positionOnCircle), up);
   if (abs(arkus) <= interpolation)
   gl_FragColor = colFour;
   else
   gl_FragColor = defaultColor;}
	else if (currentShown == 5 && sqrt((gl_Position.xy - center).x * (gl_Position.xy - center).x +  (gl_Position.xy - center).y * (gl_Position.xy - center).y) > radius / 5.0 * 4 && sqrt((gl_Position.xy - center).y * (gl_Position.xy - center).y + (gl_Position.xy - center).x * (gl_Position.xy - center).x) < radius / 5.0 * 5)				
	{vec2 positionOnCircle = gl_Position.xy - center; 
   vec2 up = vec2(0, 1.0);
   float arkus = dot(normalize(positionOnCircle), up);
   if (abs(arkus) <= interpolation)
   gl_FragColor = colFive;
   else
   gl_FragColor = defaultColor;}
})";

EntityScript::EntityScript(Type type, Entity* entity)
{
	mEntityScriptType = type;
	mEntity = entity;
}

Entity::~Entity()
{
	for (auto& a : mScripts)
	{
		delete a;
	}
}

void Entity::removeComponent(EntityScript* script)
{
	delete script;
	auto iter = std::find(mScripts.begin(), mScripts.end(), script);
	mScripts.erase(iter);
}

void Entity::update(float dt)
{
	for (auto& a : mScripts)
	{
		a->update(dt);
	}
}

sf::Vector2f RectangleRenderScript::getSize() const
{
	return mShape.getSize();
}

sf::Vector2f RectangleRenderScript::getPosition() const
{
	return mShape.getPosition();
}

CircleRenderScript::CircleRenderScript(sf::Vector2f size, sf::Color col1, sf::Color col2, sf::Color col3, sf::Color col4, sf::Color col5, sf::Color defaultColor, const sf::Vector2f& position, Entity* entity, State* state, bool finished)
	:RenderScript(EntityScript::Type::None, entity),
	mState(state)
{
	mShape.setPosition(position);
	mShape.setRadius(size.x);
	mShape.setOrigin(sf::Vector2f(size.x, size.x));
	mShader.loadFromMemory(vertexShaderCircle, fragmentShaderCircle);
	mShader.setUniform("colOne", sf::Glsl::Vec4(col1));
	mShader.setUniform("colTwo", sf::Glsl::Vec4(col2));
	mShader.setUniform("colThree", sf::Glsl::Vec4(col3));
	mShader.setUniform("colFour", sf::Glsl::Vec4(col4));
	mShader.setUniform("colFive", sf::Glsl::Vec4(col5));
	mShader.setUniform("defaultColor", sf::Glsl::Vec4(defaultColor));
	mShader.setUniform("center", mShape.getPosition());
	mShader.setUniform("radius", mShape.getRadius());
	if (finished)
	{
		mShader.setUniform("interpolation", 1.0f);
		mShader.setUniform("currentShown", 5);
	}
	else
	{
		mShader.setUniform("interpolation", 0.0f);
		mShader.setUniform("currentShown", 1);
	}
	mShader.setUniform("modelMatrix", sf::Glsl::Mat4(mShape.getTransform().getMatrix()));
	onCreate();
}

void CircleRenderScript::setInterpolation(float interpolation)
{
	mShader.setUniform("interpolation", interpolation);
}

void CircleRenderScript::setNumber(int number)
{
	mShader.setUniform("currentShown", number);
}

sf::Vector2f CircleRenderScript::getSize() const
{
	return sf::Vector2f(mShape.getRadius(), 0);
}

sf::Vector2f CircleRenderScript::getPosition() const
{
	return mShape.getPosition();
}

void CircleRenderScript::changeRotation(float rotation)
{
	mShape.setRotation(rotation);
}

void CircleRenderScript::changePosition(const sf::Vector2f& position)
{
	mShape.setPosition(position);
}

float CircleRenderScript::getRotation() const
{
	return mShape.getRotation();
}

void CircleRenderScript::update(float dt)
{

}

void CircleRenderScript::onCreate()
{
	mState->addDrawable(this);
}

void CircleRenderScript::onDestroy()
{
	mState->removeDrawable(this);
}

void CircleRenderScript::changeColor(const sf::Color& col)
{
	mShape.setFillColor(col);
}

void CircleRenderScript::notify(const ObserverMessage& msg)
{

}

void CircleRenderScript::draw(sf::RenderWindow& window)
{
	window.draw(mShape, &mShader);
}

bool CircleRenderScript::containsPoint(const sf::Vector2f& point)
{
	return mShape.getGlobalBounds().contains(point);
}

sf::Color CircleRenderScript::getColor() const
{
	return mShape.getFillColor();
}


RectangleRenderScript::RectangleRenderScript(sf::Vector2f size, sf::Color color, const sf::Vector2f& position, Entity* entity, State* state)
	:
	RenderScript(EntityScript::Type::Rectangle, entity),
	mState(state)
{
	mShape.setFillColor(color);
	mShape.setPosition(position);
	mShape.setSize(size);
	mShape.setOrigin(size / 2.0f);
	onCreate();
}

void RectangleRenderScript::changeRotation(float rotation)
{
	mShape.setRotation(rotation);
}

void RectangleRenderScript::changePosition(const sf::Vector2f& position)
{
	mShape.setPosition(position);
}

float RectangleRenderScript::getRotation() const
{
	return mShape.getRotation();
}

void RectangleRenderScript::update(float dt)
{

}

void RectangleRenderScript::onCreate()
{
	mState->addDrawable(this);
}

void RectangleRenderScript::onDestroy()
{
	mState->removeDrawable(this);
}

void RectangleRenderScript::changeColor(const sf::Color& col)
{
	mShape.setFillColor(col);
}

void RectangleRenderScript::notify(const ObserverMessage& msg)
{

}

void RectangleRenderScript::draw(sf::RenderWindow& window)
{
	window.draw(mShape);
}

bool RectangleRenderScript::containsPoint(const sf::Vector2f& point)
{
	return mShape.getGlobalBounds().contains(point);
}

sf::Color RectangleRenderScript::getColor() const
{
	return mShape.getFillColor();
}

BoxCollider::BoxCollider(b2World* world, RenderScript* object, float density, float friction, Entity* entity, bool isStatic)
	:EntityScript(EntityScript::Type::BoxCollider, entity)
{
	mWorld = world;
	b2BodyDef BodyDef;
	auto position = (object->getPosition()) / SCALE;
	BodyDef.position = b2Vec2(position.x, position.y);
	BodyDef.angle = object->getRotation() * b2_pi / 180.0f;
	if (isStatic)
	{
		BodyDef.type = b2_staticBody;
	}
	else
	{
		BodyDef.type = b2_dynamicBody;
	}
	Body = world->CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox(object->getSize().x / 2.0f / SCALE, object->getSize().y / 2.0f / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.friction = friction;
	FixtureDef.density = 1.f;
	FixtureDef.shape = &Shape;


	Body->CreateFixture(&FixtureDef);
	onCreate();
}

void BoxCollider::update(float dt)
{
	RenderScript* renderScript = (RenderScript*)mEntity->getComponent<RenderScript>();
	renderScript->changePosition(sf::Vector2f((Body->GetPosition().x) * SCALE, Body->GetPosition().y * SCALE));
	renderScript->changeRotation(Body->GetAngle() * 180.0f / b2_pi);
}

void BoxCollider::onCreate()
{

}

void BoxCollider::onDestroy()
{
	mWorld->DestroyBody(Body);
}

void BoxCollider::changePosition(const sf::Vector2f& newPos)
{
	sf::Vector2f position = (newPos) / SCALE;
	b2Vec2 posit = b2Vec2(position.x, position.y);
	Body->SetTransform(posit, Body->GetAngle());
}

MouseClickScript::MouseClickScript(Entity* entity, bool isCorrect, CircleColoringState* state) :
	EntityScript(EntityScript::Type::MouseHandler, entity)
{
	mState = state;
	mIsCorrectColor = isCorrect;
	onCreate();
}

MouseClickScript::~MouseClickScript()
{
	onDestroy();
}

void MouseClickScript::update(float dt)
{

}

void MouseClickScript::onCreate()
{
	Subscription::get()->addSubscriber(this, ObserverMessageType::MouseClicked);
}

void MouseClickScript::onDestroy()
{
	Subscription::get()->removeSubscriber(this, ObserverMessageType::MouseClicked);
}

void MouseClickScript::notify(const ObserverMessage& msg)
{
		if (((RenderScript*)mEntity->getComponent<RenderScript>())->containsPoint(sf::Vector2f(msg.twoFloats.mFloat1, msg.twoFloats.mFloat2)) && mIsCorrectColor)
		{
			ObserverMessage msg;
			msg.mType = ObserverMessageType::ColorPicked;
			msg.mBool = true;
			Subscription::get()->sendMessage(ObserverMessageType::ColorPicked, msg);
			mState->upgamePostFrame();
		}
		else if ((((RenderScript*)mEntity->getComponent<RenderScript>())->containsPoint(sf::Vector2f(msg.twoFloats.mFloat1, msg.twoFloats.mFloat2)) && !mIsCorrectColor))
		{
			ObserverMessage msg;
			msg.mType = ObserverMessageType::ColorPicked;
			msg.mBool = false;
			Subscription::get()->sendMessage(ObserverMessageType::ColorPicked, msg);
			mState->downgamePostFrame();
		}
}

MouseMoverScript::MouseMoverScript(Entity* entity, b2World* world):
	EntityScript(EntityScript::Type::MouseHandler, entity)
{
	mWorld = world;
	onCreate();
}

MouseMoverScript::~MouseMoverScript()
{
	onDestroy();
}

void MouseMoverScript::update(float dt)
{

}

void MouseMoverScript::onCreate()
{
	Subscription::get()->addSubscriber(this, ObserverMessageType::MouseMoved);
	Subscription::get()->addSubscriber(this, ObserverMessageType::MouseClicked);
	Subscription::get()->addSubscriber(this, ObserverMessageType::MouseReleased);
}

void MouseMoverScript::onDestroy()
{
	Subscription::get()->removeSubscriber(this, ObserverMessageType::MouseMoved);
	Subscription::get()->removeSubscriber(this, ObserverMessageType::MouseClicked);
	Subscription::get()->removeSubscriber(this, ObserverMessageType::MouseReleased);
}

void MouseMoverScript::notify(const ObserverMessage& msg)
{
	switch (msg.mType)
	{
	case ObserverMessageType::MouseMoved:
		if (selected)
		{
			((RenderScript*)mEntity->getComponent<RenderScript>())->changePosition(sf::Vector2f(msg.twoFloats.mFloat1, msg.twoFloats.mFloat2));
		}
		break;
	case ObserverMessageType::MouseClicked:
		if (((RenderScript*)mEntity->getComponent<RenderScript>())->containsPoint(sf::Vector2f(msg.twoFloats.mFloat1, msg.twoFloats.mFloat2)))
		{
			selected = true;
			mEntity->removeComponent(mEntity->getComponent<BoxCollider>());
		}
		else
		{
			selected = false;
		}
		break;
	case ObserverMessageType::MouseReleased:
		if (selected)
		{
			selected = false;
			EntityScript* collider = new BoxCollider(mWorld,(RenderScript*) mEntity->getComponent<RenderScript>(), 2.0f, 2.0f, mEntity, false);
			mEntity->addComponent(collider);
		}
	}
}

Fadeable::Fadeable(Entity* ent, int target) :
	EntityScript(EntityScript::Type::None, ent),
	mTarget(target)
{
	onCreate();
}

void Fadeable::update(float dt)
{
	interpolation += dt;
	if (interpolation < 1.0f)
	{
		sf::Color oldColor = ((RenderScript*)mEntity->getComponent<RenderScript>())->getColor();
		if (!targetReached)
		{
			oldColor.a = 255 - 125 * interpolation;
		}
		else
		{
			oldColor.a = 123 + 122 * interpolation;
		}
		((RenderScript*)mEntity->getComponent<RenderScript>())->changeColor(oldColor);
	}
}

void Fadeable::notify(const ObserverMessage& msg)
{
	if (msg.mType == ObserverMessageType::OutOfPosition && msg.mInt == mTarget)
	{
		interpolation = 0.0f;
		targetReached = false;
	}
	else if (msg.mType == ObserverMessageType::SetInPosition && msg.mInt == mTarget)
	{
		interpolation = 0.0f;
		targetReached = true;
	}
}

void Fadeable::onCreate()
{
	Subscription::get()->addSubscriber(this, ObserverMessageType::OutOfPosition);
	Subscription::get()->addSubscriber(this, ObserverMessageType::SetInPosition);
}

void Fadeable::onDestroy()
{
	Subscription::get()->removeSubscriber(this, ObserverMessageType::OutOfPosition);
	Subscription::get()->removeSubscriber(this, ObserverMessageType::SetInPosition);
}

Finishable::Finishable(Entity* ent, int position) : EntityScript(EntityScript::Type::None, ent), mPosition(position) { onCreate(); }

void Finishable::update(float dt)
{
	RenderScript* renderable = (RenderScript*)mEntity->getComponent<RenderScript>();
	if ((int)((480 - renderable->getPosition().y - 30) / 50) == mPosition)
	{
		mIsInPosition = true;
		mDoneOutOfPositionStep = false;
	}
	else
	{
		mIsInPosition = false;
		mDonePositionStep = false;
		if (!mDoneOutOfPositionStep)
		{
			ObserverMessage msg;
			msg.mInt = mPosition;
			msg.mType = ObserverMessageType::OutOfPosition;
			Subscription::get()->sendMessage(ObserverMessageType::OutOfPosition, msg);
			mDoneOutOfPositionStep = true;
		}
	}
	if (!mDonePositionStep && mIsInPosition)
	{
		ObserverMessage msg;
		msg.mInt = mPosition;
		msg.mType = ObserverMessageType::SetInPosition;
		Subscription::get()->sendMessage(ObserverMessageType::SetInPosition, msg);
		mDonePositionStep = true;
	}
}


CircleDrawable::CircleDrawable(Entity* entity)
	:EntityScript(EntityScript::Type::None, entity)
{
	onCreate();
}

void CircleDrawable::update(float dt)
{
	if (interpolation >= 1.0f || currentColor == 0)
	{
		return;
	}
	interpolation += dt;
	((CircleRenderScript*)mEntity->getComponent<CircleRenderScript>())->setInterpolation(interpolation);
}

void CircleDrawable::notify(const ObserverMessage& msg)
{
	if (msg.mType == ObserverMessageType::ColorPicked)
	{
		if (msg.mBool == true && currentColor <= 5)
		{
			((CircleRenderScript*)mEntity->getComponent<CircleRenderScript>())->setNumber(++currentColor);
			((CircleRenderScript*)mEntity->getComponent<CircleRenderScript>())->setInterpolation(0.0f);
			interpolation = 0.0f;
		}
		else if (msg.mBool == false && currentColor >= 1)
		{
			((CircleRenderScript*)mEntity->getComponent<CircleRenderScript>())->setNumber(--currentColor);
			((CircleRenderScript*)mEntity->getComponent<CircleRenderScript>())->setInterpolation(0.0f);
			interpolation = 0.0f;
		}
	}
}

CircleDrawable::~CircleDrawable()
{
	onDestroy();
}

void CircleDrawable::onCreate()
{
	Subscription::get()->addSubscriber(this, ObserverMessageType::ColorPicked);
}

void CircleDrawable::onDestroy()
{
	Subscription::get()->removeSubscriber(this, ObserverMessageType::ColorPicked);
}
