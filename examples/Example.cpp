#include <iostream>
#include <myo/myo.hpp>
#include "../src/SimulatedHub.h"

class PrintListener : public myo::DeviceListener {
 public:
  void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
    std::cout << "Detected pose! " << pose << std::endl;
  }
};

class VibrateListener : public myo::DeviceListener {
 public:
  void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
    if (myo) myo->vibrate(myo::Myo::vibrationShort);
  }
};

int main() {
  try {
    MyoSimulator::Hub hub("com.voidingwarranties.myo-simulator-example");

    myo::Myo* myo = hub.waitForMyo(1000);
    if (!myo) {
      std::cerr << "Unable to find a Myo! Simulating poses will still work, "
                   "but the myo pointer is NULL." << std::endl;
    }

    PrintListener print_listener;
    VibrateListener vibrate_listener;
    hub.addListener(&print_listener);
    hub.addListener(&vibrate_listener);

    while (true) {
      hub.run(0);
    }
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
  }
  return 0;
}