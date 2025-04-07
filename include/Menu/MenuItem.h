#pragma once

class MenuItem
{
    public:
        virtual ~MenuItem();
        virtual void render() = 0;
};