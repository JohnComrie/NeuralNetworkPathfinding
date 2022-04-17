#include "LevelEditor.h"

namespace AIFramework
{
	void LevelEditor::Update(b2World& _world, sf::RenderWindow& _window)
	{
		UpdateStaticShapeMenu(_world);
		UpdateGhostLine(_world, _window);

		leftButtonPressedPreviousFrame = sf::Mouse::isButtonPressed(sf::Mouse::Left) ? true : false;
	}
	
	void LevelEditor::UpdateStaticShapeMenu(b2World& _world)
	{
		bool menuOpen;

		ImGui::Begin("Level Editor", &menuOpen, ImGuiWindowFlags_MenuBar);

		// Menu bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load", "Ctrl+O")) { LoadLevel(_world); }
				if (ImGui::MenuItem("Save", "Ctrl+S")) { SaveLevel(); }
				if (ImGui::MenuItem("Clear", "Ctrl+W")) { ClearLevel(_world); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::Button("Create Line")) {
			ghostLine.setSize(sf::Vector2f(2.0f, 2.0f));
			ghostLineState = GhostLineState::First;
		}

		ImGui::End();
		
	}

	void LevelEditor::UpdateGhostLine(b2World& _world, sf::RenderWindow& _window)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);
		if (ghostLineState == GhostLineState::First)
		{
			ghostLine.setPosition(sf::Vector2f(mousePosition.x, mousePosition.y));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !leftButtonPressedPreviousFrame)
			{
				ghostLineState = GhostLineState::Second;
			}
		}
		else if (ghostLineState == GhostLineState::Second)
		{
			// Set the length of the line as the distance between the origin and the mouse position
			sf::Vector2f ghostLinePosition = ghostLine.getPosition();
			float length = Distance(sf::Vector2f(mousePosition.x, mousePosition.y), ghostLinePosition);
			ghostLine.setSize(sf::Vector2f(2.0f, length));

			// Use dot product to calculate the angle between up vector and vector between origin and mouse position
			sf::Vector2f direction = sf::Vector2f(mousePosition.x, mousePosition.y) - ghostLinePosition;
			float magnitudeOrigin = 1;
			float magnitudeDirection = std::sqrt((direction.x * direction.x) + (direction.y * direction.y));
			float angle = std::acos((direction.y) / (magnitudeOrigin * magnitudeDirection)) * (180 / PI);
			
			// Previous formula always returns a number between 0 and 180. 
			// This makes sure the number is between 0 and 360
			if (mousePosition.x > ghostLinePosition.x)
				angle = 180 + (180 - angle);

			ghostLine.setRotation(angle);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !leftButtonPressedPreviousFrame)
			{
				// Static shape origins are at (0.5, 0.5), whereas SFML shape origins are at (0, 0)
				// Use values calculated using the dot product to calculate the centre point between the two points
				sf::Vector2f linePosition = ghostLinePosition + ((direction / magnitudeDirection) * length * 0.5f);
				staticShapes.push_back(StaticShape(_world, linePosition, angle, ghostLine.getSize()));
				ghostLineState = GhostLineState::Hidden;
			}
		}
	}

	void LevelEditor::RenderGhostLine(sf::RenderWindow& _window)
	{
		if (!(ghostLineState == GhostLineState::Hidden))
		{
			_window.draw(ghostLine);
		}
	}

	void LevelEditor::SaveLevel()
	{
		std::ofstream outFile("Level.txt");
		for (StaticShape shape : staticShapes)
		{
			FileObject fileObject = FileObject(shape.getPosition(), shape.getRotation(), shape.getSize());
			outFile << fileObject;
		}
		outFile.close();
	}

	void LevelEditor::LoadLevel(b2World& _world)
	{
		ClearLevel(_world);
		std::ifstream inFile("Level.txt");
		if (inFile.is_open())
		{
			while (inFile.good())
			{
				FileObject fileObject;
				inFile >> fileObject;
				staticShapes.push_back(StaticShape(_world, fileObject.position, fileObject.rotation, fileObject.size));
			}
		}
		inFile.close();
	}
	
	void LevelEditor::ClearLevel(b2World& _world)
	{
		for (StaticShape shape : staticShapes)
		{
			shape.DestroyBody(_world);
		}
		staticShapes.clear();
	}

	std::istream& operator >> (std::istream& _in, FileObject& _obj)
	{
		_in >> _obj.position.x;
		_in >> _obj.position.y;
		_in >> _obj.rotation;
		_in >> _obj.size.x;
		_in >> _obj.size.y;
		return _in;
	}

	std::ostream& operator << (std::ostream& _out, const FileObject& _obj)
	{
		_out << _obj.position.x << std::endl;
		_out << _obj.position.y << std::endl;
		_out << _obj.rotation << std::endl;
		_out << _obj.size.x << std::endl;
		_out << _obj.size.y << std::endl;
		return _out;
	}
}
