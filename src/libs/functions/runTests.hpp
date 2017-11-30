//
//  runTests.hpp
//  IMtrajectoryModel
//
//  Created by Kai Schüller on 29.05.17.
//  Copyright © 2017 Kai Schüller. All rights reserved.
//

#ifndef runTests_hpp
#define runTests_hpp

#include <iostream>
#include <string>
#include "../classes/IMlogfile.hpp"
#include "../functions/IMinterpreter.hpp"
#include "../classes/IMmodel.hpp"
#include "../classes/IMtrajectory.hpp"
#include "../classes/IMinputFileParser.hpp"
#include "../functions/improvedAnalyticalModel_velocity.hpp"
using namespace std;

int testLogfileReading(string);

int runTests(string,int);

#endif /* runTests_hpp */
