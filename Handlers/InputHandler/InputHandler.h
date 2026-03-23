#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

struct HeldKeyBinding
{
    sf::Keyboard::Key key;
    float timeToHold;
    bool isHolding = false;
    bool holdAchieved = false;
    sf::Clock timer;
};

struct DoubleTapBinding
{
    sf::Keyboard::Key key;
    float timeToDoubleTap;
    bool firstTap = false;
    bool doubleTapOccured = false;
    float tapCount = 0.f;
    sf::Clock timer;
};

class InputHandler
{
public:
    InputHandler(){};
    ~InputHandler(){};

    bool isHeldKey(HeldKeyBinding& binding, sf::Event event);
    bool isDoubleTap(DoubleTapBinding& binding, sf::Event event);
    void ImplementHeldKey(sf::Keyboard::Key Key, float TimeToHold);
    void ImplementDoubleTap(sf::Keyboard::Key Key, float TimeToDoubleTap);
    void update(sf::Event event);

    std::vector<HeldKeyBinding> mHeldKeyBindings;
    std::vector<DoubleTapBinding> mDoubleTapBindings;
};


#endif