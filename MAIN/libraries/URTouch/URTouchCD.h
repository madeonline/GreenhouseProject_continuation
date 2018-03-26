// URTouchCD.h
// ----------
//
// Since there are slight deviations in all touch screens you should run a
// calibration on your display module. Run the URTouch_Calibration sketch
// that came with this library and follow the on-screen instructions to
// update this file.
//
// Remember that is you have multiple display modules they will probably 
// require different calibration data so you should run the calibration
// every time you switch to another module.
// You can, of course, store calibration data for all your modules here
// and comment out the ones you dont need at the moment.
//

// These calibration settings works with my ITDB02-3.2S.
// They MIGHT work on your 320x240 display module, but you should run the
// calibration sketch anyway. If you are using a display with any other
// resolution you MUST calibrate it as these settings WILL NOT work.

/*
#define CAL_X 0x00378F66UL
#define CAL_Y 0x03C34155UL
#define CAL_S 0x000EF13FUL
*/
//7.0"
//#define CAL_X 0x00170F93UL
//#define CAL_Y 0x005D8F04UL
//#define CAL_S 0x8031F1DFUL

// 2,0"
#define CAL_X 0x005B0EE7UL
#define CAL_Y 0x03BC41CFUL
#define CAL_S 0x000AE0DBUL