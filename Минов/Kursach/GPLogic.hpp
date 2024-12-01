#pragma once
#include "GPFields.hpp"

class GPLogic : public virtual GPFields
{
private:
	const std::vector<unsigned char> m_frames_per_tile = {48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 5, 5, 4, 4, 4};
	const float m_move_time = .06f;
	float m_fall_time = .5f;
	float m_move_timer = 0.f;

	bool CollionOccured(Figure& fig) const;
	bool TryMove(int dx, int dy, Figure& fig) const;
	bool TryRotate(int dir, Figure& fig) const;
	void EndGame();
	void StartAnimation();
	void EndAnimation();
	void LandFigure(Figure& fig);
	void CheckClearedLines();
	void DropLines();
	void HardDrop();
protected:
	std::mt19937 m_random_generator;
	void HandleControlsEvent(const sf::Event event);
	void UpdateGhost();
	void PlayingUpdate(float dt);
	void AnimationUpdate(float dt);
	void ResetFigure();
public: 
	void SetSeed(unsigned int seed);
	bool IsNewRecord(int& record_out);
};