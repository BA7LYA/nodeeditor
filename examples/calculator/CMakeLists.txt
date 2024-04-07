set(CALC_SOURCE_FILES
  main.cxx
  MathOperationDataModel.cxx
  NumberDisplayDataModel.cxx
  NumberSourceDataModel.cxx
)

set(CALC_HEADER_FILES
  AdditionModel.hxx
  DivisionModel.hxx
  DecimalData.hxx
  MathOperationDataModel.hxx
  NumberDisplayDataModel.hxx
  NumberSourceDataModel.hxx
  SubtractionModel.hxx
)

add_executable(calculator
  ${CALC_SOURCE_FILES}
  ${CALC_HEAEDR_FILES}
)

target_link_libraries(calculator QtNodes)

set(HEADLESS_CALC_SOURCE_FILES
  headless_main.cxx
  MathOperationDataModel.cxx
  NumberDisplayDataModel.cxx
  NumberSourceDataModel.cxx
)

add_executable(headless_calculator 
  ${HEADLESS_CALC_SOURCE_FILES}
  ${CALC_HEAEDR_FILES}
)

target_link_libraries(headless_calculator QtNodes)
