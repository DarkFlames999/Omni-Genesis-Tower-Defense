#include "InputHandler.h"

bool InputHandler::isHeldKey(HeldKeyBinding& binding, sf::Event event)
{
    if ((event.type == sf::Event::KeyPressed && event.key.code == binding.key) ||
        (event.type == sf::Event::KeyReleased && event.key.code == binding.key))
    {
        if (!binding.isHolding)
        {
            binding.timer.restart();
        }
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == binding.key)
    {
        binding.isHolding = true;
        if (binding.timer.getElapsedTime().asSeconds() >= binding.timeToHold && binding.isHolding)
        {
            binding.holdAchieved = true;
            return binding.holdAchieved;
        }
    }
    else if (event.type == sf::Event::KeyReleased && event.key.code == binding.key && binding.isHolding)
    {
        binding.isHolding = false;
        binding.holdAchieved = false;
        return binding.holdAchieved;
    }
    return false;
}
bool InputHandler::isDoubleTap(DoubleTapBinding& binding, sf::Event event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == binding.key)
    {
        if (!binding.firstTap)
        {
            binding.firstTap = true;
            binding.tapCount = 1.f;
            binding.timer.restart();
        }
        else
        {
            binding.tapCount += 1.f;
            if (binding.tapCount >= 2.f && binding.timer.getElapsedTime().asSeconds() <= binding.timeToDoubleTap && binding.firstTap)
            {
                binding.doubleTapOccured = true;
                return binding.doubleTapOccured;
            }
        }
    }

    if ((binding.timer.getElapsedTime().asSeconds() > binding.timeToDoubleTap) || (binding.firstTap && binding.doubleTapOccured))
    {
        binding.firstTap = false;
        binding.doubleTapOccured = false;
        binding.tapCount = 0.f;
        return false;
    }
    return false;
}

void InputHandler::ImplementHeldKey(sf::Keyboard::Key Key, float TimeToHold)
{
    HeldKeyBinding newHeldKeyBinding;
    newHeldKeyBinding.key = Key;
    newHeldKeyBinding.timeToHold = TimeToHold;
    mHeldKeyBindings.push_back(newHeldKeyBinding);
}

void InputHandler::ImplementDoubleTap(sf::Keyboard::Key Key, float TimeToDoubleTap)
{
    DoubleTapBinding newDoubleTapBinding;
    newDoubleTapBinding.key = Key;
    newDoubleTapBinding.timeToDoubleTap = TimeToDoubleTap;
    mDoubleTapBindings.push_back(newDoubleTapBinding);
}
    
void InputHandler::update(sf::Event event)
{
    for (HeldKeyBinding& heldKeyBinding : mHeldKeyBindings)
    {
        isHeldKey(heldKeyBinding, event);
    }
    for (DoubleTapBinding& doubleTapBinding : mDoubleTapBindings)
    {
        isDoubleTap(doubleTapBinding, event);
    }
}