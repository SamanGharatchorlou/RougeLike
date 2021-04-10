#pragma once

class Actor;
class Collider;

std::vector<int> broadPhaseIndexes(const Collider* collider, const std::vector<Actor*>* targets);

void testFunction(float* minMax, int* range, const std::vector<Actor*>* targets, std::vector<int>* indexes);

void braodPhaseRange(float* minMax, const std::vector<Actor*>* targets, int* range, std::vector<int>* indexes);