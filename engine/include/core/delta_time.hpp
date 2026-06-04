#pragma once

class Time{
public:
    static void calculateDeltaTime();

    static float deltaTime();
    static float totalTime();

private:
    static float _current;
    static float _deltaTime;
    static float _lastFrame;
    static float _totalTime;
};
