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

        sf::Event e;
        while (sfmlWin.pollEvent(e)) {

            switch (e.type) {
            case sf::Event::EventType::Closed:
                sfmlWin.close();
                break;
            }
        }

        sf::RectangleShape rectangle(sf::Vector2f(50, 50));

        rectangle.setPosition(200, - 100 - 70 * body->getTransform().getPosition().y);

        sfmlWin.clear();
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