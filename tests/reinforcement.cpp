#include <iostream>
#include "catch.h"

#include "../include/QLearn.h"
#include "../include/WireFitQLearn.h"
#include "../include/FidoControlSystem.h"
#include "../include/NeuralNet.h"
#include "../include/Adadelta.h"
#include "../include/LSInterpolator.h"

static const unsigned int NUM_TRIALS = 200;
static const double PERCENT_CORRECT = 0.70;

void train(rl::Learner *learner) {
	int correct = 0;
	int selector;
	for(unsigned int a = 0; a < NUM_TRIALS; a++) {
    selector = rand() % 2;
    int action = (int)learner->chooseBoltzmanAction({(double)selector}, 100)[0];
    learner->applyReinforcementToLastAction((1 - 2*fabs(action-selector)) / 3.0, {(double)selector});

		if((int)learner->chooseBoltzmanAction({0}, 0)[0] == 0) correct++; if((int)learner->chooseBoltzmanAction({1}, 0)[0] == 1) correct++;
  }

	std::cout << "Last selector: " << selector << "\n";
	REQUIRE(correct / double(NUM_TRIALS*2) > PERCENT_CORRECT);
}

TEST_CASE("QLearn ", "[reinforcement]") {
	srand(time(NULL));

	net::NeuralNet *modelNet = new net::NeuralNet(1, 1, 1, 2, "sigmoid");
	modelNet->setOutputActivationFunction("simpleLinear");
  rl::QLearn learner(modelNet, new net::Adadelta(0.9, 0.01, 1000), 1, 0, {{0}, {1}});
	train(&learner);
}


TEST_CASE("WireFitQLearn ", "[reinforcement]") {
	srand(time(NULL));

  rl::WireFitQLearn learner(1, 1, 1, 6, 3, {0}, {1}, 2, new rl::LSInterpolator(), new net::Adadelta(0.9, 0.1, 1000), 1, 0);
  train(&learner);
}

TEST_CASE("FidoControlSystem ", "[reinforcement]") {
	srand(time(NULL));

  rl::FidoControlSystem learner(1, {0}, {1}, 2);
  train(&learner);
}
