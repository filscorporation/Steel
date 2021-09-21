#pragma once

#include "Steel/Core/Application.h"

#include <gtest/gtest.h>

class TestApplication : public ::testing::Environment
{
public:
    void SetUp() override;
    void TearDown() override;

public:
    static Application* App;
};
