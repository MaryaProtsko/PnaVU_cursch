#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/Graphics.hpp>
#include <vector>
#include "Animation.h"
#include "level.h"


class Entity
{
public:
	float x, y, dx, dy, w, h;
	AnimationManager anim;
	std::vector<Object> obj;
	bool life, dir;
	float timer, timer_end;
	std::string Name;
	bool isDead;

	Entity(AnimationManager& A, int X, int Y);
	virtual void update(float time) = 0;
	void draw(sf::RenderWindow& window);
	sf::FloatRect getRect();
	void option(std::string NAME, float SPEED, std::string FIRST_ANIM);
	virtual ~Entity();
};

class Player : public Entity
{
public:
	enum { stay, walk, jump } STATE;
	int cash;
	float sx, sy;
	std::map<std::string, bool> key;

	Player(AnimationManager& a, Level& lev, int x, int y);
	void Keyboard();
	void Animation(float time);
	void update(float time);
	void Collision(int num);
};

class Enemy : public Entity
{
public:
	Enemy(AnimationManager& a, Level& lev, int x, int y);
	void update(float time);
	void Collision(float Dx, float Dy);
};

#endif ENTITY_H