#ifndef THRESHOLDSETTINGS_H
#define THRESHOLDSETTINGS_H

struct ThresholdSettings{
  bool useAdaptiveThreshold;
  int thresholdValue;
  int thresholdType;
  int adaptiveMode;
  int adaptiveThresholdBlocksize;
  double adaptiveThresholdConstantC;
};

#endif