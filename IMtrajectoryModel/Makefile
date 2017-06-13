IMtrajectoryModel: main.o classes/IMinputFileParser.o classes/IMlogfile.o functions/IMinterpreter.o classes/IMmodel.o classes/IMtrajectory.o functions/runTests.o functions/utilities.o
	g++ main.o classes/IMinputFileParser.o classes/IMlogfile.o functions/IMinterpreter.o classes/IMmodel.o classes/IMtrajectory.o functions/runTests.o functions/utilities.o -o IMtrajectoryModel

main.o: main.cpp
	g++ -c main.cpp

utilities.o: functions/utilities.cpp
	g++ -c functions/utilities.cpp

IMinputFileParser.o: classes/IMinputFileParser.cpp
	g++ -c classes/IMinputFileParser.cpp

IMlogfile.o: classes/IMlogfile.cpp
	g++ -c classes/IMlogfile.cpp

IMinterpreter.o: functions/IMinterpreter.cpp
		g++ -c functions/IMinterpreter.cpp

IMmodel.o: classes/IMmodel.cpp
		g++ -c classes/IMmodel.cpp

IMtrajectory.o: classes/IMtrajectory.cpp
		g++ -c classes/IMtrajectory.cpp

runTests.o: functions/runTests.cpp
		g++ -c functions/runTests.cpp

clean:
	rm *.o classes/*.o functions/*.o IMtrajectoryModel
