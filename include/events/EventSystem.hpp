//
// Copyright 2015 infinideas
//

#ifndef NEWORLD_EVENT_SYSTEM_HPP

#include "../NativeSupport.hpp"
#include "../logging/LogSystem.hpp"

#include <atomic>

#include <boost/signals2.hpp>

#define EventType boost::signals2::signal

/**
 * 程序退出事件
 */
extern EventType<void()> ApplicationQuitEvent;

/**
 * 窗口改变大小事件
 */
extern EventType<void(int, int)> WindowResizeEvent;

/**
 * FPS数据更新事件
 */
extern EventType<void(float)> FPSReportEvent;

/**
 * TPS数据更新事件
 */
extern EventType<void(float)> TPSReportEvent;

/**
 * 处理SDL2中的事件
 */
void DoEvents();

/**
 * 链接事件
 * @param signal 事件信号
 * @param slot   接口
 */
template <typename SignalType, typename SlotType>
void ConnectEvent(SignalType &signal, SlotType &slot);

/**
 * 链接事件到链表尾部
 * @param signal 事件信号
 * @param slot   接口
 */
template <typename SignalType, typename SlotType>
void ConnectEventAtBack(SignalType &signal, SlotType &slot);

/**
 * 链接事件到链表前部
 * @param signal 事件信号
 * @param slot   接口
 */
template <typename SignalType, typename SlotType>
void ConnectEventAtFront(SignalType &signal, SlotType &slot);

/**
 * 断开与事件的链接
 * @param signal 事件信号
 * @param slot   要断开的接口
 */
template <typename SignalType, typename SlotType>
void DisconnectEvent(SignalType &signal, SlotType &slot);

/**
 * 断开某一事件所有的接口
 */
template <typename SignalType>
void DisconnectAllEvent(SignalType &signal);

// 实现部分

template <typename SignalType, typename SlotType>
void ConnectEvent(SignalType &signal, SlotType &slot) {
    signal.connect(slot);
}

template <typename SignalType, typename SlotType>
void ConnectEventAtBack(SignalType &signal, SlotType &slot) {
    signal.connect(slot, boost::signals2::at_back);
}

template <typename SignalType, typename SlotType>
void ConnectEventAtFront(SignalType &signal, SlotType &slot) {
    signal.connect(slot, boost::signals2::at_front);
}

template <typename SignalType, typename SlotType>
void DisconnectEvent(SignalType &signal, SlotType &slot) {
    signal.disconnect(slot);
}

template <typename SignalType>
void DisconnectEventAll(SignalType &signal) {
    signal.disconnect_all_slots();
}

#endif
// IFNDEF NEWORLD_EVENT_SYSTEM_HPP
