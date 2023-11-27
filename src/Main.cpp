// Libraries
#include <iostream>
#include <thread>
#include <cassert>

#include <reactphysics3d/reactphysics3d.h>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace reactphysics3d;

const decimal TIME_STEP = 1.0f / 60.0f;
const decimal WINDOW_REFRESH_STEP = 1.0f / 144.0f * 1000;

void runPhysics(PhysicsWorld* world) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)std::floor(TIME_STEP * 1000)));
        
        world->update(TIME_STEP);
    }
}

void runGraphics(RigidBody* body) {
    sf::RenderWindow sfmlWin(sf::VideoMode(1600, 900), "TaxiDrifter SFML debugger");

    while (sfmlWin.isOpen()) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)std::floor(WINDOW_REFRESH_STEP)));

        sfmlWin.clear();

        sf::Event e;
        while (sfmlWin.pollEvent(e)) {

            switch (e.type) {
            case sf::Event::EventType::Closed:
                sfmlWin.close();
                break;
            }
        }

        /*
        *   up arrow
        */

        sf::RectangleShape up_arrow(sf::Vector2f(50, 50));
        up_arrow.setPosition(200, 700);

        sf::Texture up_texture;
        if (!up_texture.loadFromFile("up.png"))
        {
            return;
        }

        up_arrow.setTexture(&up_texture);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            up_arrow.setFillColor(sf::Color::Red);
        }
        
        sfmlWin.draw(up_arrow);

        /*
        *   left arrow
        */

        sf::RectangleShape left_arrow(sf::Vector2f(50, 50));
        left_arrow.setPosition(150, 750);

        sf::Texture left_texture;
        if (!left_texture.loadFromFile("left.png"))
        {
            return;
        }

        left_arrow.setTexture(&left_texture);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            left_arrow.setFillColor(sf::Color::Red);
        }
        
        sfmlWin.draw(left_arrow);

        /*
        *   right arrow
        */

        sf::RectangleShape right_arrow(sf::Vector2f(50, 50));
        right_arrow.setPosition(250, 750);

        sf::Texture right_texture;
        if (!right_texture.loadFromFile("right.png"))
        {
            return;
        }

        right_arrow.setTexture(&right_texture);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            right_arrow.setFillColor(sf::Color::Red);
        }
        
        sfmlWin.draw(right_arrow);

        /*
        *   down arrow
        */

        sf::RectangleShape down_arrow(sf::Vector2f(50, 50));
        down_arrow.setPosition(200, 750);

        sf::Texture down_texture;
        if (!down_texture.loadFromFile("down.png"))
        {
            return;
        }

        down_arrow.setTexture(&down_texture);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            down_arrow.setFillColor(sf::Color::Red);
        }
        
        sfmlWin.draw(down_arrow);

        /*
        *   falling box
        */

        sf::RectangleShape rectangle(sf::Vector2f(50, 50));

        rectangle.setPosition(200, 50 - 70 * body->getTransform().getPosition().y);

        sfmlWin.draw(rectangle);
        sfmlWin.display();
    }
}

int main() {
    PhysicsCommon physicsCommon;

    PhysicsWorld* world = physicsCommon.createPhysicsWorld();

    Vector3 init_position(0, 20, 0);
    Quaternion init_orientation = Quaternion::identity();
    Transform init_transform(init_position, init_orientation);
    RigidBody* body = world->createRigidBody(init_transform);

    thread physics_thread = thread(runPhysics, world);
    thread graphics_thread = thread(runGraphics, body);

    physics_thread.join();
    graphics_thread.join();

    return 0;
}