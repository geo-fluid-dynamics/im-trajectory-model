# Set the name of the project and target:
SET(TARGET "IMTrajectoryModel")

FILE(GLOB_RECURSE TARGET_SRC  src/libs/*.cpp src/main.cpp)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)

PROJECT(${TARGET})

ADD_EXECUTABLE(${TARGET} ${TARGET_SRC})

ENABLE_TESTING()

foreach(number 1 2 3 4 5 6 7 8 9)
  add_test(NAME IMInterpreter${number} COMMAND IMTrajectoryModel -t IMInterpreter ${number})
endforeach()

foreach(number 1 2 3 4 5)
  add_test(NAME IMModel${number} COMMAND IMTrajectoryModel -t IMModel ${number})
endforeach()

foreach(number 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17)
  add_test(NAME IMTrajectory${number} COMMAND IMTrajectoryModel -t IMTrajectory ${number})
endforeach()

foreach(number 1 2 3 4 5 6 7)
add_test(NAME IMTrajectoryModel_test_sl${number} COMMAND IMTrajectoryModel -sl ../test/simplifiedLogfile/test_sl${number}/test_sl${number}.csv -i ../test/simplifiedLogfile/test_sl${number}/test_sl${number}.ini -o Testing/test_sl${number}.result)
add_test(IMTrajectoryModel_test_sl${number}_Compare ${CMAKE_COMMAND} -E compare_files Testing/test_sl${number}.result
../test/simplifiedLogfile/test_sl${number}/test_sl${number}.result)
endforeach()

foreach(number 1)
  add_test(NAME improvedAnalyticalModel${number} COMMAND IMTrajectoryModel -t improvedAnalyticalModel ${number})
endforeach()