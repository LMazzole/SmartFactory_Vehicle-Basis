// /**
//  * @file Vision.h
//  * @brief TODO Library for mechatronic component vision.
//  *
//  * The vision module consists of a pixy-cam and a servo motor.
//  * The cam searches for target-objects and adjusts its position to that direction.
//  * Afterwarts it sends information for course adaption.
//  *
//  * @author Glenn Huber (glenn.patrick.huber@hsr.ch)
//  *
//  * @version 1.1 - Added Doxygen-Documentation  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
//  * @version 1.0 - BA FTS FS 2018
//  *
//  * @date 2019-03-20
//  * @copyright Copyright (c) 2019
//  *
//  */

// #ifndef vision_h
// #define vision_h

// #include "Arduino.h"
// #include "Configuration.h"
// #include "LogConfiguration.h"
// #include "Modular.h"
// #include "PixyI2C.h"
// #include "Servo.h"
// #include "Wire.h"

// /**
//  * @brief
//  *
//  */
// struct VisionState {
//     int target;                   ///<
//     int servoAngle;               ///<
//     bool targetDetected = false;  ///<
//     bool reset = false;           ///<
// };

// /**
//  * @brief
//  *
//  */
// class Vision : public Component {
//    public:
//     /**
//     * @brief Construct a new Vision object
//     *
//     * @param startAngle -
//     * @param servoPin -
//     * @param dddelay -
//     * @param toleranceL -
//     * @param toleranceR -
//     */
//     Vision(int startAngle, int servoPin, int dddelay, int toleranceL, int toleranceR);

//     /**
//      * @brief
//      *
//      * @param state -
//      */
//     void loop(VisionState *state);

//     /**
//      * @brief
//      *
//      * @param angle - right(0 degree)-staight(90 degree)-left(180 degree)
//      *
//      * @todo this is the same servo as in Sonar.. but somehow with other parameter?
//      *
//      */
//     void turnVision(int angle);

//     /**
//      * @brief Testfunction for Vision-Class
//      *
//      * 0 - run all tests \n
//      * 1 - run turnVision-Servo test right(0 degree)-staight(90 degree)-left(180 degree)-turn \n
//      *
//      * @param test - Choose which test to run
//      */
//     void Test(const int test);

//     /**
//      * @brief reset Vision
//      *
//      */
//     void reset(VisionState *state);

//     VisionState visionState;  ///<

//    protected:
//     Servo visionServo;  ///<
//     PixyI2C pixy;       ///<

//     int toleranceLeft;   ///<
//     int toleranceRight;  ///<

//     int target1;       ///<
//     int target2;       ///<
//     int target3;       ///<
//     int target4;       ///<
//     int target5;       ///<
//     int target6;       ///<
//     int target7;       ///<
//     int target8;       ///<
//     int targetFound;   ///<
//     int targetFound1;  ///<
//     int targetFound2;  ///<
//     int targetFound3;  ///<
//     int targetFound4;  ///<
//     int ddelay;        ///<
//     int failCounter;   ///<
//     int _servoPin;     ///<
// };

// #endif