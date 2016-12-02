/*
 * Copyright 2016 Andrii Maksai
 */
#include "CenterlineWithWidthPatternTest.h"
#include "GroundPlaneTransitionProviderTest.h"
#include "ParametersTest.h"
#include "TrackingOptimizerTest.h"
#include "TransformationTest.h"
#include "IDFMatch3DPatternTest.h"
#include "MetricTest.h"
#include "PatternOptimizerTest.h"
#include "Parameters.h"
#include "SolverTest.h"
int main() {
  ParametersTest().testAll();
  Parameters::read_config("resources/test.cfg");
  GroundPlaneTransitionProviderTest().testAll();
  CenterlineWithWidthPatternTest().testAll();
  TrackingOptimizerTest().testAll();
  TransformationTest().testAll();
  IDFMatch3DPatternTest().testAll();
  MetricTest().testAll();
  PatternOptimizerTest().testAll();
  SolverTest().testAll();
}
