#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class Button{
private:
    sf::RectangleShape shape;
    sf::Text label;
    sf::Color baseColor;
    bool enabled;

    void centerLabel();

public:
    Button();
    Button(const string& text, const sf::Font& font, float x, float y, float width, float height);

    void setText(const string& text);
    void setEnabled(bool enabled);
    void setColor(const sf::Color& color);
    bool isEnabled() const;

    bool contains(float x, float y) const;
    bool isClicked(const sf::Event& event) const;
    void draw(sf::RenderWindow& window) const;
};

#endif
