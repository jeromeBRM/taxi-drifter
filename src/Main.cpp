// Libraries
#include <iostream>
#include <thread>
#include <cassert>

#include <reactphysics3d/reactphysics3d.h>
#include <SFML/Graphics.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;
using namespace reactphysics3d;

const decimal TIME_STEP = 1.0f / 60.0f;
const decimal WINDOW_REFRESH_STEP = 1.0f / 144.0f * 1000;

const decimal FRONT_MASS = 725;
const decimal BACK_MASS = 725;
const decimal ACCELERATION = 15.79;

const decimal LENGTH = 4.31;
const decimal HEIGHT = 1.815;
const decimal WIDTH = 1.315;
const decimal WHEELBASE = 2.65;

Vector3 worldToLocalVelocity(Quaternion& relativeRotation, const Vector3& relativePosition, const Vector3& worldVelocity) {
    relativeRotation.normalize();
    
    Matrix3x3 relativeRotationMatrix = relativeRotation.getMatrix();

    // Translate the vector to local space
    return relativeRotationMatrix.getTranspose() * worldVelocity;
}

void runPhysics(PhysicsWorld* world) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)std::floor(TIME_STEP * 1000)));
        
        world->update(TIME_STEP);
    }
}

void runGraphics(RigidBody* back, RigidBody* front) {
    sf::RenderWindow sfmlWin(sf::VideoMode(1600, 900), "TaxiDrifter SFML debugger");

    while (sfmlWin.isOpen()) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)std::floor(WINDOW_REFRESH_STEP)));

        sfmlWin.clear();

        /*
        *   get local velocity of front rigidbody
        */

        Quaternion copyQuaternion = front->getTransform().getOrientation();

        Vector3 localSpaceVelocity = worldToLocalVelocity(copyQuaternion, front->getTransform().getPosition(), front->getLinearVelocity());

        /*
        *
        */

        sf::Event e;
        while (sfmlWin.pollEvent(e)) {

            switch (e.type) {
            case sf::Event::EventType::Closed:
                sfmlWin.close();
                break;
            }
        }

        /*
        *   floor
        */

        sf::RectangleShape rectanglefloor(sf::Vector2f(1400, 700));

        rectanglefloor.setPosition(100, 100);
        rectanglefloor.setFillColor(sf::Color::Blue);

        sfmlWin.draw(rectanglefloor);

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            Vector3 force(0.0, 0.0, ACCELERATION * (BACK_MASS + FRONT_MASS)); 
            back->applyLocalForceAtCenterOfMass(force);

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            Vector3 force(-localSpaceVelocity.z, 0.0, 0.0);
            front->applyLocalForceAtCenterOfMass(force * FRONT_MASS);
            
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            Vector3 force(localSpaceVelocity.z, 0.0, 0.0);
            front->applyLocalForceAtCenterOfMass(force * FRONT_MASS);

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            Vector3 force(0.0, 0.0, -40.0);
            back->applyLocalForceAtCenterOfMass(force);
            
            down_arrow.setFillColor(sf::Color::Red);
        }

        
        /*
        *   add wheel friction
        */

        copyQuaternion = front->getTransform().getOrientation();

        localSpaceVelocity = worldToLocalVelocity(copyQuaternion, front->getTransform().getPosition(), front->getLinearVelocity());
        Vector3 friction = Vector3(-localSpaceVelocity.x, 0.0, 0.0);
        front->applyLocalForceAtCenterOfMass(friction * FRONT_MASS);

        localSpaceVelocity = worldToLocalVelocity(copyQuaternion, back->getTransform().getPosition(), back->getLinearVelocity());
        friction = Vector3(-localSpaceVelocity.x, 0.0, 0.0);
        back->applyLocalForceAtCenterOfMass(friction * BACK_MASS);

        /*
        *
        */
        
        sfmlWin.draw(down_arrow);

        /*
        *   falling box
        */

        sf::RectangleShape rectangle(sf::Vector2f(4, 4));
        sf::RectangleShape rectangle2(sf::Vector2f(4, 4));

        rectangle.setPosition(795 + 10 * back->getTransform().getPosition().x, 445 - 10 * back->getTransform().getPosition().z);
        rectangle2.setPosition(795 + 10 * front->getTransform().getPosition().x, 445 - 10 * front->getTransform().getPosition().z);

        float angle = 0;
        Vector3 up(0.0, 1.0, 0.0);
        back->getTransform().getOrientation().getRotationAngleAxis(angle, up);

        //rectangle.setRotation(angle * 180);

        sfmlWin.draw(rectangle);
        sfmlWin.draw(rectangle2);

        sf::Vertex velocity_line[] =
        {
            sf::Vertex(sf::Vector2f(rectangle2.getPosition().x, rectangle2.getPosition().y)),
            sf::Vertex(sf::Vector2f(rectangle2.getPosition().x + back->getLinearVelocity().x * 10, rectangle2.getPosition().y - back->getLinearVelocity().z * 10))
        };

        sfmlWin.draw(velocity_line, 2, sf::Lines);

        sf::Font font;
        if (!font.loadFromFile("font.ttf")) {
            return;
        }

        string positionString = "x: " + to_string(back->getTransform().getPosition().x) + "\n" + 
        "y: " + to_string(back->getTransform().getPosition().y) + "\n" +
        "z: " + to_string(back->getTransform().getPosition().z) + "\n" +
        "z velocity: " + to_string(localSpaceVelocity.z) + "\n" +
        "x velocity: " + to_string(localSpaceVelocity.x) + "\n" +
        "km/h: " + to_string(localSpaceVelocity.x);

        sf::Text message(positionString, font);

        sfmlWin.draw(message);

        sfmlWin.display();
    }
}

int main() {
    PhysicsCommon physicsCommon;

    PhysicsWorld* world = physicsCommon.createPhysicsWorld();

    Vector3 init_position(0, 1.0 + HEIGHT / 2, - WHEELBASE / 2);
    Vector3 init_position2(0, 1.0 + HEIGHT / 2, WHEELBASE / 2);
    Quaternion init_orientation = Quaternion::identity();
    Transform init_transform(init_position, init_orientation);
    Transform init_transform2(init_position2, init_orientation);
    RigidBody* body = world->createRigidBody(init_transform);
    RigidBody* body2 = world->createRigidBody(init_transform2);

    /*
    *   set mass of car
    */

    body->setMass(BACK_MASS);
    body2->setMass(FRONT_MASS);

    /*
    *
    */

    Vector3 init_position_floor(0, -2, 0);
    Transform init_transform_floor(init_position_floor, init_orientation);
    RigidBody* floor = world->createRigidBody(init_transform_floor);

    const Vector3 halfExtents(70.0, 1.0, 35.0);
    const Vector3 halfExtents2(WIDTH / 2, HEIGHT / 2, 0.5);

    BoxShape* car = physicsCommon.createBoxShape(halfExtents2); 
    BoxShape* floorShape =  physicsCommon.createBoxShape(halfExtents);

    // Relative transform of the collider relative to the body origin
    Transform transform = Transform::identity();
    
    // Add the collider to the rigid body
    Collider* collider;
    Collider* collider2;
    Collider* floor_coll;
    collider = body->addCollider(car, transform);
    collider2 = body2->addCollider(car, transform);
    floor_coll = floor->addCollider(floorShape, transform);

    // Anchor point in world-space 
    Vector3 anchorPoint(0.0, 1.0 + HEIGHT / 2, 0.0); 
    
    // Create the joint info object 
    FixedJointInfo jointInfo(body, body2, anchorPoint);

    // Create the fixed joint in the physics world 
    FixedJoint* joint; 
    joint = dynamic_cast<FixedJoint*>(world->createJoint(jointInfo));

    floor->setType(BodyType::STATIC);

    thread physics_thread = thread(runPhysics, world);
    thread graphics_thread = thread(runGraphics, body, body2);

    physics_thread.join();
    graphics_thread.join();

    return 0;
}