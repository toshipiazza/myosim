#pragma once

#include <myo/myo.hpp>

#include "MyoEventTypes.h"

namespace MyoSim {
class EventRecorder : myo::DeviceListener {
 public:
  enum EventTypes {
    PAIR                = 1 << 0,
    UNPAIR              = 1 << 1,
    CONNECT             = 1 << 2,
    DISCONNECT          = 1 << 3,
    ARM_SYNC            = 1 << 4,
    ARM_UNSYNC          = 1 << 5,
    UNLOCK              = 1 << 6,
    LOCK                = 1 << 7,
    POSE                = 1 << 8,
    ORIENTATION_DATA    = 1 << 9,
    ACCELEROMETER_DATA  = 1 << 10,
    GYROSCOPE_DATA      = 1 << 11,
    RSSI                = 1 << 12,
    EMG                 = 1 << 13,
    ALL_EVENT_TYPES     = (1 << 14) - 1
  };

  explicit EventRecorder(EventTypes event_types);

  // Call this function after each call to Hub::run or Hub::runOnce.
  void endEventLoopGroup(myo::Myo* myo);

  std::map<myo::Myo*, EventSession> getEventSession() const { return events_; }
  EventSession getEventSession(myo::Myo* myo) const {
    if (events_.count(myo) == 0) return EventSession();
    return events_.at(myo);
  }

  ///////////////////////////////////////////////////////////////////////
  // virtual event functions that override functions in DeviceListener //
  ///////////////////////////////////////////////////////////////////////
  virtual void onPair(myo::Myo* myo, uint64_t timestamp,
                      myo::FirmwareVersion firmwareVersion) override;
  virtual void onUnpair(myo::Myo* myo, uint64_t timestamp) override;
  virtual void onConnect(myo::Myo* myo, uint64_t timestamp,
                         myo::FirmwareVersion firmwareVersion) override;
  virtual void onDisconnect(myo::Myo* myo, uint64_t timestamp) override;
  virtual void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm,
                         myo::XDirection xDirection) override;
  virtual void onArmUnsync(myo::Myo* myo, uint64_t timestamp) override;
  virtual void onUnlock(myo::Myo* myo, uint64_t timestamp) override;
  virtual void onLock(myo::Myo* myo, uint64_t timestamp) override;
  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      myo::Pose pose) override;
  virtual void onOrientationData(
      myo::Myo* myo, uint64_t timestamp,
      const myo::Quaternion<float>& rotation) override;
  virtual void onAccelerometerData(myo::Myo* myo, uint64_t timestamp,
                                   const myo::Vector3<float>& accel) override;
  virtual void onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                               const myo::Vector3<float>& gyro) override;
  virtual void onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi) override;
  virtual void onEmgData(myo::Myo* myo, uint64_t timestamp,
                         const int8_t* emg) override;

 private:
  // Add an event to the current event group.
  void addEvent(myo::Myo* myo, const std::shared_ptr<MyoEvent>& event);

  EventTypes event_types_;
  std::map<myo::Myo*, EventSession> events_;
};

EventRecorder::EventRecorder(EventTypes event_types)
    : event_types_(event_types) {}

void EventRecorder::endEventLoopGroup(myo::Myo* myo) {
  events_[myo].events.push_back(EventLoopGroup());
}

void EventRecorder::onPair(myo::Myo* myo, uint64_t timestamp,
                           myo::FirmwareVersion firmwareVersion) {
  if (!(event_types_ & PAIR)) return;
  addEvent(myo, std::make_shared<onPairEvent>(timestamp, firmwareVersion));
}

void EventRecorder::onUnpair(myo::Myo* myo, uint64_t timestamp) {
  if (!(event_types_ & UNPAIR)) return;
  addEvent(myo, std::make_shared<onUnpairEvent>(timestamp));
}

void EventRecorder::onConnect(myo::Myo* myo, uint64_t timestamp,
                              myo::FirmwareVersion firmwareVersion) {
  if (!(event_types_ & CONNECT)) return;
  addEvent(myo, std::make_shared<onConnectEvent>(timestamp, firmwareVersion));
}

void EventRecorder::onDisconnect(myo::Myo* myo, uint64_t timestamp) {
  if (!(event_types_ & DISCONNECT)) return;
  addEvent(myo, std::make_shared<onDisconnectEvent>(timestamp));
}

void EventRecorder::onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm,
                              myo::XDirection xDirection) {
  if (!(event_types_ & ARM_SYNC)) return;
  addEvent(myo, std::make_shared<onArmSyncEvent>(timestamp, arm, xDirection));
}

void EventRecorder::onArmUnsync(myo::Myo* myo, uint64_t timestamp) {
  if (!(event_types_ & ARM_UNSYNC)) return;
  addEvent(myo, std::make_shared<onArmUnsyncEvent>(timestamp));
}

void EventRecorder::onUnlock(myo::Myo* myo, uint64_t timestamp) {
  if (!(event_types_ & UNLOCK)) return;
  addEvent(myo, std::make_shared<onUnlockEvent>(timestamp));
}

void EventRecorder::onLock(myo::Myo* myo, uint64_t timestamp) {
  if (!(event_types_ & LOCK)) return;
  addEvent(myo, std::make_shared<onLockEvent>(timestamp));
}

void EventRecorder::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
  if (!(event_types_ & POSE)) return;
  addEvent(myo, std::make_shared<onPoseEvent>(timestamp, pose));
}

void EventRecorder::onOrientationData(myo::Myo* myo, uint64_t timestamp,
                                      const myo::Quaternion<float>& rotation) {
  if (!(event_types_ & ORIENTATION_DATA)) return;
  addEvent(myo, std::make_shared<onOrientationDataEvent>(timestamp, rotation));
}

void EventRecorder::onAccelerometerData(myo::Myo* myo, uint64_t timestamp,
                                        const myo::Vector3<float>& accel) {
  if (!(event_types_ & ACCELEROMETER_DATA)) return;
  addEvent(myo, std::make_shared<onAccelerometerDataEvent>(timestamp, accel));
}

void EventRecorder::onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                                    const myo::Vector3<float>& gyro) {
  if (!(event_types_ & GYROSCOPE_DATA)) return;
  addEvent(myo, std::make_shared<onGyroscopeDataEvent>(timestamp, gyro));
}

void EventRecorder::onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi) {
  if (!(event_types_ & RSSI)) return;
  addEvent(myo, std::make_shared<onRssiEvent>(timestamp, rssi));
}

void EventRecorder::onEmgData(myo::Myo* myo, uint64_t timestamp,
                              const int8_t* const emg) {
  if (!(event_types_ & EMG)) return;
  addEvent(myo, std::make_shared<onEmgDataEvent>(timestamp, emg));
}

void EventRecorder::addEvent(myo::Myo* myo,
                             const std::shared_ptr<MyoEvent>& event) {
  events_[myo].events.back().events.push_back(event);
}
}
