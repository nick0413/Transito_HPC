#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML UI Example");

    sf::Font font;
    if (!font.loadFromFile("fonts/ethnocentric rg.otf"))
    {
        // handle font loading error
    }

    // Button
    sf::RectangleShape button(sf::Vector2f(200.f, 50.f));
    button.setPosition(300.f, 250.f);
    button.setFillColor(sf::Color::Blue);

    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setString("Click Me");
    buttonText.setPosition(350.f, 260.f);

    // Label
    sf::Text label;
    label.setFont(font);
    label.setCharacterSize(24);
    label.setFillColor(sf::Color::Black);
    label.setString("No button clicked.");
    label.setPosition(300.f, 400.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (button.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                {
                    label.setString("Button clicked!");
                }
            }
        }

        window.clear(sf::Color::White);
        window.draw(button);
        window.draw(buttonText);
        window.draw(label);
        window.display();
    }

    return 0;
}