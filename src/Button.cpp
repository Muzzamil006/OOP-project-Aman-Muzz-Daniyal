#include <iostream>
#include "Button.hpp"

using namespace std;

Button::Button() : shape(), label(), baseColor(60, 60, 90), enabled(true){
}

Button::Button(const string& text, const sf::Font& font, float x, float y, float width, float height)
    : shape(sf::Vector2f(width, height)), label(text, font, 18), baseColor(60, 60, 90), enabled(true){
    shape.setPosition(x, y);
    shape.setFillColor(baseColor);
    shape.setOutlineColor(sf::Color(230, 190, 70));
    shape.setOutlineThickness(2.0f);
    label.setFillColor(sf::Color::White);
    centerLabel();
}

void Button::centerLabel(){
    sf::FloatRect textBounds = label.getLocalBounds();
    sf::Vector2f position = shape.getPosition();
    sf::Vector2f size = shape.getSize();
    label.setPosition(position.x + size.x / 2.0f - textBounds.width / 2.0f - textBounds.left,
                      position.y + size.y / 2.0f - textBounds.height / 2.0f - textBounds.top);
}

void Button::setText(const string& text){
    label.setString(text);
    centerLabel();
}

void Button::setEnabled(bool enabled){
    this->enabled = enabled;
    if (enabled){
        shape.setFillColor(baseColor);
        label.setFillColor(sf::Color::White);
    }
    else{
        shape.setFillColor(sf::Color(45, 45, 50));
        label.setFillColor(sf::Color(120, 120, 120));
    }
}

void Button::setColor(const sf::Color& color){
    baseColor = color;
    if (enabled){
        shape.setFillColor(baseColor);
    }
}

bool Button::isEnabled() const{
    return enabled;
}

bool Button::contains(float x, float y) const{
    return shape.getGlobalBounds().contains(x, y);
}

bool Button::isClicked(const sf::Event& event) const{
    if (!enabled){
        return false;
    }
    if (event.type != sf::Event::MouseButtonPressed || event.mouseButton.button != sf::Mouse::Left){
        return false;
    }
    return contains((float)event.mouseButton.x, (float)event.mouseButton.y);
}

void Button::draw(sf::RenderWindow& window) const{
    window.draw(shape);
    window.draw(label);
}
