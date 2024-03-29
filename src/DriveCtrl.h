/**
 * @file DriveCtrl.h
 * @brief The Drive Controll class contains the FSM for the Drive
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - Implementation Drive Controll FSM - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-23
 * 
 * @date 2019-04-23
 * @copyright Copyright (c) 2019
 * 
 * @todo Improve linedetection with something like: "where was the line first detected" and NOT with a (nonblocking) delay.
 *  This would allow to change the speed and still detect all lines correctly
 */

#ifndef DRIVECTRL_H__
#define DRIVECTRL_H__

#include "HardwareConfiguration.h"
#include "LogConfiguration.h"

#include <PID_v1.h>

#include "Drive/Drive.h"
#include "EnvironmentDetection.h"

/**
 * @brief Contains the FSM to controll the Drive
 * 
 * @image html DriveCtrl.png width=1000
 */
class DriveCtrl {
    //=====PUBLIC====================================================================================
   public:
    /**
    * @brief Enum holds all possible events
    * 
    */
    enum class Event { TurnLeft,            ///< Ext.: Turn left
                       TurnRight,           ///< Ext.: Turn right
                       TurnAround,          ///< Ext.: Turn around on position
                       FollowLineForward,   ///< Ext.: Follow the line forwards
                       FollowLineBackward,  ///< Ext.: Follow the line backwards
                       FullLineDetected,    ///< Signal: Full line detected
                       LineAligned,         ///< Signal: Line is alligned in the middle of the vehicle
                       Error,               ///< Ext.: Error occured
                       Resume,              ///< Ext.: Resume after Error occured
                       Reset,               ///< Ext.: Reset after Error occured
                       NoEvent              ///< No event generated
    };

    /**
    * @brief Enum holds all possible states
    * 
    */
    enum class State { idle,                   ///< idle state
                       turningLeft,            ///< turning left state
                       turningRight,           ///< turning right state
                       turningAround,          ///< turning around state
                       followingLineForward,   ///< follow the line while driving forward state
                       followingLineBackward,  ///< follow the line while driving backward state
                       resetState,             ///< Reset state
                       errorState              ///< Error state
    };

    /**
     * @brief Construct a new Drive Ctrl object
     * and set the currentState with idle state and initialize the PID-Controller 
     * 
     */
    DriveCtrl();

    /**
     * @brief Calls the do-function of the active state and hence generates Events
     * 
     */
    void loop();

    /**
     * @brief procceses the current Event and calls the do-function of the active state
     * 
     * @param currentEvent - Event
     */
    void loop(Event currentEvent);

    /**
     * @brief Get the current State
     * 
     * @return State - current State
     */
    const State getcurrentState();

    //=====PRIVATE====================================================================================
   private:
    State lastStateBevorError;  ///< holds the last state of the FSM so it's possible to resume after error
    State currentState;         ///< holds the current state of the FSM
    Event currentEvent;         ///< holds the current event of the FSM

    /**
     * @brief Functionpointer to call the current states do-function
     * 
     * https://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
     */
    Event (DriveCtrl::*doActionFPtr)(void) = nullptr;

    unsigned long currentMillis = 0;                      ///< will store current time
    unsigned long previousMillis = 0;                     ///< will store last time
    const unsigned long ignoreSensorTurnMillis = 500;     ///< sets how long the sensors are ignored for a turn
    const unsigned long ignoreSensorAroundMillis = 1500;  ///< sets how long the sensors are ignored for a turning around

    double pController_Input = 0;          ///< Controller Input
    double pController_Output = 0;         ///< Controller Output
    double pController_Setpoint = 0;       ///< Controller Setpoint
    int pSampleTime = 50;                  ///< Controler SampleTime in ms
    double pVal_p = PID_KP;                ///< P-Value
    double pVal_i = PID_KI / pSampleTime;  ///< I-Value (independet of SamplingTime)
    double pVal_d = PID_KD * pSampleTime;  ///< D-Value (independet of SamplingTime)

    /**
     * @brief PID-Controller Object
     * 
     *   https://www.quora.com/What-would-be-appropriate-tuning-factors-for-PID-line-follower-robot
     *  1. Set all the gains to zero
     *  2. Increase only Kp (Proportional gain) to get an ultimate oscillating value Kp(max)
     *  3. Increase Kd (Derivative gain) until the oscillations disappear.
     *  4. Repeat steps 2 and 3 until increasing Kd does not dampen the oscillations
     *  5. Now increase Ki (Integral gain) to get a good system with desired number of oscillations (Ideally zero)
     * 
     * @todo Improve controller and make it adaptive to speedchanges
     */
    PID pController = PID(&pController_Input, &pController_Output, &pController_Setpoint, pVal_p, pVal_i, pVal_d, DIRECT);
    Drive pDrive = Drive(RIGHT_MOTOR, LEFT_MOTOR);  ///< Drive Object
    EnvironmentDetection pEnvdetect;                ///< EnviromentDetection Object

    //=====PrivateFunctions=========================================================================
    /**
     * @brief changes the state of the FSM based on the event
     * 
     * @param e - Event
     */
    void process(Event e);

    //=====StateFunctions=====
    //=====idle==========================================================
    /**
     * @brief executes the entry action of the idle
     * 
     */
    void entryAction_idle();

    /**
     * @brief executes the main action of the idle
     * 
     * This is an idle-state.
     * Return NoEvent.
     * 
     * @return DriveCtrl::Event - generated Event
     */
    DriveCtrl::Event doAction_idle();

    /**
     * @brief executes the exit action of the idle
     * 
     */
    void exitAction_idle();

    //=====turningLeft==========================================================
    /**
     * @brief executes the entry action of the turningLeft 
     * 
     * Start Turning Left.
     * 
     */
    void entryAction_turningLeft();

    /**
     * @brief executes the main action of the turningLeft
     * 
     * Turn Left until the line is aligned again. Return LineAligned
     * else return no Event.
     * 
     * @return DriveCtrl::Event - generated Event
     */
    DriveCtrl::Event doAction_turningLeft();

    /**
     * @brief executes the exit action of the turningLeft
     * 
     * Stop Driving.
     */
    void exitAction_turningLeft();

    //=====turningRight==========================================================
    /**
     * @brief executes the entry action of the turningRight 
     * 
     * Start Turning Right.
     */
    void entryAction_turningRight();

    /**
     * @brief executes the main action of the turningRight
     * 
     * Turn Right until the line is aligned again. Return LineAligned
     * else return no Event.
     * 
     * @return DriveCtrl::Event - generated Event
     */
    DriveCtrl::Event doAction_turningRight();

    /**
     * @brief executes the exit action of the turningRight
     * 
     * Stop Driving.
     */
    void exitAction_turningRight();

    //=====turningAround==========================================================
    /**
     * @brief executes the entry action of the turningAround 
     * 
     * Start Turning Right.
     */
    void entryAction_turningAround();

    /**
     * @brief executes the main action of the turningAround
     * 
     * Turn Right until the line is aligned again. Return LineAligned
     * else return no Event.
     * 
     * @return DriveCtrl::Event - generated Event
     */
    DriveCtrl::Event doAction_turningAround();

    /**
     * @brief executes the exit action of the turningAround
     * 
     * Stop Driving.
     */
    void exitAction_turningAround();

    //=====followingLineForward==========================================================
    /**
     * @brief executes the entry action of the followingLineForward 
     * 
     * Start Driving Straight
     */
    void entryAction_followingLineForward();

    /**
     * @brief executes the main action of the followingLineForward
     * 
     * Check Line-deviation and correct accordingly via PID-Controller
     * and drive until a FullLine is Detected. Return Event FullLine else reutrn NoEvent.
     * 
     * @return DriveCtrl::Event - generated Event
     */
    DriveCtrl::Event doAction_followingLineForward();

    /**
     * @brief executes the exit action of the followingLineForward
     * 
     * Stop driving.
     */
    void exitAction_followingLineForward();

    //=====followingLineBackward==========================================================
    /**
     * @brief executes the entry action of the followingLineBackward 
     * 
     * Start Driving Straight
     */
    void entryAction_followingLineBackward();

    /**
     * @brief executes the main action of the followingLineBackward
     * 
     * Check Line-deviation and correct accordingly via PID-Controller
     * and drive until a FullLine is Detected. Return Event FullLine else reutrn NoEvent.
     * 
     * @return DriveCtrl::Event - generated Event
     */
    DriveCtrl::Event doAction_followingLineBackward();

    /**
     * @brief executes the exit action of the followingLineBackward
     * 
     * Stop driving.
     */
    void exitAction_followingLineBackward();
    //==errorState==========================================================
    /**
     * @brief entry action of the errorState
     * 
     */
    void entryAction_errorState();

    /**
     * @brief main action of the errorState
     * 
     *  @return DriveCtrl::Event - generated Event
     */
    DriveCtrl::Event doAction_errorState();

    /**
     * @brief exit action of the errorState
     * 
     */
    void exitAction_errorState();

    //==resetState==========================================================
    /**
     * @brief entry action of the resetState
     * 
     */
    void entryAction_resetState();

    /**
     * @brief main action of the resetState
     * 
     *  @return DriveCtrl::Event - generated Event
     */
    DriveCtrl::Event doAction_resetState();

    /**
     * @brief exit action of the resetState
     * 
     */
    void exitAction_resetState();
    //============================================================================
    //==Aux-Function==============================================================
    /**
     * @brief Decodes the State-Enum and returns a description
     * 
     * @param state - enum State
     * @return String - State as String
     */
    String decodeState(State state);

    /**
     * @brief Decodes the Event-Enum and returns a description
     * 
     * @param event - enum Event
     * @return String - Event as String
     */
    String decodeEvent(Event event);
};
#endif
