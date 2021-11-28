#pragma once

class RenderContext;

class DrawCallsSource
{
public:
    virtual ~DrawCallsSource() = default;
    virtual void CollectDrawCalls(RenderContext* renderContext) = 0;
};
