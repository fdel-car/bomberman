#include "engine/Skybox.hpp"

extern std::string _assetsDir;

Skybox::Skybox(std::string str) : test(str) {}

Skybox::Skybox(void) {}

Skybox::~Skybox(void) {}

void Skybox::update(void) {}

std::string Skybox::getTest(void) { return test; }