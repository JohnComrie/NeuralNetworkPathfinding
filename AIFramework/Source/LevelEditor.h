#pragma once
#include "imgui.h"
#include <vector>
#include "StaticShape.h"
#include <iostream>
#include <fstream>

namespace AIFramework
{
	enum class GhostLineState { Hidden, First, Second };

	struct FileObject
	{
		FileObject(sf::Vector2f _position, float _rotation, sf::Vector2f _size) :
			position(_position),
			rotation(_rotation),
			size(_size)
		{
		}
		FileObject() :
			position(sf::Vector2f(0.0f, 0.0f)),
			rotation(0),
			size(sf::Vector2f(0.0f, 0.0f))
		{
		}

		sf::Vector2f position;
		float rotation;
		sf::Vector2f size;

		friend std::istream& operator >> (std::istream& _in, FileObject& _obj);
		friend std::ostream& operator << (std::ostream& _out, const FileObject& _obj);
	};

	class LevelEditor
	{
	public:
		LevelEditor()
			: staticShapeSize(sf::Vector2f(1.0f, 1.0f)),
			ghostLineState(GhostLineState::Hidden)
		{
			ghostLine.setPosition(sf::Vector2f(0.0f, 0.0f));
			ghostLine.setFillColor(sf::Color::Yellow);
			ghostLine.setSize(sf::Vector2f(2.0f, 2.0f));
		}

		void Update(b2World& _world, sf::RenderWindow& _window);
		void RenderGhostLine(sf::RenderWindow& _window);

		std::vector<StaticShape> staticShapes;

	private:

		sf::Vector2f staticShapeSize;
		void UpdateStaticShapeMenu(b2World& _world);
		void UpdateGhostLine(b2World& _world, sf::RenderWindow& _window);
		void LoadLevel(b2World& _world);
		void SaveLevel();
		void ClearLevel(b2World& _world);

		const sf::Vector2f maxShapeSize = sf::Vector2f(2000.0f, 2000.0f);
		const sf::Vector2f minShapeSize = sf::Vector2f(1.0f, 1.0f);
		sf::RectangleShape ghostLine;
		GhostLineState ghostLineState;

		bool leftButtonPressedPreviousFrame = false;
	};
}