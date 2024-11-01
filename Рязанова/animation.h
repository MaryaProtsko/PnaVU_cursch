#ifndef ANIMATION_H
#define ANIMATION_H

#include "tinyxml.h"
#include <SFML/Graphics.hpp>


class Animation
{
public:
	std::vector<sf::IntRect> frames, frames_flip;
	float currentFrame, speed;
	bool loop, flip, isPlaying;
	sf::Sprite sprite;

	Animation();
	void tick(float time);

};

class AnimationManager
{
public:
	std::string currentAnim;
	std::map<std::string, Animation> animList;

	AnimationManager();
	~AnimationManager();

	void loadFromXML(std::string fileName, sf::Texture& t);
	void set(std::string name);
	void draw(sf::RenderWindow& window, int x, int y);
	void flip(bool b);
	void tick(float time);
	void pause();
	void play();
	void play(std::string name);
	bool isPlaying();
	float getH();
	float getW();
};

#endif ANIMATION_H
