# Set the name of the project and target:
SET(TARGET "IMtrajectoryModel")

FILE(GLOB_RECURSE TARGET_SRC  src/libs/*.cpp src/main.cpp)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)

PROJECT(${TARGET})

ADD_EXECUTABLE(${TARGET} ${TARGET_SRC})

ENABLE_TESTING()

foreach(number 1)
  add_test(NAME IMLogfile${number} COMMAND IMtrajectoryModel -t IMLogfile ${number})
endforeach()

foreach(number 1 2 3 4 5 6 7 8 9)
  add_test(NAME IMInterpreter${number} COMMAND IMtrajectoryModel -t IMInterpreter ${number})
endforeach()

foreach(number 1 2 3 4 5)
  add_test(NAME IMModel${number} COMMAND IMtrajectoryModel -t IMModel ${number})
endforeach()

foreach(number 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17)
  add_test(NAME IMTrajectory${number} COMMAND IMtrajectoryModel -t IMTrajectory ${number})
endforeach()

foreach(number 1 2 3)
add_test(NAME IMtrajectoryModel_test_sl${number} COMMAND IMtrajectoryModel -sl ../test/simplifiedLogfile/test_sl${number}/test_sl${number}.csv -i ../test/simplifiedLogfile/test_sl${number}/test_sl${number}.ini -o Testing/test_sl${number}.result)
add_test(IMtrajectoryModel_test_sl${number}_Compare ${CMAKE_COMMAND} -E compare_files Testing/test_sl${number}.result
../test/simplifiedLogfile/test_sl${number}/test_sl${number}.result)
endforeach()
