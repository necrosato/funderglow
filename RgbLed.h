//
// RgbLed.h
// Naookie Sato
//
// Created by Naookie Sato on 08/01/2020
// Copyright © 2020 Naookie Sato. All rights reserved.
//

#ifndef _RGBLED_H_
#define _RGBLED_H_

struct RgbLed
{
  RgbLed(int rPin, int gPin, int bPin) : pins_({rPin, gPin, bPin})
  {
    pinMode(this->rPin(), OUTPUT);
    pinMode(this->gPin(), OUTPUT);
    pinMode(this->bPin(), OUTPUT);
    this->off();
  }

  const int & rPin() const
  {
    return pins_[0];
  }

  const int & gPin() const
  {
    return pins_[1];
  }

  const int & bPin() const
  {
    return pins_[2];
  }

  void red(int val)
  {
    analogWrite(rPin(), val);
  }

  void green(int val)
  {
    analogWrite(gPin(), val);
  }

  void blue(int val)
  {
    analogWrite(bPin(), val);
  }

  void off()
  {
    red(0);
    green(0);
    blue(0);
  }

  void on()
  {
    red(255);
    green(255);
    blue(255);
  }

  const std::vector<int> & pins() const
  {
    return pins_;
  }

  std::vector<int> pins_;
};

#endif  // _RGBLED_H_
