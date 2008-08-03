// -*- mode: c++;-*-
// $Id: StjTrackCutDca.h,v 1.4 2008/08/03 00:26:36 tai Exp $
// Copyright (C) 2008 Tai Sakuma <sakuma@bnl.gov>
#ifndef STJTRACKCUTDCA_H
#define STJTRACKCUTDCA_H

#include "StjTrackCut.h"

class StjTrackCutDca : public StjTrackCut {
  // to reduce pile up tracks

public:
  StjTrackCutDca(double max = 3.0, double min = 0.0) : _max(max), _min(min) { }
  virtual ~StjTrackCutDca() { }

  bool operator()(const StjTrack& track)
  {
    if(track.Tdca > _max) return true;

    if(track.Tdca < _min) return true;

    return false;
  }

private:

  double  _max;
  double  _min;

};

#endif // STJTRACKCUTDCA_H
