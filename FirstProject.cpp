
#include<cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

float begin_speed=0.00f;
float speed_down=-0.5f; 
float angle =0.0f; 
float friction = 0.3f;
float Degree2Radian = 3.141f/180.f;

struct Car{
    float x = 300.00f;
    float y = 300.00f;

    sf::Color colour = sf::Color::Blue;
    sf::RectangleShape body;
    Car()
    {
        body.setSize(sf::Vector2f(50.f, 100.f));
        body.setFillColor(sf::Color::Blue);
        body.setPosition(sf::Vector2f(x, y));
        body.setOrigin(sf::Vector2f(30.f, 75.f));
    }
};


void Handleinput()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        begin_speed= (begin_speed + 0.09f);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        begin_speed=(begin_speed - 0.07f);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        angle = angle - 2.0f;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        angle = angle + 2.0f;   
    }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && // if player is not applying any speed up or down button then only friction should be applied;

    !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        if (begin_speed > 0)
            begin_speed -= friction;
        else if (begin_speed < 0)
            begin_speed += friction;
    }   

}
void Update_Physics(Car& car)
{
    if (std::abs(begin_speed) < 0.01f) //basically killing small sliding when there is no movement;
        begin_speed =0.00f;

    float rad = (angle - 90.f) * Degree2Radian; // math think 0 degree up but in our case 0 degree is up so angle - 90;

    car.x += std::cos(rad) * begin_speed; // degree at which car should move;
    car.y += std::sin(rad) * begin_speed;

    car.body.setPosition(sf::Vector2f(car.x, car.y));
    car.body.setRotation(sf::degrees(angle));
}

void Draw_Track(sf::RenderWindow& window , const sf::RectangleShape& track)
{
    window.draw(track);
}

void CheckTrackBounds(Car& car, const sf::RectangleShape& track)
{    sf::FloatRect bounds = track.getGlobalBounds();
    if (!bounds.contains(sf::Vector2f(car.x , car.y)))
    {
        car.x = 300.f;
        car.y = 300.f;
        begin_speed = 0.f;
        angle = 0.f;

        car.body.setPosition(sf::Vector2f(car.x, car.y));
        car.body.setRotation(sf::degrees(angle));
    }
}

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "First Racing Game"
    );

    window.setFramerateLimit(60);

    Car car;

    sf::RectangleShape track;
    track.setSize(sf::Vector2f(650.f, 400.f));
    track.setPosition(sf::Vector2f(100.f, 100.f));
    track.setFillColor(sf::Color(50, 50, 50));


    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // Game systems
        Handleinput();
        Update_Physics(car);

        // Rendering
        window.clear(sf::Color::Black);
        Draw_Track(window , track);
        CheckTrackBounds(car , track);
        window.draw(car.body);
        window.display();
    }

    return 0;
}
