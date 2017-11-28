/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */
#include <core/rc/Servo.hpp>

namespace core {
namespace rc {

static PWMConfig pwm1_config = {
		1000000, /* 1 MHz PWM clock frequency.   */
		20000, /* 50 Hz frequency. */
		nullptr,
		{{ PWM_OUTPUT_ACTIVE_HIGH, NULL },
		 { PWM_OUTPUT_ACTIVE_HIGH, NULL },
		 { PWM_OUTPUT_ACTIVE_HIGH, NULL },
		 { PWM_OUTPUT_ACTIVE_HIGH, NULL } },
		 0, 0
};

static PWMConfig pwm2_config = {
		1000000, /* 1 MHz PWM clock frequency.   */
		20000, /* 50 Hz frequency. */
		nullptr,
		{{ PWM_OUTPUT_ACTIVE_HIGH, NULL },
		 { PWM_OUTPUT_ACTIVE_HIGH, NULL },
		 { PWM_OUTPUT_ACTIVE_HIGH, NULL },
		 { PWM_OUTPUT_ACTIVE_HIGH, NULL } },
		 0, 0
};

static PWMConfig pwm3_config = {
		1000000, /* 1 MHz PWM clock frequency.   */
		20000, /* 50 Hz frequency. */
		nullptr,
		{{ PWM_OUTPUT_ACTIVE_HIGH, NULL },
		 { PWM_OUTPUT_ACTIVE_HIGH, NULL },
		 { PWM_OUTPUT_ACTIVE_HIGH, NULL },
		 { PWM_OUTPUT_ACTIVE_HIGH, NULL } },
		 0, 0
};

Servo::Servo(
        const char*                           name,
        core::os::Thread::Priority            priority
    ) :
        CoreNode::CoreNode(name, priority),
        CoreConfigurable<core::rc::ServoConfiguration>::CoreConfigurable(name),
		_failsafe(true)
    {
        _workingAreaSize = 512;
    }

	Servo::~Servo()
    {
        teardown();
    }


    bool
	Servo::onPrepareMW()
    {
        _subscriber.set_callback(Servo::callback);
        this->subscribe(_subscriber, configuration().topic);

        return true;
    }

    bool
	Servo::onConfigure()
    {
        return isConfigured();
    }

    bool
	Servo::onPrepareHW()
    {
    	_pwm1.start(pwm1_config);
    	_pwm2.start(pwm2_config);
    	_pwm3.start(pwm3_config);

    	_channel0.disable();
    	_channel1.disable();
    	_channel2.disable();
    	_channel3.disable();
    	_channel4.disable();
    	_channel5.disable();
    	_channel6.disable();
    	_channel7.disable();
    	_channel8.disable();
    	_channel9.disable();
    	_channel10.disable();
    	_channel11.disable();

    	_pwm1.setCallback([this]() {
    		core::os::SysLock::ISRScope lock;
    		if(this->_failsafe) {
    			this->_channel4.setI(this->configuration().failsafe[4]);
    			this->_channel5.setI(this->configuration().failsafe[5]);
    			this->_channel6.setI(this->configuration().failsafe[6]);
    			this->_channel7.setI(this->configuration().failsafe[7]);
    		} else {
    			this->_channel4.setI(this->_values[4]);
    			this->_channel5.setI(this->_values[5]);
    			this->_channel6.setI(this->_values[6]);
    			this->_channel7.setI(this->_values[7]);
    		}
    	});

    	_pwm2.setCallback([this]() {
    		core::os::SysLock::ISRScope lock;
    		if(this->_failsafe) {
    			this->_channel8.setI(this->configuration().failsafe[8]);
    			this->_channel9.setI(this->configuration().failsafe[9]);
    			this->_channel10.setI(this->configuration().failsafe[10]);
    			this->_channel11.setI(this->configuration().failsafe[11]);
    		} else {
    			this->_channel8.setI(this->_values[8]);
    			this->_channel9.setI(this->_values[9]);
    			this->_channel10.setI(this->_values[10]);
    			this->_channel11.setI(this->_values[11]);
    		}
    	});

    	_pwm3.setCallback([this]() {
    		core::os::SysLock::ISRScope lock;
    		if(this->_failsafe) {
    			this->_channel0.setI(this->configuration().failsafe[0]);
    			this->_channel1.setI(this->configuration().failsafe[1]);
    			this->_channel2.setI(this->configuration().failsafe[2]);
    			this->_channel3.setI(this->configuration().failsafe[3]);
    		} else {
    			this->_channel0.setI(this->_values[0]);
    			this->_channel1.setI(this->_values[1]);
    			this->_channel2.setI(this->_values[2]);
    			this->_channel3.setI(this->_values[3]);
    		}
    	});

    	_pwm1.enableCallback();
    	_pwm2.enableCallback();
    	_pwm3.enableCallback();

    	return true;
    }

    bool
	Servo::onStart()
    {
    	_failsafe = true;

        return true;
    }

    bool
	Servo::onLoop()
    {
		_failsafe = !this->spin(core::os::Time::ms(1000));

        return true;
    }

    bool
	Servo::onStop()
    {
    	_pwm1.stop();
    	_pwm2.stop();
    	_pwm3.stop();

    	return true;
    }

    bool
	Servo::callback(
        const core::common_msgs::Array12_u16& msg,
        void*                                 context
    )
    {
    	Servo* _this = static_cast<Servo*>(context);

    	for (int i = 0; i < 12; i++) {
    		uint16_t p = msg.values[i];

    		if((p >= 500) && (p <= 2500)) {
    			_this->_values[i] = p;
    		}
    	}

    	_this->_failsafe = false;

        return true;
    }

}
}
