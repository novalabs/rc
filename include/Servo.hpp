/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/mw/Subscriber.hpp>
#include <core/mw/CoreNode.hpp>

#include <core/rc/ServoConfiguration.hpp>

#include <ModuleConfiguration.hpp>

#include <core/common_msgs/Array12_u16.hpp>
#include <core/hw/PWM.hpp>

namespace core {
namespace rc {

class Servo:
    public core::mw::CoreNode,
    public core::mw::CoreConfigurable<core::rc::ServoConfiguration>
{
public:
	Servo(
        const char*                           name,
        core::os::Thread::Priority            priority = core::os::Thread::PriorityEnum::NORMAL
    );

    virtual
    ~Servo();

private:
    core::mw::Subscriber<core::common_msgs::Array12_u16, 5> _subscriber;
    uint16_t _values[12];
    bool _failsafe;

    core::hw::PWMMaster_<core::hw::PWM_1> _pwm1;
    core::hw::PWMMaster_<core::hw::PWM_2> _pwm2;
    core::hw::PWMMaster_<core::hw::PWM_3> _pwm3;

    core::hw::PWMChannel_<core::hw::PWM_1, 0> _channel6;
    core::hw::PWMChannel_<core::hw::PWM_1, 1> _channel7;
    core::hw::PWMChannel_<core::hw::PWM_1, 2> _channel5;
    core::hw::PWMChannel_<core::hw::PWM_1, 3> _channel4;

    core::hw::PWMChannel_<core::hw::PWM_2, 0> _channel8;
    core::hw::PWMChannel_<core::hw::PWM_2, 1> _channel9;
    core::hw::PWMChannel_<core::hw::PWM_2, 2> _channel10;
    core::hw::PWMChannel_<core::hw::PWM_2, 3> _channel11;

    core::hw::PWMChannel_<core::hw::PWM_3, 0> _channel0;
    core::hw::PWMChannel_<core::hw::PWM_3, 1> _channel1;
    core::hw::PWMChannel_<core::hw::PWM_3, 2> _channel2;
    core::hw::PWMChannel_<core::hw::PWM_3, 3> _channel3;
private:
    bool
    onPrepareMW();

    bool
    onConfigure();

    bool
    onPrepareHW();

    bool
    onStart();

    bool
    onLoop();
    bool
    onStop();

    static bool
    callback(
        const core::common_msgs::Array12_u16& msg,
        void*                                 context
    );
};
}
}
